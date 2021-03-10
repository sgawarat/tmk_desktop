/**
 * @file sink.cpp
 * @brief Sink
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include "sink.hpp"
#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <thread>
#include <Windows.h>
#include <tmk_desktop/bitset.hpp>
#include <tmk_desktop/engine/keyboard.hpp>
#include <tmk_desktop/engine/mapping.hpp>
#include "host_driver.hpp"
#include "key.hpp"
#include "logging.hpp"
#include "settings.hpp"
#include "utility.hpp"

extern "C" {
#include <tmk_core/common/action_layer.h>
}  // extern "C"

namespace tmk_desktop::win32 {
namespace {
/**
 * @brief キー入力イベントを格納するクラス
 */
class KeyEvent {
public:
  constexpr KeyEvent() noexcept = default;

  constexpr KeyEvent(const KBDLLHOOKSTRUCT& info) noexcept
    : vk_(info.vkCode),
      sc_(info.scanCode),
      flags_(info.flags) {}

  Key key() const noexcept {
    return make_key(sc_, !!(flags_ & LLKHF_EXTENDED));
  }

  constexpr bool is_pressed() const noexcept {
    return !(flags_ & LLKHF_UP);
  }

  // friend std::ostream& operator <<(std::ostream& os, const KeyEvent& event) {
  //   os << format("{{vk:{:02x},sc:{:04x},flags:{:08x}}}", event.vk_, event.sc_, event.flags_);
  //   return os;
  // }

private:
  [[maybe_unused]] WORD vk_ = 0;
  WORD sc_ = 0;
  DWORD flags_ = 0;
};

std::thread thread_{};  ///< スレッド
std::atomic<bool> running_{false};  ///< スレッドが動作中かどうか
std::atomic<bool> stop_requested_{false};  ///< スレッドに対する停止要求
OnSinkError on_error_ = nullptr;  ///< 例外発生時に呼ばれる関数

std::deque<KeyEvent> event_queue_;  ///< イベントキュー
std::mutex event_queue_mtx_;
std::condition_variable event_queue_cv_;

GlobalSettingsInfo global_settings_;  ///< グローバル設定
std::array<KeyBehavior, Key::COUNT> key_behaviors_;  ///< 各キーの挙動

Key repeat_key_ = K_UNASSIGNED;  ///< キーリピートしているキー
}  // namespace

bool start_sink(OnSinkError on_error) {
  if (thread_.joinable()) return false;

  // 状態を初期化する
  stop_requested_.store(false, std::memory_order_release);
  on_error_ = on_error;

  // 設定を読み出す
  // TODO: 不正な値への対策を追加する
  global_settings_ = get_global_settings_info();
  {
    key_behaviors_.fill(KeyBehavior::DEFAULT);
    const auto info = get_key_behavior_map_info();
    for (const auto& entry : std::span(info.entries, info.count)) {
      for (const auto& key : std::span{entry.keys, entry.key_count}) {
        if (key.value() < key_behaviors_.size()) key_behaviors_[key.value()] = entry.key_behavior;
      }
    }
  }
  {
    const auto& info = get_action_map_info();
    for (const auto& entry : std::span(info.entries, info.count)) {
      engine::update_action_map(entry.layer, entry.actions, entry.action_count);
    }
  }
  {
    const auto& info = get_macro_map_info();
    for (const auto& entry : std::span(info.entries, info.count)) {
      engine::update_macro(entry.id, entry.macro, 0);
    }
  }

  thread_ = std::thread([] {
    running_.store(true, std::memory_order_release);

    uint8_t im_status = 0;
    while (!stop_requested_.load(std::memory_order_acquire)) {
      KeyEvent event;
      try {
        std::unique_lock lock{event_queue_mtx_};
        if (event_queue_.empty()) {
          event_queue_cv_.wait(lock, [] {
            return !event_queue_.empty() || stop_requested_.load(std::memory_order_acquire);
          });
          if (stop_requested_.load(std::memory_order_acquire)) break;
          if (event_queue_.empty()) continue;
        }
        event = event_queue_.front();
        event_queue_.pop_front();
      } catch (...) {
        if (on_error_) on_error_();
        break;
      }

      // HACK: IMの状態に応じてレイヤーを無理矢理に遷移させる
      im_status = (im_status << 1) | (get_im_status() ? 1 : 0);
      if ((im_status & 0b11) == 0b01) layer_move(global_settings_.layout_for_input_method);
      if ((im_status & 0b11) == 0b10) layer_move(0);

      const auto key = event.key();
      const auto pressed = event.is_pressed();
      if (key.value() >= Key::COUNT) continue;

      switch (key_behaviors_[key.value()]) {
        case KeyBehavior::DEFAULT: {
          if (pressed) {
            if (key == repeat_key_) {
              debug_log("Input: Repeat {}\n", key);
              send_key_repeat();
            } else {
              debug_log("Input: Press {}\n", key);
              clear_key_repeat();
              repeat_key_ = key;
              engine::update_keyboard(key, pressed);
            }
          } else {
            debug_log("Input: Release {}\n", key);
            if (key == repeat_key_) {
              clear_key_repeat();
              repeat_key_ = K_UNASSIGNED;
            }
            engine::update_keyboard(key, pressed);
          }
          break;
        }
        case KeyBehavior::PRESS_ONLY: {
          if (pressed) {
            debug_log("Input: PressOnly {}\n", key);
            engine::update_keyboard(key, true);
            engine::update_keyboard(key, false);
            clear_key_repeat();
            repeat_key_ = K_UNASSIGNED;
          }
          break;
        }
        case KeyBehavior::RELEASE_ONLY: {
          if (!pressed) {
            debug_log("Input: ReleaseOnly {}\n", key);
            engine::update_keyboard(key, true);
            engine::update_keyboard(key, false);
            clear_key_repeat();
            repeat_key_ = K_UNASSIGNED;
          }
          break;
        }
        default: {
          debug_log("Input: Ignore {}\n", key);
          break;
        }
      }

      // CPUを明け渡す
      std::this_thread::yield();
    }

    running_.store(false, std::memory_order_release);
  });

  return true;
}

bool stop_sink() {
  // すでにスレッドが停止しているかを確認する
  if (!thread_.joinable()) return false;

  // すでにスレッドに停止要求が出されているかを確認する
  if (stop_requested_.load(std::memory_order_acquire)) return false;

  // スレッドに停止要求を出す
  stop_requested_.store(true, std::memory_order_release);
  event_queue_cv_.notify_one();

  // スレッドが停止するのを待つ
  thread_.join();

  return true;
}

void send_to_sink(const KBDLLHOOKSTRUCT& info) {
  {
    std::lock_guard lock{event_queue_mtx_};
    event_queue_.emplace_back(info);
  }
  event_queue_cv_.notify_one();
}

SinkStatus get_sink_status() noexcept {
  if (running_.load(std::memory_order_acquire)) {
    if (stop_requested_.load(std::memory_order_acquire)) return SinkStatus::STOPPING;
    return SinkStatus::RUNNING;
  } else {
    if (thread_.joinable()) return SinkStatus::STOPPED;
    return SinkStatus::RESET;
  }
}
}  // namespace tmk_desktop::win32
