/**
 * @file settings.hpp
 * @brief 設定
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <span>
#include <cstdlib>
#include <cstdint>
#include <tmk_desktop/key.hpp>
#include <tmk_desktop/action.hpp>
#include <tmk_desktop/macro.hpp>

namespace tmk_desktop::win32 {
/**
 * @brief グローバルな設定
 */
struct GlobalSettingsInfo {
  uint8_t layout_for_input_method = 0;  ///< Input Methodが有効であるときに使用されるレイアウト
};

/**
 * @brief キーの挙動
 */
enum class KeyBehavior : uint32_t {
  DEFAULT = 0,  ///< 通常のキー処理
  PRESS_ONLY = 1,  ///< pressイベントのみで押下を認識する
  RELEASE_ONLY = 2,  ///< releaseイベントのみで押下を認識する
  IGNORED = 3,  ///< キー入力を無視する
};
constexpr KeyBehavior KEY_BEHAVIOR_MAX = KeyBehavior::IGNORED;
constexpr size_t KEY_BEHAVIOR_COUNT = static_cast<size_t>(KEY_BEHAVIOR_MAX) + 1;

struct KeyBehaviorMapInfo {
  struct Entry {
    constexpr Entry() noexcept = default;

    constexpr Entry(KeyBehavior key_behavior, size_t key_count, const Key* keys) noexcept
      : key_behavior(key_behavior),
        key_count(key_count),
        keys(keys) {}

    constexpr Entry(KeyBehavior key_behavior, std::span<const Key> keys) noexcept
      : key_behavior(key_behavior),
        key_count(keys.size()),
        keys(keys.data()) {}

    KeyBehavior key_behavior = KeyBehavior::DEFAULT;
    size_t key_count = 0;
    const Key* keys = nullptr;
  };

  constexpr KeyBehaviorMapInfo() noexcept = default;

  constexpr KeyBehaviorMapInfo(std::span<const Entry> entries) noexcept
    : count(entries.size()),
      entries(entries.data()) {}

  size_t count = 0;
  const Entry* entries = nullptr;
};

/**
 * @brief アクションの情報
 */
struct ActionMapInfo {
  struct Entry {
    constexpr Entry() noexcept = default;

    constexpr Entry(uint8_t layer, size_t action_count, const action_t* actions) noexcept
      : layer(layer),
        action_count(action_count),
        actions(actions) {}

    constexpr Entry(uint8_t layer, std::span<const action_t> actions) noexcept
      : layer(layer),
        action_count(actions.size()),
        actions(actions.data()) {}

    uint8_t layer = 0;
    size_t action_count = 0;
    const action_t* actions = nullptr;
  };

  constexpr ActionMapInfo() noexcept = default;

  constexpr ActionMapInfo(std::span<const Entry> entries) noexcept
    : count(entries.size()),
      entries(entries.data()) {}

  size_t count = 0;
  const Entry* entries = nullptr;
};

/**
 * @brief マクロの情報
 */
struct MacroMapInfo {
  struct Entry {
    constexpr Entry() noexcept = default;

    constexpr Entry(uint8_t id, size_t macro_size, const macro_t* macro) noexcept
      : id(id),
        macro_size(macro_size),
        macro(macro) {}

    constexpr Entry(uint8_t id, std::span<const macro_t> macro) noexcept
      : id(id),
        macro_size(macro.size()),
        macro(macro.data()) {}

    uint8_t id = 0;
    size_t macro_size = 0;
    const macro_t* macro = nullptr;
  };

  constexpr MacroMapInfo() noexcept = default;

  constexpr MacroMapInfo(std::span<const Entry> entries) noexcept
    : count(entries.size()),
      entries(entries.data()) {}

  size_t count = 0;
  const Entry* entries = nullptr;
};

GlobalSettingsInfo get_global_settings_info() noexcept;

KeyBehaviorMapInfo get_key_behavior_map_info() noexcept;

ActionMapInfo get_action_map_info() noexcept;

MacroMapInfo get_macro_map_info() noexcept;
}  // namespace tmk_desktop::win32
