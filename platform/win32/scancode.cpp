/**
 * @file scancode.cpp
 * @brief スキャンコード
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include "scancode.hpp"
#include <tmk_desktop/keycode.hpp>

namespace tmk_desktop::win32 {
uint16_t make_scancode(Keycode keycode) noexcept {
  switch (keycode.value()) {
    #define M(name, value) case KC_##name: return value;
    TMK_DESKTOP_WIN32_ALL_SCANCODES(M)
    TMK_DESKTOP_WIN32_SCANCODE_ALIASES(M)
    #undef M
  }
  return 0;
}

std::string_view get_scancode_string(uint16_t scancode) noexcept {
  switch (scancode) {
    #define M(name, value) case value: return #name;
    TMK_DESKTOP_WIN32_ALL_SCANCODES(M)
    #undef M
  }
  return "NONAME";
}
}  // namespace tmk_desktop::win32
