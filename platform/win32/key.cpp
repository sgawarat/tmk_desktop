/**
 * @file key.cpp
 * @brief Win32でのキー
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include "key.hpp"

namespace tmk_desktop::win32 {
Keycode to_keycode(Key key) noexcept {
  switch (key.value()) {
    #define M(name, value) case value: return KC_##name;
    TMK_DESKTOP_WIN32_ALL_KEYS(M)
    #undef M
  }
  return KC_NO;
}

std::string_view make_string(Key key) noexcept {
  switch (key.value()) {
    #define M(name, value) case value: return #name;
    TMK_DESKTOP_WIN32_ALL_KEYS(M)
    #undef M
  }
  return "NONAME";
}
}  // namespace tmk_desktop::win32
