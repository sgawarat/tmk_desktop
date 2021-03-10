/**
 * @file host_driver.hpp
 * @brief Win32のホストドライバ関数
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

extern "C" {
#include <tmk_core/common/host_driver.h>
}  // extern "C"

namespace tmk_desktop::win32 {
// uint8_t keyboard_leds() noexcept;

/**
 * @brief キー入力イベントをOSに送信する
 * 
 * @param r キー状態を格納するクラスへのポインタ
 */
void send_keyboard(report_keyboard_t* r) noexcept;

/**
 * @brief マウス入力イベントをOSに送信する
 * 
 * @param r マウス状態を格納するクラスへのポインタ
 */
void send_mouse(report_mouse_t* r) noexcept;

/**
 * @brief システム制御イベントをOSに送る
 * 
 * @param val Generic Desktop PageのUsage ID
 */
void send_system(uint16_t val) noexcept;

/**
 * @brief メディア制御イベントをOSに送る
 * 
 * @param val Comsumer PageのUsage ID
 */
void send_consumer(uint16_t val) noexcept;

/**
 * @brief キーリピートを表す入力イベントをOSに送信する
 */
void send_key_repeat() noexcept;

/**
 * @brief キーリピートが途切れたことを伝える
 */
void clear_key_repeat() noexcept;
}  // namespace tmk_desktop::win32
