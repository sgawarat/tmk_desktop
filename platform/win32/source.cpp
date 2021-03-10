/**
 * @file source.cpp
 * @brief Source
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include "source.hpp"
#include <atomic>
#include <exception>
#include <thread>
#include <Windows.h>
#include <tmk_desktop/bitset.hpp>
#include "sink.hpp"
#include "hook.hpp"
#include "injected.hpp"
#include "settings.hpp"
#include "logging.hpp"

namespace tmk_desktop::win32 {
namespace {
std::thread thread_;  ///< スレッド
std::atomic<bool> running_{false};  ///< スレッドが動作中かどうか
std::atomic<bool> stop_requested_{false};  ///< スレッドに対する停止要求
std::exception_ptr ep_ = nullptr;  ///< フックプロシージャで発生した例外
OnSinkError on_error_ = nullptr;  ///< エラー時に呼ばれる関数

/**
 * @brief フックプロシージャ
 * 
 * フック処理にはミリ秒単位の制限時間が設けられているので、重たい処理はフックの外で行う。
 */
LRESULT CALLBACK hook_proc(int code, WPARAM wparam, LPARAM lparam) noexcept {
  switch (code) {
    case HC_ACTION: {
      KBDLLHOOKSTRUCT* info_ptr = reinterpret_cast<KBDLLHOOKSTRUCT*>(lparam);
      if (!info_ptr) break;

      // ハードウェア由来でないキー入力を素通りさせる
      if (info_ptr->flags & (LLKHF_LOWER_IL_INJECTED | LLKHF_INJECTED)) break;

      // 自身に由来するキーイベントを素通りさせる
      if (remove_injected(*info_ptr)) break;

      // キー入力を奪ってエンジンに横流しする
      try {
        send_to_sink(*info_ptr);
      } catch (...) {
        ep_ = std::current_exception();
        PostQuitMessage(0);  // スレッドをメッセージループから脱出させる
      }
      return TRUE;
    }
  }

  // 素通りさせたキー入力を下流に流す
  return CallNextHookEx(NULL, code, wparam, lparam);
}
}  // namespace

bool start_source(OnSourceError on_error) {
  if (thread_.joinable()) return false;

  // 状態を初期化する
  stop_requested_.store(false, std::memory_order_release);
  ep_ = nullptr;
  on_error_ = on_error;

  thread_ = std::thread([] {
    running_.store(true, std::memory_order_release);
    const auto hook = enable_keyboard_hook(hook_proc);

    MSG msg{};
    while (!stop_requested_.load(std::memory_order_acquire)) {
      // フック処理はメッセージ処理の合間に行われるらしい
      if (GetMessage(&msg, NULL, 0, 0) == 0) break;
    }

    disable_keyboard_hook(hook);

    if (ep_ && on_error_) {
      try {
        std::rethrow_exception(ep_);
      } catch (...) {
        on_error_();
      }
    }

    running_.store(false, std::memory_order_release);
  });

  return true;
}

bool stop_source() {
  // すでにスレッドが停止しているかを確認する
  if (!thread_.joinable()) return false;

  // すでにスレッドに停止要求が出されているかを確認する
  if (stop_requested_.load(std::memory_order_acquire)) return false;

  // スレッドに停止要求を出す
  stop_requested_.store(true, std::memory_order_release);
  PostThreadMessage(GetThreadId(thread_.native_handle()), WM_QUIT, 0, 0);

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
}  // namespace tmk_desktop::win32
