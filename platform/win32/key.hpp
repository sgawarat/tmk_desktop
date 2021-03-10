/**
 * @file key.hpp
 * @brief Win32でのキー
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <string_view>
#include <Windows.h>
#include <tmk_desktop/key.hpp>
#include <tmk_desktop/keycode.hpp>
#include <tmk_desktop/action.hpp>
#include "logging.hpp"

namespace {
#define TMK_DESKTOP_WIN32_ALL_KEYS(m) \
    m(ESCAPE, 0x001) \
    m(1, 0x002) \
    m(2, 0x003) \
    m(3, 0x004) \
    m(4, 0x005) \
    m(5, 0x006) \
    m(6, 0x007) \
    m(7, 0x008) \
    m(8, 0x009) \
    m(9, 0x00a) \
    m(0, 0x00b) \
    m(MINUS, 0x00c) \
    m(EQUAL, 0x00d) \
    m(BSPACE, 0x00e) \
    m(TAB, 0x00f) \
    m(Q, 0x010) \
    m(W, 0x011) \
    m(E, 0x012) \
    m(R, 0x013) \
    m(T, 0x014) \
    m(Y, 0x015) \
    m(U, 0x016) \
    m(I, 0x017) \
    m(O, 0x018) \
    m(P, 0x019) \
    m(LBRACKET, 0x01a) \
    m(RBRACKET, 0x01b) \
    m(ENTER, 0x01c) \
    m(LCTRL, 0x01d) \
    m(A, 0x01e) \
    m(S, 0x01f) \
    m(D, 0x020) \
    m(F, 0x021) \
    m(G, 0x022) \
    m(H, 0x023) \
    m(J, 0x024) \
    m(K, 0x025) \
    m(L, 0x026) \
    m(SCOLON, 0x027) \
    m(QUOTE, 0x028) \
    m(GRAVE, 0x029) \
    m(LSHIFT, 0x02a) \
    m(BSLASH, 0x02b) \
    m(Z, 0x02c) \
    m(X, 0x02d) \
    m(C, 0x02e) \
    m(V, 0x02f) \
    m(B, 0x030) \
    m(N, 0x031) \
    m(M, 0x032) \
    m(COMMA, 0x033) \
    m(DOT, 0x034) \
    m(SLASH, 0x035) \
    m(KP_ASTERISK, 0x037) \
    m(LALT, 0x038) \
    m(SPACE, 0x039) \
    m(CAPSLOCK, 0x03a) \
    m(F1, 0x03b) \
    m(F2, 0x03c) \
    m(F3, 0x03d) \
    m(F4, 0x03e) \
    m(F5, 0x03f) \
    m(F6, 0x040) \
    m(F7, 0x041) \
    m(F8, 0x042) \
    m(F9, 0x043) \
    m(F10, 0x044) \
    m(PAUSE, 0x045) \
    m(SCROLLLOCK, 0x046) \
    m(KP_7, 0x047) \
    m(KP_8, 0x048) \
    m(KP_9, 0x049) \
    m(KP_MINUS, 0x04a) \
    m(KP_4, 0x04b) \
    m(KP_5, 0x04c) \
    m(KP_6, 0x04d) \
    m(KP_PLUS, 0x04e) \
    m(KP_1, 0x04f) \
    m(KP_2, 0x050) \
    m(KP_3, 0x051) \
    m(KP_0, 0x052) \
    m(KP_DOT, 0x053) \
    m(NONUS_BSLASH, 0x056) \
    m(F11, 0x057) \
    m(F12, 0x058) \
    m(KP_EQUAL, 0x059) \
    m(INT6, 0x05c) \
    m(F13, 0x064) \
    m(F14, 0x065) \
    m(F15, 0x066) \
    m(F16, 0x067) \
    m(F17, 0x068) \
    m(F18, 0x069) \
    m(F19, 0x06a) \
    m(F20, 0x06b) \
    m(F21, 0x06c) \
    m(F22, 0x06d) \
    m(F23, 0x06e) \
    m(INT2, 0x070) \
    m(INT1, 0x073) \
    m(LANG5, 0x076) \
    m(LANG4, 0x077) \
    m(LANG3, 0x078) \
    m(INT4, 0x079) \
    m(INT5, 0x07b) \
    m(INT3, 0x07d) \
    m(KP_COMMA, 0x07e) \
    m(LANG2, 0x0f1) \
    m(LANG1, 0x0f2) \
    m(POST_FAIL, 0x0fc) \
    m(ROLL_OVER, 0x0ff) \
    m(MEDIA_PREV_TRACK, 0x110) \
    m(MEDIA_NEXT_TRACK, 0x119) \
    m(KP_ENTER, 0x11c) \
    m(RCTRL, 0x11d) \
    m(AUDIO_MUTE, 0x120) \
    m(CALCULATOR, 0x121) \
    m(MEDIA_PLAY_PAUSE, 0x122) \
    m(MEDIA_STOP, 0x124) \
    m(AUDIO_VOL_DOWN, 0x12e) \
    m(AUDIO_VOL_UP, 0x130) \
    m(WWW_HOME, 0x132) \
    m(KP_SLASH, 0x135) \
    m(RSHIFT, 0x136) \
    m(PSCREEN, 0x137) \
    m(RALT, 0x138) \
    m(NUMLOCK, 0x145) \
    m(HOME, 0x147) \
    m(UP, 0x148) \
    m(PGUP, 0x149) \
    m(LEFT, 0x14b) \
    m(RIGHT, 0x14d) \
    m(END, 0x14f) \
    m(DOWN, 0x150) \
    m(PGDOWN, 0x151) \
    m(INSERT, 0x152) \
    m(DELETE, 0x153) \
    m(LGUI, 0x15b) \
    m(RGUI, 0x15c) \
    m(APPLICATION, 0x15d) \
    m(SYSTEM_POWER, 0x15e) \
    m(SYSTEM_SLEEP, 0x15f) \
    m(SYSTEM_WAKE, 0x163) \
    m(WWW_SEARCH, 0x165) \
    m(WWW_FAVORITES, 0x166) \
    m(WWW_REFRESH, 0x167) \
    m(WWW_STOP, 0x168) \
    m(WWW_FORWARD, 0x169) \
    m(WWW_BACK, 0x16a) \
    m(MY_COMPUTER, 0x16b) \
    m(MAIL, 0x16c) \
    m(MEDIA_SELECT, 0x16d)

#define TMK_DESKTOP_WIN32_KEY_ALIASES(m) \
    m(F24, 0x076) \
    m(BREAK, 0x146) \
    m(POWER, 0x15e)
}  // namespace

namespace tmk_desktop::win32 {
static constexpr uint16_t EXTENDED_KEY_BIT = 0x100;  ///< EXTENDEDなキーを表すビット

#define M(name, value) constexpr Key K_##name{value};
TMK_DESKTOP_WIN32_ALL_KEYS(M)
TMK_DESKTOP_WIN32_KEY_ALIASES(M)
#undef M

/**
 * @brief Win32の入力イベントをキー表現に変換する
 * 
 * @param sc 入力イベントのスキャンコード
 * @param extended EXTENDEDフラグの状態
 */
