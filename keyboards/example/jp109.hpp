/**
 * @file jp109.hpp
 * @brief 日本語配列
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <tmk_desktop/event.hpp>

extern "C" {
#include <common/keycode.h>
#include <common/action_code.h>
}  // extern "C"

#ifdef _WIN32

/* clang-format off */
/**
 * @brief 日本語配列にあるキー
 *
 * (キー名, 対応するキーコード名, キーの値)
 *
 * 基本的にはPS/2 Set1コードと等価だが、
 * - PauseキーがNumLockの値(45)に割り当てられる
 * - NumLockキーがExtendedキーになっている
 * - RShiftキーがExtendedキーになっている
 */
#define TMK_DESKTOP_JP109_KEYS(m) \
    m(ESCAPE, ESCAPE, 0x1) \
    m(1, 1, 0x2) \
    m(2, 2, 0x3) \
    m(3, 3, 0x4) \
    m(4, 4, 0x5) \
    m(5, 5, 0x6) \
    m(6, 6, 0x7) \
    m(7, 7, 0x8) \
    m(8, 8, 0x9) \
    m(9, 9, 0xa) \
    m(0, 0, 0xb) \
    m(MINUS, MINUS, 0xc) \
    m(CIRCUMFLEX, EQUAL, 0xd) \
    m(BSPACE, BSPACE, 0xe) \
    m(TAB, TAB, 0xf) \
    m(Q, Q, 0x10) \
    m(W, W, 0x11) \
    m(E, E, 0x12) \
    m(R, R, 0x13) \
    m(T, T, 0x14) \
    m(Y, Y, 0x15) \
    m(U, U, 0x16) \
    m(I, I, 0x17) \
    m(O, O, 0x18) \
    m(P, P, 0x19) \
    m(AT, LBRACKET, 0x1a) \
    m(LBRACKET, RBRACKET, 0x1b) \
    m(ENTER, ENTER, 0x1c) \
    m(LCTRL, LCTRL, 0x1d) \
    m(A, A, 0x1e) \
    m(S, S, 0x1f) \
    m(D, D, 0x20) \
    m(F, F, 0x21) \
    m(G, G, 0x22) \
    m(H, H, 0x23) \
    m(J, J, 0x24) \
    m(K, K, 0x25) \
    m(L, L, 0x26) \
    m(SCOLON, SCOLON, 0x27) \
    m(COLON, QUOTE, 0x28) \
    m(HANKAKU_ZENKAKU, GRAVE, 0x29) \
    m(LSHIFT, LSHIFT, 0x2a) \
    m(RBRACKET, BSLASH, 0x2b) \
    m(Z, Z, 0x2c) \
    m(X, X, 0x2d) \
    m(C, C, 0x2e) \
    m(V, V, 0x2f) \
    m(B, B, 0x30) \
    m(N, N, 0x31) \
    m(M, M, 0x32) \
    m(COMMA, COMMA, 0x33) \
    m(DOT, DOT, 0x34) \
    m(SLASH, SLASH, 0x35) \
    m(KP_ASTERISK, KP_ASTERISK, 0x37) \
    m(LALT, LALT, 0x38) \
    m(SPACE, SPACE, 0x39) \
    m(CAPSLOCK, CAPSLOCK, 0x3a) \
    m(F1, F1, 0x3b) \
    m(F2, F2, 0x3c) \
    m(F3, F3, 0x3d) \
    m(F4, F4, 0x3e) \
    m(F5, F5, 0x3f) \
    m(F6, F6, 0x40) \
    m(F7, F7, 0x41) \
    m(F8, F8, 0x42) \
    m(F9, F9, 0x43) \
    m(F10, F10, 0x44) \
    m(PAUSE, PAUSE, 0x45) \
    m(SCROLLLOCK, SCROLLLOCK, 0x46) \
    m(KP_7, KP_7, 0x47) \
    m(KP_8, KP_8, 0x48) \
    m(KP_9, KP_9, 0x49) \
    m(KP_MINUS, KP_MINUS, 0x4a) \
    m(KP_4, KP_4, 0x4b) \
    m(KP_5, KP_5, 0x4c) \
    m(KP_6, KP_6, 0x4d) \
    m(KP_PLUS, KP_PLUS, 0x4e) \
    m(KP_1, KP_1, 0x4f) \
    m(KP_2, KP_2, 0x50) \
    m(KP_3, KP_3, 0x51) \
    m(KP_0, KP_0, 0x52) \
    m(KP_DOT, KP_DOT, 0x53) \
    m(F11, F11, 0x57) \
    m(F12, F12, 0x58) \
    m(KATAKANA_HIRAGANA, INT2, 0x70) \
    m(BSLASH, INT1, 0x73) \
    m(HENKAN, INT4, 0x79) \
    m(MUHENKAN, INT5, 0x7b) \
    m(YEN, INT3, 0x7d) \
    m(MEDIA_PREV_TRACK, MEDIA_PREV_TRACK, 0x110) \
    m(MEDIA_NEXT_TRACK, MEDIA_NEXT_TRACK, 0x119) \
    m(KP_ENTER, KP_ENTER, 0x11c) \
    m(RCTRL, RCTRL, 0x11d) \
    m(AUDIO_MUTE, AUDIO_MUTE, 0x120) \
    m(MEDIA_PLAY_PAUSE, MEDIA_PLAY_PAUSE, 0x122) \
    m(AUDIO_VOL_DOWN, AUDIO_VOL_DOWN, 0x12e) \
    m(AUDIO_VOL_UP, AUDIO_VOL_UP, 0x130) \
    m(KP_SLASH, KP_SLASH, 0x135) \
    m(RSHIFT, RSHIFT, 0x136) \
    m(PSCREEN, PSCREEN, 0x137) \
    m(RALT, RALT, 0x138) \
    m(NUMLOCK, NUMLOCK, 0x145) \
    m(HOME, HOME, 0x147) \
    m(UP, UP, 0x148) \
    m(PGUP, PGUP, 0x149) \
    m(LEFT, LEFT, 0x14b) \
    m(RIGHT, RIGHT, 0x14d) \
    m(END, END, 0x14f) \
    m(DOWN, DOWN, 0x150) \
    m(PGDOWN, PGDOWN, 0x151) \
    m(INSERT, INSERT, 0x152) \
    m(DELETE, DELETE, 0x153) \
    m(LGUI, LGUI, 0x15b) \
    m(RGUI, RGUI, 0x15c) \
    m(APPLICATION, APPLICATION, 0x15d)
