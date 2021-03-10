/**
 * @file hook.hpp
 * @brief キーボードフック
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <Windows.h>

#ifndef TMK_DESKTOP_WIN32_HOOK_EXPORTS
#define TMK_DESKTOP_WIN32_HOOK_API __declspec(dllimport)
#else
#define TMK_DESKTOP_WIN32_HOOK_API __declspec(dllexport)
#endif

namespace tmk_desktop::win32 {
/**
 * @brief キーボードフックを有効化する
 * 
 * @param proc キー入力を受け取るコールバック関数
 * @return フックのハンドル
 */
TMK_DESKTOP_WIN32_HOOK_API HHOOK enable_keyboard_hook(HOOKPROC proc) noexcept;

/**
 * @brief キーボードフックを無効化する
 * 
 * @param hook 無効化するフックのハンドル
 */
TMK_DESKTOP_WIN32_HOOK_API void disable_keyboard_hook(HHOOK hook) noexcept;
}  // namespace tmk_desktop::win32
