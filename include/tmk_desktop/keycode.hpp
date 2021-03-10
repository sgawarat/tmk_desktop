/**
 * @file keycode.hpp
 * @brief キーコード
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <type_traits>
#include <compare>
#include <cstdlib>
#include <cstdint>

extern "C" {
#include <tmk_core/common/keycode.h>
}  // extern "C"


namespace /* TMK_DESKTOP_*_KEYCODES */ {
// 0x00 - 0xa4
#define TMK_DESKTOP_KEYBOARD_KEYCODES(m) \
    m(NO) \
    m(ROLL_OVER) \
    m(POST_FAIL) \
    m(UNDEFINED) \
    m(A) \
    m(B) \
    m(C) \
    m(D) \
    m(E) \
    m(F) \
    m(G) \
    m(H) \
    m(I) \
    m(J) \
    m(K) \
    m(L) \
    m(M) \
    m(N) \
    m(O) \
    m(P) \
    m(Q) \
    m(R) \
    m(S) \
    m(T) \
    m(U) \
    m(V) \
    m(W) \
    m(X) \
    m(Y) \
    m(Z) \
    m(1) \
    m(2) \
    m(3) \
    m(4) \
    m(5) \
    m(6) \
    m(7) \
    m(8) \
    m(9) \
    m(0) \
    m(ENTER) \
    m(ESCAPE) \
    m(BSPACE) \
    m(TAB) \
    m(SPACE) \
    m(MINUS) \
    m(EQUAL) \
    m(LBRACKET) \
    m(RBRACKET) \
    m(BSLASH) \
    m(NONUS_HASH) \
    m(SCOLON) \
    m(QUOTE) \
    m(GRAVE) \
    m(COMMA) \
    m(DOT) \
    m(SLASH) \
    m(CAPSLOCK) \
    m(F1) \
    m(F2) \
    m(F3) \
    m(F4) \
    m(F5) \
    m(F6) \
    m(F7) \
    m(F8) \
    m(F9) \
    m(F10) \
    m(F11) \
    m(F12) \
    m(PSCREEN) \
    m(SCROLLLOCK) \
    m(PAUSE) \
    m(INSERT) \
    m(HOME) \
    m(PGUP) \
    m(DELETE) \
    m(END) \
    m(PGDOWN) \
    m(RIGHT) \
    m(LEFT) \
    m(DOWN) \
    m(UP) \
    m(NUMLOCK) \
    m(KP_SLASH) \
    m(KP_ASTERISK) \
    m(KP_MINUS) \
    m(KP_PLUS) \
    m(KP_ENTER) \
    m(KP_1) \
    m(KP_2) \
    m(KP_3) \
    m(KP_4) \
    m(KP_5) \
    m(KP_6) \
    m(KP_7) \
    m(KP_8) \
    m(KP_9) \
    m(KP_0) \
    m(KP_DOT) \
    m(NONUS_BSLASH) \
    m(APPLICATION) \
    m(POWER) \
    m(KP_EQUAL) \
    m(F13) \
    m(F14) \
    m(F15) \
    m(F16) \
    m(F17) \
    m(F18) \
    m(F19) \
    m(F20) \
    m(F21) \
    m(F22) \
    m(F23) \
    m(F24) \
    m(EXECUTE) \
    m(HELP) \
    m(MENU) \
    m(SELECT) \
    m(STOP) \
    m(AGAIN) \
    m(UNDO) \
    m(CUT) \
    m(COPY) \
    m(PASTE) \
    m(FIND) \
    m(_MUTE) \
    m(_VOLUP) \
    m(_VOLDOWN) \
    m(LOCKING_CAPS) \
    m(LOCKING_NUM) \
    m(LOCKING_SCROLL) \
    m(KP_COMMA) \
    m(KP_EQUAL_AS400) \
    m(INT1) \
    m(INT2) \
    m(INT3) \
    m(INT4) \
    m(INT5) \
    m(INT6) \
    m(INT7) \
    m(INT8) \
    m(INT9) \
    m(LANG1) \
    m(LANG2) \
    m(LANG3) \
    m(LANG4) \
    m(LANG5) \
    m(LANG6) \
    m(LANG7) \
    m(LANG8) \
    m(LANG9) \
    m(ALT_ERASE) \
    m(SYSREQ) \
    m(CANCEL) \
    m(CLEAR) \
    m(PRIOR) \
    m(RETURN) \
    m(SEPARATOR) \
    m(OUT) \
    m(OPER) \
    m(CLEAR_AGAIN) \
    m(CRSEL) \
    m(EXSEL)

// 0xa5 - 0xbc
#define TMK_DESKTOP_SPECIAL_KEYCODES(m) \
    m(SYSTEM_POWER) \
    m(SYSTEM_SLEEP) \
    m(SYSTEM_WAKE) \
    m(AUDIO_MUTE) \
    m(AUDIO_VOL_UP) \
    m(AUDIO_VOL_DOWN) \
    m(MEDIA_NEXT_TRACK) \
    m(MEDIA_PREV_TRACK) \
    m(MEDIA_FAST_FORWARD) \
    m(MEDIA_REWIND) \
    m(MEDIA_STOP) \
    m(MEDIA_PLAY_PAUSE) \
    m(MEDIA_EJECT) \
    m(MEDIA_SELECT) \
    m(MAIL) \
    m(CALCULATOR) \
    m(MY_COMPUTER) \
    m(WWW_SEARCH) \
    m(WWW_HOME) \
    m(WWW_BACK) \
    m(WWW_FORWARD) \
    m(WWW_STOP) \
    m(WWW_REFRESH) \
    m(WWW_FAVORITES)

// 0xbf - 0xdf
#define TMK_DESKTOP_INTERNAL_KEYCODES(m) \
    m(BOOTLOADER) \
    m(FN0) \
    m(FN1) \
    m(FN2) \
    m(FN3) \
    m(FN4) \
    m(FN5) \
    m(FN6) \
    m(FN7) \
    m(FN8) \
    m(FN9) \
    m(FN10) \
    m(FN11) \
    m(FN12) \
    m(FN13) \
    m(FN14) \
    m(FN15) \
    m(FN16) \
    m(FN17) \
    m(FN18) \
    m(FN19) \
    m(FN20) \
    m(FN21) \
    m(FN22) \
    m(FN23) \
    m(FN24) \
    m(FN25) \
    m(FN26) \
    m(FN27) \
    m(FN28) \
    m(FN29) \
    m(FN30) \
    m(FN31)

// 0xe0 - 0xe7
#define TMK_DESKTOP_MODIFIER_KEYCODES(m) \
    m(LCTRL) \
    m(LSHIFT) \
    m(LALT) \
    m(LGUI) \
    m(RCTRL) \
    m(RSHIFT) \
    m(RALT) \
    m(RGUI)

// 0xf0 - 0xff
#define TMK_DESKTOP_MOUSE_KEYCODES(m) \
    m(MS_UP) \
    m(MS_DOWN) \
    m(MS_LEFT) \
    m(MS_RIGHT) \
    m(MS_BTN1) \
    m(MS_BTN2) \
    m(MS_BTN3) \
    m(MS_BTN4) \
    m(MS_BTN5) \
    m(MS_WH_UP) \
    m(MS_WH_DOWN) \
    m(MS_WH_LEFT) \
    m(MS_WH_RIGHT) \
    m(MS_ACCEL0) \
    m(MS_ACCEL1) \
    m(MS_ACCEL2)
}  // namespace

