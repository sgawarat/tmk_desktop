/**
 * @file scancode.cpp
 * @brief PS/2 Set1 スキャンコード
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */

#include <tmk_desktop/win32/settings.hpp>

extern "C" {
#include <common/keycode.h>
}  // extern "C"

/* clang-format off */
/**
 * @brief キーコードに対応するPS/2 Set1のコード一覧
 */
#define TMK_DESKTOP_WIN32_PS2_SET1_SCANCODES(m) \
    m(ESCAPE, 0x0001) \
    m(1, 0x0002) \
    m(2, 0x0003) \
    m(3, 0x0004) \
    m(4, 0x0005) \
    m(5, 0x0006) \
    m(6, 0x0007) \
    m(7, 0x0008) \
    m(8, 0x0009) \
    m(9, 0x000a) \
    m(0, 0x000b) \
    m(MINUS, 0x000c) \
    m(EQUAL, 0x000d) \
    m(BSPACE, 0x000e) \
    m(TAB, 0x000f) \
    m(Q, 0x0010) \
    m(W, 0x0011) \
    m(E, 0x0012) \
    m(R, 0x0013) \
    m(T, 0x0014) \
    m(Y, 0x0015) \
    m(U, 0x0016) \
    m(I, 0x0017) \
    m(O, 0x0018) \
    m(P, 0x0019) \
    m(LBRACKET, 0x001a) \
    m(RBRACKET, 0x001b) \
    m(ENTER, 0x001c) \
    m(LCTRL, 0x001d) \
    m(A, 0x001e) \
    m(S, 0x001f) \
    m(D, 0x0020) \
    m(F, 0x0021) \
    m(G, 0x0022) \
    m(H, 0x0023) \
    m(J, 0x0024) \
    m(K, 0x0025) \
    m(L, 0x0026) \
    m(SCOLON, 0x0027) \
    m(QUOTE, 0x0028) \
    m(GRAVE, 0x0029) \
    m(LSHIFT, 0x002a) \
    m(BSLASH, 0x002b) \
    m(Z, 0x002c) \
    m(X, 0x002d) \
    m(C, 0x002e) \
    m(V, 0x002f) \
    m(B, 0x0030) \
    m(N, 0x0031) \
    m(M, 0x0032) \
    m(COMMA, 0x0033) \
    m(DOT, 0x0034) \
    m(SLASH, 0x0035) \
    m(RSHIFT, 0x0036) \
    m(KP_ASTERISK, 0x0037) \
    m(LALT, 0x0038) \
    m(SPACE, 0x0039) \
    m(CAPSLOCK, 0x003a) \
    m(F1, 0x003b) \
    m(F2, 0x003c) \
    m(F3, 0x003d) \
    m(F4, 0x003e) \
    m(F5, 0x003f) \
    m(F6, 0x0040) \
    m(F7, 0x0041) \
    m(F8, 0x0042) \
    m(F9, 0x0043) \
    m(F10, 0x0044) \
    m(NUMLOCK, 0x0045) \
    m(SCROLLLOCK, 0x0046) \
    m(KP_7, 0x0047) \
    m(KP_8, 0x0048) \
    m(KP_9, 0x0049) \
    m(KP_MINUS, 0x004a) \
    m(KP_4, 0x004b) \
    m(KP_5, 0x004c) \
    m(KP_6, 0x004d) \
    m(KP_PLUS, 0x004e) \
    m(KP_1, 0x004f) \
    m(KP_2, 0x0050) \
    m(KP_3, 0x0051) \
    m(KP_0, 0x0052) \
    m(KP_DOT, 0x0053) \
    m(NONUS_BSLASH, 0x0056) \
    m(F11, 0x0057) \
    m(F12, 0x0058) \
    m(KP_EQUAL, 0x0059) \
    m(INT6, 0x005c) \
    m(F13, 0x0064) \
    m(F14, 0x0065) \
    m(F15, 0x0066) \
    m(F16, 0x0067) \
    m(F17, 0x0068) \
    m(F18, 0x0069) \
    m(F19, 0x006a) \
    m(F20, 0x006b) \
    m(F21, 0x006c) \
    m(F22, 0x006d) \
    m(F23, 0x006e) \
    m(INT2, 0x0070) \
    m(INT1, 0x0073) \
    m(F24, 0x0076) \
    m(LANG5, 0x0076) \
    m(LANG4, 0x0077) \
    m(LANG3, 0x0078) \
    m(INT4, 0x0079) \
    m(INT5, 0x007b) \
    m(INT3, 0x007d) \
    m(KP_COMMA, 0x007e) \
    m(LANG2, 0x00f1) \
    m(LANG1, 0x00f2) \
    m(POST_FAIL, 0x00fc) \
    m(ROLL_OVER, 0x00ff) \
    m(MEDIA_PREV_TRACK, 0xe010) \
    m(MEDIA_NEXT_TRACK, 0xe019) \
    m(KP_ENTER, 0xe01c) \
    m(RCTRL, 0xe01d) \
    m(AUDIO_MUTE, 0xe020) \
    m(CALCULATOR, 0xe021) \
    m(MEDIA_PLAY_PAUSE, 0xe022) \
    m(MEDIA_STOP, 0xe024) \
    m(AUDIO_VOL_DOWN, 0xe02e) \
    m(AUDIO_VOL_UP, 0xe030) \
    m(WWW_HOME, 0xe032) \
    m(KP_SLASH, 0xe035) \
    m(PSCREEN, 0xe037) \
    m(RALT, 0xe038) \
    m(HOME, 0xe047) \
    m(UP, 0xe048) \
    m(PGUP, 0xe049) \
    m(LEFT, 0xe04b) \
    m(RIGHT, 0xe04d) \
    m(END, 0xe04f) \
    m(DOWN, 0xe050) \
    m(PGDOWN, 0xe051) \
    m(INSERT, 0xe052) \
    m(DELETE, 0xe053) \
    m(LGUI, 0xe05b) \
    m(RGUI, 0xe05c) \
    m(APPLICATION, 0xe05d) \
    m(SYSTEM_POWER, 0xe05e) \
    m(SYSTEM_SLEEP, 0xe05f) \
    m(SYSTEM_WAKE, 0xe063) \
    m(WWW_SEARCH, 0xe065) \
    m(WWW_FAVORITES, 0xe066) \
    m(WWW_REFRESH, 0xe067) \
    m(WWW_STOP, 0xe068) \
    m(WWW_FORWARD, 0xe069) \
    m(WWW_BACK, 0xe06a) \
    m(MY_COMPUTER, 0xe06b) \
    m(MAIL, 0xe06c) \
    m(MEDIA_SELECT, 0xe06d) \
    m(PAUSE, 0xe11d)
    // m(POWER, 0xe05e)
/* clang-format on */

namespace tmk_desktop::inline win32 {
#ifndef TMK_DESKTOP_NOIMPL_KEYCODE_TO_SCANCODE_TABLE
const KeycodeToScancodeTable keycode_to_scancode_table = []() {
  KeycodeToScancodeTable t{};
#define M(name, value) t[KC_##name] = value;
  TMK_DESKTOP_WIN32_PS2_SET1_SCANCODES(M)
#undef M
  return t;
}();
#endif
}  // namespace tmk_desktop::inline win32
