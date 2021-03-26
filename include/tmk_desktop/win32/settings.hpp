/**
 * @file settings.hpp
 * @brief 設定を注入するためのインターフェイス
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <array>
#include "event.hpp"

extern "C" {
#include <common/keyboard.h>
}  // extern "C"

namespace tmk_desktop::inline win32 {
/**
 * @brief キーからkeypos_tへの変換表
 */
using KeyToKeyposTable = std::array<keypos_t, KEY_COUNT>;
extern const KeyToKeyposTable key_to_keypos_table;

/**
 * @brief キーを押すと同時に離すと解釈するかどうかのフラグ
 */
using TappingKeyTable = std::array<bool, KEY_COUNT>;
extern const TappingKeyTable tapping_key_table;

/**
 * @brief キーコードからスキャンコードへの変換表
 */
using KeycodeToScancodeTable = std::array<uint16_t, 256>;
extern const KeycodeToScancodeTable keycode_to_scancode_table;
}  // namespace tmk_desktop::inline win32