constexpr Key make_key(WORD sc, bool extended) noexcept {
  if (sc >= 256) return K_UNASSIGNED;
  if (extended) return Key(sc | EXTENDED_KEY_BIT);
  return Key(sc);
}

/**
 * @brief キーと等価のキーコードに変換する
 */
Keycode to_keycode(Key key) noexcept;

/**
 * @brief キーと等価のアクションに変換する
 */
inline action_t to_action(Key key, uint16_t mods = 0) noexcept {
  const uint16_t keycode = to_keycode(key).value();
  return { .code = static_cast<uint16_t>(mods | keycode)};
}

/**
 * @brief キーを文字列にする
 */
std::string_view make_string(Key key) noexcept;
} // namespace tmk_desktop::win32

#ifdef TMK_DESKTOP_ENABLE_LOGGING
namespace tmk_desktop {
inline std::ostream& operator <<(std::ostream& os, const Key& key) {
  if (sizeof(Key) <= 1) {
    os << win32::format("{} ({:#01x})", win32::make_string(key), key.value());
  } else if (sizeof(Key) <= 2) {
    os << win32::format("{} ({:#02x})", win32::make_string(key), key.value());
  } else if (sizeof(Key) <= 4) {
    os << win32::format("{} ({:#04x})", win32::make_string(key), key.value());
  } else {
    os << win32::format("{} ({:#0x})", win32::make_string(key), key.value());
  }
  return os;
}
}  // namespace tmk_desktop
#endif