namespace tmk_desktop {
/**
 * @brief キーコードを格納するクラス
 */
class Keycode final {
public:
  /**
   * @brief 値を表現する厳格な型
   */
  enum class EnumType : uint8_t {};

  /**
   * @brief 値を表現する整数型
   */
  using ValueType = std::underlying_type_t<EnumType>;

  static constexpr size_t COUNT = 256;  ///< キーコードが取り得る値の個数

  constexpr Keycode() noexcept = default;

  constexpr Keycode(hid_keyboard_keypad_usage keycode) noexcept
    : value_(static_cast<EnumType>(keycode)) {}

  constexpr Keycode(internal_special_keycodes keycode) noexcept
    : value_(static_cast<EnumType>(keycode)) {}

  constexpr Keycode(EnumType value) noexcept
    : value_(value) {}

  constexpr explicit Keycode(ValueType value) noexcept
    : value_(static_cast<EnumType>(value)) {}

  constexpr operator EnumType() const noexcept {
    return value_;
  }

  constexpr explicit operator ValueType() const noexcept {
    return static_cast<ValueType>(value_);
  }

  constexpr auto operator <=>(const Keycode&) const noexcept = default;

  constexpr ValueType value() const noexcept {
    return static_cast<ValueType>(value_);
  }

private:
  EnumType value_{};
};

static_assert(sizeof(Keycode) == 1);
}  // namespace tmk_desktop
