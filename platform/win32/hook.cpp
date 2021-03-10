/**
 * @file hook.cpp
 * @brief キーボードフック
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include "hook.hpp"

namespace tmk_desktop::win32 {
namespace {
HINSTANCE instance_ = NULL;  // DLLモジュールのハンドル
}  // namespace

HHOOK enable_keyboard_hook(HOOKPROC proc) noexcept {
  // instance_が指すDLLモジュールをグローバルフック（スレッド指定なし）で登録する
  return SetWindowsHookEx(WH_KEYBOARD_LL, proc, instance_, 0);
}

void disable_keyboard_hook(HHOOK hook) noexcept {
  UnhookWindowsHookEx(hook);
}

extern "C" {
BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, [[maybe_unused]] LPVOID reserved) {
  switch (reason) {
    case DLL_PROCESS_ATTACH: {
      instance_ = hmodule;
      break;
    }
    case DLL_PROCESS_DETACH: {
      break;
    }
    default:
      break;
  }
  return TRUE;
}
}  // exetrn "C"
}  // namespace tmk_desktop::win32
