/**
 * @file action_map.hpp
 * @brief アクションマップ
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <array>
#include <span>
#include "key.hpp"
#include "action.hpp"

namespace tmk_desktop {
class ActionMap {
public:
  constexpr ActionMap() noexcept : actions_{} {}

  /**
   * @brief 指定のキーに対してアクションを設定する
   */
  constexpr ActionMap& set(Key key, action_t action) noexcept {
    if (key.value() < actions_.size()) actions_[key.value()] = action;
    return *this;
  }

  /**
   * @brief すべてのキーに対して個別のアクションを設定する
   * 
   * @param fn キーを引数とする関数オブジェクト
   */
  template <typename Fn>
  constexpr ActionMap& update(Fn fn) noexcept {
    Key::ValueType i = 0;
    for (auto& action : actions_) {
      action = fn(Key{i++});
    }
    return *this;
  }

  /**
   * @brief 指定のキー列に対して個別のアクションを設定する
   * 
   * @param keys キー列
   * @param fn キー列中のインデックスとキーを引数とする関数オブジェクト
   */
  template <typename Fn>
  constexpr ActionMap& update(std::span<const Key> keys, Fn fn) noexcept {
    size_t i = 0;
    for (const auto& key : keys) {
      if (key.value() < actions_.size()) actions_[key.value()] = fn(i++, key);
    }
    return *this;
  }

  constexpr auto begin() const noexcept {
    return actions_.begin();
  }

  constexpr auto end() const noexcept {
    return actions_.end();
  }

  constexpr const action_t* data() const noexcept {
    return actions_.data();
  }

  constexpr size_t size() const noexcept {
    return actions_.size();
  }

private:
  std::array<action_t, Key::COUNT> actions_;
};
}  // namespace tmk_desktop
