/**
 * @file key.hpp
 * @brief キー
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <compare>
#include <type_traits>
#include <cstdlib>
#include <cstdint>

namespace tmk_desktop {
/**
 * @brief キーを格納するクラス
 */
class Key final {
public:
#if MATRIX_SIZE <= 0x100
  /**
   * @brief 値を表現する厳格な型
   */
  enum class EnumType : uint8_t {};
#elif MATRIX_SIZE <= 0x10000
  /**
   * @brief 値を表現する厳格な型
   */
  enum class EnumType : uint16_t {};
#else
  #error "MATRIX_SIZEが大きすぎる"
#endif

  /**
   * @brief 値を表現する型
   */
  using ValueType = std::underlying_type_t<EnumType>;

  static constexpr size_t COUNT = MATRIX_SIZE;  ///< 取り得る値の個数

  constexpr Key() noexcept = default;

  constexpr Key(EnumType value) noexcept : value_(value) {}

  constexpr explicit Key(ValueType value) noexcept : value_(static_cast<EnumType>(value)) {}

  /**
   * @brief 厳格な型へ暗黙的に変換する
   */
  constexpr operator EnumType() const noexcept {
    return value_;
  }

  /**
   * @brief 数値型へ明示的に変換する
   */
  constexpr explicit operator ValueType() const noexcept {
    return static_cast<ValueType>(value_);
  }

  constexpr auto operator <=>(const Key&) const noexcept = default;

  constexpr ValueType value() const noexcept {
    return static_cast<ValueType>(value_);
  }

private:
  EnumType value_{};
};

/**
 * @brief 未割当のキー
 */
static constexpr Key K_UNASSIGNED{Key::COUNT};
}  // namespace tmk_desktop
