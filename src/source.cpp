/**
 * @file source.cpp
 * @brief Source
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <tmk_desktop/source.hpp>
#include <atomic>
#include <exception>
#include <thread>

#ifdef _WIN32
#include "win32/receiver.hpp"
#endif

namespace tmk_desktop {
namespace {
std::thread thread_;  ///< スレッド
std::atomic<bool> running_{false};  ///< スレッドが動作中かどうか
std::atomic<bool> stop_requested_{false};  ///< スレッドに対する停止要求
EventReceiver receiver_;  ///< OSから入力イベントを受け取るためのクラス
}  // namespace

bool start_source() {
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
        ScopedInit() {receiver_.enable();}
        ~ScopedInit() {receiver_.disable();}
      } _init{};

      while (!stop_requested_.load(std::memory_order_acquire)) {
        receiver_.poll();
        std::this_thread::yield();
      }
    } catch (std::exception& e) {
      on_source_error(e);
    }
  });

  return true;
}

bool stop_source() {
  // すでにスレッドが停止しているかを確認する
  if (!thread_.joinable()) return false;

  // すでにスレッドが実行終了しているかを確認する
  if (!running_.load(std::memory_order_acquire)) return false;

  // スレッドに停止要求を出す
  stop_requested_.store(true, std::memory_order_release);
  receiver_.notify();

  // スレッドが停止するのを待つ
  thread_.join();

  return true;
}

SourceStatus get_source_status() noexcept {
  if (running_.load(std::memory_order_acquire)) {
    if (stop_requested_.load(std::memory_order_acquire)) return SourceStatus::STOPPING;
    return SourceStatus::RUNNING;
  } else {
    if (thread_.joinable()) return SourceStatus::STOPPED;
    return SourceStatus::RESET;
  }
}
}  // namespace tmk_desktop
