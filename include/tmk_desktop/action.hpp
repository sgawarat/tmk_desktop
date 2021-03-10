/**
 * @file action.hpp
 * @brief アクション
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <array>
#include <cstdint>
#include "keycode.hpp"

extern "C" {
#include <tmk_core/common/action_code.h>
}  // extern "C"

namespace /* TMK_DESKTOP_*_ACTIONS */ {
#define TMK_DESKTOP_SHIFT_ACTIONS(m) \
    m(1, EXCLAMATION) \
    m(2, AT_SIGN) \
    m(3, HASH) \
    m(4, DOLLAR) \
    m(5, PERCENT) \
    m(6, CIRCUMFLEX) \
    m(7, AMPERSAND) \
    m(8, ASTERISK) \
    m(9, LPAREN) \
    m(0, RPAREN) \
    m(MINUS, UNDERSCORE) \
    m(EQUAL, PLUS) \
    m(LBRACKET, LBRACE) \
    m(RBRACKET, RBRACE) \
    m(BSLASH, VERTICAL_BAR) \
    m(NONUS_HASH, NONUS_TILDE) \
    m(SCOLON, COLON) \
    m(QUOTE, DOUBLE_QUOTE) \
    m(GRAVE, TILDE) \
    m(COMMA, LESS_THAN) \
    m(DOT, GREATER_THAN) \
    m(SLASH, QUESTION) \
    m(NONUS_BSLASH, NONUS_VERTICAL_BAR)
}  // namespace

namespace tmk_desktop {
// 下位レイヤーのアクションを参照するアクション
constexpr action_t AC_TRANSPARENT = ACTION_TRANSPARENT;

// シフトなし
#define M(name) constexpr action_t AC_##name = ACTION_KEY(KC_##name);
TMK_DESKTOP_KEYBOARD_KEYCODES(M)
TMK_DESKTOP_SPECIAL_KEYCODES(M)
TMK_DESKTOP_MODIFIER_KEYCODES(M)
#undef M

// シフトあり
#define M(name) constexpr action_t AC_SHIFT_##name = ACTION_MODS_KEY(MOD_LSFT, KC_##name);
TMK_DESKTOP_KEYBOARD_KEYCODES(M)
TMK_DESKTOP_SPECIAL_KEYCODES(M)
#undef M

// シフトあり記号
#define M(name, shift_name) constexpr action_t AC_##shift_name = ACTION_MODS_KEY(MOD_LSFT, KC_##name);
TMK_DESKTOP_SHIFT_ACTIONS(M)
#undef M

// 右側の修飾キーを示すビット
constexpr uint16_t MOD_RIGHT_BIT = 0x1000;
}  // namespace tmk_desktop
