/**
 * @file keyboard.cpp
 * @brief Keyboard
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <tmk_desktop/keyboard.hpp>
#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <thread>
#include <tmk_desktop/bitset.hpp>
#include <tmk_desktop/sink.hpp>

extern "C" {
#include <common/keyboard.h>
#include <common/action.h>
#include <common/matrix.h>
#include <common/host.h>
#include <common/report.h>
}  // extern "C"

#ifdef _WIN32
#include <tmk_desktop/win32/settings.hpp>
#endif

namespace tmk_desktop {
namespace {
std::thread thread_{};                     ///< スレッド
std::atomic<bool> running_{false};         ///< スレッドが動作中かどうか
std::atomic<bool> stop_requested_{false};  ///< スレッドに対する停止要求

std::deque<KeyEvent> event_queue_;        ///< イベントキュー
std::mutex event_queue_mtx_;              ///< イベントキューのためのMutex
std::condition_variable event_queue_cv_;  ///< イベントキューのためのCV

using Matrix = Bitset<MATRIX_ROWS * MATRIX_COLS, matrix_row_t>;
static constexpr Key NO_REPEAT = Key{KEY_COUNT};  ///< キーリピートしていないことを示す値

Matrix matrix_;               ///< キーボードの状態
Key repeat_key_ = NO_REPEAT;  ///< リピートしているキー

// Sinkにイベントを送信するホストドライバ関数たち
uint8_t keyboard_leds() noexcept {
  return 0;
}
void send_keyboard(report_keyboard_t* report_ptr) noexcept {
  if (report_ptr) send_to_sink(*report_ptr);
}
void send_mouse(report_mouse_t* report_ptr) noexcept {
  if (report_ptr) send_to_sink(*report_ptr);
}
void send_system(uint16_t val) noexcept {
  send_to_sink(HidUsage{HidUsagePage::GENERIC_DESKTOP_CONTROL, val});
}
void send_consumer(uint16_t val) noexcept {
  send_to_sink(HidUsage{HidUsagePage::CONSUMER, val});
}

// 変換表にアクセスする関数
inline keypos_t key_to_keypos(Key key) noexcept {
  if (key >= KEY_COUNT) return {0xff, 0xff};
  return key_to_keypos_table[key];
}
inline bool is_tapping_key(Key key) noexcept {
  if (key >= KEY_COUNT) return false;
  return tapping_key_table[key];
}
}  // namespace

bool start_keyboard() {
  if (thread_.joinable()) return false;

  // 状態を初期化する
  stop_requested_.store(false, std::memory_order_release);

  thread_ = std::thread([] {
    const struct ScopedRunning {
      ScopedRunning() {
        running_.store(true, std::memory_order_release);
      }
      ~ScopedRunning() {
        running_.store(false, std::memory_order_release);
      }
    } _running{};

    try {
      const struct ScopedInit {
        ScopedInit() {
          static host_driver_t driver{
              keyboard_leds, send_keyboard, send_mouse, send_system, send_consumer,
          };
          host_set_driver(&driver);
          keyboard_init();
        }
        ~ScopedInit() {
          clear_keyboard();
          host_set_driver(nullptr);
        }
      } _init{};

      while (!stop_requested_.load(std::memory_order_acquire)) {
        KeyEvent event;
        {
          std::unique_lock lock{event_queue_mtx_};
          if (event_queue_.empty()) {
            event_queue_cv_.wait(lock, [] { return !event_queue_.empty() || stop_requested_.load(std::memory_order_acquire); });
            if (stop_requested_.load(std::memory_order_acquire)) break;
            if (event_queue_.empty()) continue;
          }
          event = event_queue_.front();
          event_queue_.pop_front();
        }

        const auto key = event.key();
        const auto keypos = key_to_keypos(key);
        if (keypos.row < MATRIX_ROWS && keypos.col < MATRIX_COLS) {
          const auto pos = Matrix::Position{keypos.row, keypos.col};
          if (event.is_pressed()) {
            if (key == repeat_key_) {
              send_to_sink(SinkSignal::KEY_REPEAT);
            } else {
              send_to_sink(SinkSignal::KEY_REPEAT_END);
              repeat_key_ = key;
              matrix_.set(pos);
              keyboard_task();

              // 指定のキーはすぐに離す処理を行う
              if (is_tapping_key(key)) {
                send_to_sink(SinkSignal::KEY_REPEAT_END);
                repeat_key_ = NO_REPEAT;
                matrix_.reset(pos);
                keyboard_task();
              }
            }
          } else {
            if (key == repeat_key_) {
              send_to_sink(SinkSignal::KEY_REPEAT_END);
              repeat_key_ = NO_REPEAT;
            }
            matrix_.reset(pos);
            keyboard_task();
          }
        }

        // CPUを明け渡す
        std::this_thread::yield();
      }
    } catch (std::exception& e) {
      on_keyboard_error(e);
    }
  });

  return true;
}

bool stop_keyboard() {
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

void send_to_keyboard(const KeyEvent& event) {
  {
    std::lock_guard lock{event_queue_mtx_};
    event_queue_.emplace_back(event);
  }
  event_queue_cv_.notify_one();
}

KeyboardStatus get_keyboard_status() noexcept {
  if (running_.load(std::memory_order_acquire)) {
    if (stop_requested_.load(std::memory_order_acquire)) return KeyboardStatus::STOPPING;
    return KeyboardStatus::RUNNING;
  } else {
    if (thread_.joinable()) return KeyboardStatus::STOPPED;
    return KeyboardStatus::RESET;
  }
}

extern "C" {

#ifndef TMK_DESKTOP_NOIMPL_MATRIX
void matrix_init() {
  matrix_.clear();
}

uint8_t matrix_scan() {
  return 0;
}

matrix_row_t matrix_get_row(uint8_t row) {
  return matrix_.value(row);
}
#endif

}  // extern "C"
}  // namespace tmk_desktop
