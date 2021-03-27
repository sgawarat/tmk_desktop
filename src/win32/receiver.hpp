/**
 * @file receiver.cpp
 * @brief Win32の入力イベントを受け取るやつ
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <exception>
#include <Windows.h>
#include <tmk_desktop/source.hpp>
#include <tmk_desktop/keyboard.hpp>
#include "injected.hpp"

namespace tmk_desktop::inline win32 {
class EventReceiver final {
public:
  /**
   * @brief 有効化
   */
  void enable() noexcept {
    thread_id_ = GetCurrentThreadId();
    hook_ = SetWindowsHookEx(WH_KEYBOARD_LL, hook_proc, GetModuleHandle(NULL), 0);
  }

  /**
   * @brief 無効化
   */
  void disable() noexcept {
    if (hook_) {
      UnhookWindowsHookEx(hook_);
      thread_id_ = 0;
      hook_ = NULL;
    }
  }

  /**
   * @brief イベントを受け取って処理する
   *
   * notify()を受けたり異常停止したりしない限りリターンされない。
   */
  void poll() noexcept {
    MSG msg;
    GetMessage(&msg, NULL, 0, 0);
  }

  /**
   * @brief pollを抜けるよう通知する
   */
  void notify() noexcept {
    if (thread_id_ > 0) PostThreadMessage(thread_id_, WM_NULL, 0, 0);
  }

private:
  /**
   * @brief フックプロシージャ
   *
   * @note フック処理にはミリ秒単位の制限時間が設けられているので、重たい処理はフックの外で行う。
   */
  static LRESULT CALLBACK hook_proc(int code, WPARAM wparam, LPARAM lparam) noexcept {
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
          send_to_keyboard(*info_ptr);
        } catch (std::exception& e) {
          on_source_error(e);
        }
        return TRUE;
      }
    }

    // 素通りさせたキー入力を下流に流す
    return CallNextHookEx(NULL, code, wparam, lparam);
  }

  HHOOK hook_ = NULL;    ///< フックのハンドル
  DWORD thread_id_ = 0;  ///< スレッドID
};
}  // namespace tmk_desktop::inline win32
