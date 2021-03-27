/**
 * @file utility.hpp
 * @brief ユーティリティ
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <Windows.h>
#include <shellapi.h>

namespace tmk_desktop::inline win32 {
/**
 * @brief 通知領域にアイコンを登録する
 *
 * @param wnd ウィンドウのハンドル
 * @param id 識別番号
 * @param callback_message ウィンドウプロシージャに送られるメッセージのID
 * @param icon アイコンのハンドル
 * @param title アプリケーションの名前
 * @retval true 登録に成功した
 * @retval false 登録に失敗した
 */
inline bool add_notify_icon(HWND wnd, UINT id, UINT callback_message, HICON icon, LPCWSTR title = nullptr) noexcept {
  NOTIFYICONDATA nid{
      .cbSize = sizeof(NOTIFYICONDATA),
      .hWnd = wnd,
      .uID = id,
      .uFlags = (UINT)NIF_MESSAGE | NIF_SHOWTIP | (icon ? NIF_ICON : 0) | (title ? NIF_TIP : 0),
      .uCallbackMessage = callback_message,
      .hIcon = icon,
      .szTip = {},
  };
  if (title) lstrcpyn(nid.szTip, title, sizeof(nid.szTip) / sizeof(*nid.szTip));

  // 高負荷などが原因で登録に失敗することがあるので何回か試みる
  bool result = false;
  for (size_t i = 0; i < 5; ++i) {
    if (Shell_NotifyIcon(NIM_ADD, &nid)) {
      result = true;
      break;
    }

    // タイムアウトした場合、応答が返ってこなかっただけかもしれないので、少し待ってから上手くいっていたかどうかを確認する
    if (GetLastError() == ERROR_TIMEOUT) {
      Sleep(100);
      if (Shell_NotifyIcon(NIM_MODIFY, &nid)) {
        result = true;
        break;
      }
    }
  }
  if (!result) return false;

  // 新しいバージョンを用いる
  nid.uVersion = NOTIFYICON_VERSION_4;
  if (!Shell_NotifyIcon(NIM_SETVERSION, &nid)) {
    Shell_NotifyIcon(NIM_DELETE, &nid);
    return false;
  }

  return true;
}

/**
 * @brief 通知領域からアイコンを取り除く
 *
 * @param wnd ウィンドウのハンドル
 * @param id 識別番号
 */
inline void remove_notify_icon(HWND wnd, UINT id) noexcept {
  NOTIFYICONDATA nid{
      .cbSize = sizeof(NOTIFYICONDATA),
      .hWnd = wnd,
      .uID = id,
  };
  Shell_NotifyIcon(NIM_DELETE, &nid);
}

/**
 * @brief IMEの状態を取得する
 *
 * かなり古い仕様を無理矢理に使っているので、今後とも上手く動いてくれるかは分からない。
 */
inline bool get_im_status() noexcept {
  GUITHREADINFO gui_thread_info{
      .cbSize = sizeof(GUITHREADINFO),
  };
  GetGUIThreadInfo(0, &gui_thread_info);
  const HWND hwnd = ImmGetDefaultIMEWnd(gui_thread_info.hwndFocus);
  return SendMessage(hwnd, WM_IME_CONTROL, 5 /*IMC_GETOPENSTATUS*/, 0) != 0;
}
}  // namespace tmk_desktop::inline win32
