/**
 * @file sink.cpp
 * @brief Sink
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <tmk_desktop/sink.hpp>
#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <thread>
#include <tmk_desktop/bitset.hpp>

extern "C" {
#include <common/action.h>
}  // extern "C"

#ifdef _WIN32
#include "win32/sender.hpp"
#endif

namespace tmk_desktop {
namespace {
std::thread thread_{};  ///< スレッド
std::atomic<bool> running_{false};  ///< スレッドが動作中かどうか
std::atomic<bool> stop_requested_{false};  ///< スレッドに対する停止要求

std::deque<SinkEvent> event_queue_;  ///< イベントキュー
std::mutex event_queue_mtx_;  ///< イベントキューのためのMutex
std::condition_variable event_queue_cv_;  ///< イベントキューのためのCV
EventSender sender_;  ///< OSに入力イベントを送るためのクラス

/**
 * @brief SinkEventのvisitor
 */
class SinkEventVisitor final {
public:
  using Keyset = Bitset<256, uint8_t>;
  using ModKeyset = Bitset<8, uint8_t>;

  void operator()(const report_keyboard_t& report) noexcept {
    // 前回のキー状態を保存する
    const auto prev_keyset = keyset_;  // 前回のキー状態
    const auto prev_mod_keyset = mod_keyset_;  // 前回の修飾キー状態

    // 今回のキー状態を記録する
#ifdef NKRO_ENABLE
    keyset_ = Keyset{report.nkro.bits};  // 今回のキー状態
    mod_keyset_ = ModKeyset{report.nkro.mods};  // 今回の修飾キー状態
#else
    keyset_.clear();
    for (auto key : std::span(report.keys)) {
      if (key >= KC_LCTRL && key <= KC_RGUI) continue;
      if (key != 0) keyset_.set(key);
    }
    mod_keyset_ = ModKeyset{report.mods};
#endif

    // 今回の更新で変化するキーを抽出する
    const auto pressed_keyset = reset_to_set(prev_keyset, keyset_);  // 押した
    const auto released_keyset = set_to_reset(prev_keyset, keyset_); // 離した
    const auto pressed_mod_keyset = reset_to_set(prev_mod_keyset, mod_keyset_);  // 押した
    const auto released_mod_keyset = set_to_reset(prev_mod_keyset, mod_keyset_); // 離した

    // キーイベントを送信する
    released_keyset.scan([](auto pos) {  // 非修飾キーを離す
      sender_.send_key_release(pos.index());
    });
    released_mod_keyset.scan([](auto pos) {  // 修飾キーを離す
      sender_.send_key_release(KC_LCTRL + pos.index());
    });
    pressed_mod_keyset.scan([](auto pos) {  // 修飾キーを押す
      sender_.send_key_press(KC_LCTRL + pos.index());
    });
    pressed_keyset.scan([](auto pos) {  // 非修飾キーを押す
      sender_.send_key_press(pos.index());
    });
  }

  void operator()(const report_mouse_t& report) noexcept {
    // TODO: send_mouseを実装する
  }

  void operator()(const HidUsage& usage) noexcept {
    const auto keycode = [=]() -> uint8_t {
      switch (usage.page) {
        case HidUsagePage::GENERIC_DESKTOP_CONTROL:
          switch (usage.id) {
            case SYSTEM_POWER_DOWN: return KC_SYSTEM_POWER;
            case SYSTEM_SLEEP: return KC_SYSTEM_SLEEP;
            case SYSTEM_WAKE_UP: return KC_SYSTEM_WAKE;
          }
          break;
        case HidUsagePage::CONSUMER:
          switch (usage.id) {
            case AUDIO_MUTE: return KC_AUDIO_MUTE;
            case AUDIO_VOL_UP: return KC_AUDIO_VOL_UP;
            case AUDIO_VOL_DOWN: return KC_AUDIO_VOL_DOWN;
            case TRANSPORT_NEXT_TRACK: return KC_MEDIA_NEXT_TRACK;
            case TRANSPORT_PREV_TRACK: return KC_MEDIA_PREV_TRACK;
            case TRANSPORT_STOP: return KC_MEDIA_STOP;
            case TRANSPORT_STOP_EJECT: return KC_MEDIA_EJECT;
            case TRANSPORT_PLAY_PAUSE: return KC_MEDIA_PLAY_PAUSE;
            case APPLAUNCH_CC_CONFIG: return KC_MEDIA_SELECT;
            case APPLAUNCH_EMAIL: return KC_MAIL;
            case APPLAUNCH_CALCULATOR: return KC_CALCULATOR;
            case APPLAUNCH_LOCAL_BROWSER: return KC_MY_COMPUTER;
            case APPCONTROL_SEARCH: return KC_WWW_SEARCH;
            case APPCONTROL_HOME: return KC_WWW_HOME;
            case APPCONTROL_BACK: return KC_WWW_BACK;
            case APPCONTROL_FORWARD: return KC_WWW_FORWARD;
            case APPCONTROL_STOP: return KC_WWW_STOP;
            case APPCONTROL_REFRESH: return KC_WWW_REFRESH;
            case APPCONTROL_BOOKMARKS: return KC_WWW_FAVORITES;
          }
          break;
      }
      return KC_NO;
    }();
    if (keycode != KC_NO) sender_.send_key_tap(keycode);
  }

  void operator()(const NativeSinkEvent& event) noexcept {
    sender_.send_native_event(event);
  }

  void operator()(SinkSignal signal) noexcept {
    switch (signal) {
      case SinkSignal::KEY_REPEAT:
        sender_.send_key_repeat();
        break;
      case SinkSignal::KEY_REPEAT_END:
        sender_.clear_key_repeat();
        break;
    }
  }

private:
  Keyset keyset_{};  ///< 最新のキー状態
  ModKeyset mod_keyset_{};  ///< 最新の修飾キー状態
} visitor_;
}  // namespace

bool start_sink() {
  if (thread_.joinable()) return false;

  // 状態を初期化する
  stop_requested_.store(false, std::memory_order_release);

  thread_ = std::thread([] {
    const struct ScopedRunning {
      ScopedRunning() {running_.store(true, std::memory_order_release);}
      ~ScopedRunning() {running_.store(false, std::memory_order_release);}
    } _running{};

    try {
      const struct ScopedInit {
        ScopedInit() {sender_.enable();}
        ~ScopedInit() {sender_.disable();}
      } _init{};

      while (!stop_requested_.load(std::memory_order_acquire)) {
        SinkEvent event;
        {
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
        }

        // イベントの中身に応じて処理を行う
        std::visit(visitor_, event);

        // CPUを明け渡す
        std::this_thread::yield();
      }
    } catch (std::exception& e) {
      on_sink_error(e);
    }
  });

  return true;
}

bool stop_sink() {
  // すでにスレッドが停止しているかを確認する
  if (!thread_.joinable()) return false;

  // すでにスレッドが実行終了しているかを確認する
  if (!running_.load(std::memory_order_acquire)) return false;

  // スレッドに停止要求を出す
  stop_requested_.store(true, std::memory_order_release);
  event_queue_cv_.notify_one();

  // スレッドが停止するのを待つ
  thread_.join();

  return true;
}

void send_to_sink(const SinkEvent& event) {
  {
    std::lock_guard lock{event_queue_mtx_};
    event_queue_.push_back(event);
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
}  // namespace tmk_desktop
