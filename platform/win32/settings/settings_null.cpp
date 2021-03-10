/**
 * @file settings_null.cpp
 * @brief 何も出力しないキー配列
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include "../settings.hpp"

namespace tmk_desktop::win32 {
GlobalSettingsInfo get_global_settings_info() noexcept {
  return {};
}

KeyBehaviorMapInfo get_key_behavior_map_info() {
  return {};
}

ActionMapInfo get_action_map_info() {
  return {};
}

MacroMapInfo get_macro_map_info() {
  return {};
}
}  // namespace tmk_desktop::win32