/* clang-format on */
#endif

/**
 * @brief 日本語配列にあるキーのシフト変化
 *
 * (キー名, シフトありで入力されるキー名)
 */
/* clang-format off */
#define TMK_DESKTOP_JP109_SHIFTED_KEYS(m) \
    m(1, EXCLAIM) \
    m(2, DQUOTE) \
    m(3, HASH) \
    m(4, DOLLAR) \
    m(5, PERCENT) \
    m(6, AMPERSAND) \
    m(7, QUOTE) \
    m(8, LPAREN) \
    m(9, RPAREN) \
    m(MINUS, EQUAL) \
    m(CIRCUMFLEX, TILDE) \
    m(YEN, PIPE) \
    m(AT, GRAVE) \
    m(LBRACKET, LBRACE) \
    m(SCOLON, PLUS) \
    m(COLON, ASTERISK) \
    m(RBRACKET, RBRACE) \
    m(COMMA, LT) \
    m(DOT, GT) \
    m(SLASH, QUESTION) \
    m(BSLASH, UNDERSCORE)
/* clang-format on */

namespace tmk_desktop::jp109 {
static constexpr action_t AC_NO = ACTION_NO;
static constexpr action_t AC_TRANSPARENT = ACTION_TRANSPARENT;

#define M(name, keycode, value) \
  static constexpr Key K_##name = value; \
  static constexpr auto KC_##name = ::KC_##keycode; \
  static constexpr action_t AC_##name = ACTION_KEY(::KC_##keycode); \
  static constexpr action_t AC_SHIFT_##name = ACTION_MODS_KEY(MOD_LSFT, ::KC_##keycode);
TMK_DESKTOP_JP109_KEYS(M)
#undef M

#define M(name, shifted_name) static constexpr action_t AC_##shifted_name = ACTION_MODS_KEY(MOD_LSFT, KC_##name);
TMK_DESKTOP_JP109_SHIFTED_KEYS(M)
#undef M
}  // namespace tmk_desktop::jp109
