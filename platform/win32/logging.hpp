/**
 * @file logging.hpp
 * @brief ロギング
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#ifdef TMK_DESKTOP_ENABLE_LOGGING

#include <iostream>
#include <string_view>
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace tmk_desktop::win32 {
using fmt::format;

template <typename... Args>
void debug_log(std::string_view fmt, Args&&... args) {
  std::cout << format(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void error_log(std::string_view fmt, Args&&... args) {
  std::cout << format(fmt, std::forward<Args>(args)...);
}
}  // namespace tmk_desktop::win32

#else

#define debug_log(...) (void)0;
#define error_log(...) (void)0;

#endif
