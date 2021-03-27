/**
 * @file bitset.hpp
 * @brief ビットセット
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <array>
#include <span>
#include <cstdint>

namespace tmk_desktop {
/**
 * @brief ビットセット
 *
 * @tparam N ビット数
 * @tparam ValueT ビット列を格納する値の型
 */
template <size_t N, typename ValueT = uintmax_t>
class Bitset {
public:
  /**
   * @brief ビット列を格納する値の型のビット幅
   */
  static constexpr size_t VALUE_WIDTH = sizeof(ValueT) * 8;

  /**
   * @brief ビット列を格納するのに十分な値配列の長さ
   */
  static constexpr size_t VALUE_COUNT = (N + VALUE_WIDTH - 1) / VALUE_WIDTH;

  /**
   * @brief ビット列における位置
   */
  class Position {
  public:
    Position() = default;

    constexpr Position(size_t index) noexcept : index_(index) {}

    constexpr Position(size_t row, size_t col) noexcept : index_(row * VALUE_WIDTH + col) {}

    constexpr bool is_valid() const noexcept {
      return index_ < N;
    }

    constexpr size_t index() const noexcept {
      return index_;
    }

    constexpr size_t row() const noexcept {
      return index_ / VALUE_WIDTH;
    }

    constexpr size_t col() const noexcept {
      return index_ % VALUE_WIDTH;
    }

  private:
    size_t index_;
  };

  /**
   * @brief 全体を0で初期化する
   */
  constexpr Bitset() noexcept : values_{} {}

  /**
   * @brief 単一の値で全体を初期化する
   *
   * @param default_value 初期値
   */
  constexpr explicit Bitset(ValueT default_value) noexcept {
    for (auto& value : values_) {
      value = default_value;
    }
  }

  /**
   * @brief ビット列をコピーする
   *
   * @param values ビット列を格納する値の配列
   */
  constexpr explicit Bitset(std::span<const ValueT, VALUE_COUNT> values) noexcept {
    for (size_t i = 0; i < VALUE_COUNT; ++i) {
      values_[i] = values[i];
    }
  }

  /**
   * @brief ビット列をコピーする
   *
   * @param values ビット列を格納する値の配列
   */
  constexpr explicit Bitset(const ValueT (&values)[VALUE_COUNT]) noexcept : Bitset(std::span(values)) {}

  /**
   * @brief ビット列を格納する数値列を取得する
   *
   * @return 値の配列
   */
  constexpr std::span<const ValueT, VALUE_COUNT> values() const noexcept {
    return values_;
  }

  /**
   * @brief ビット列を格納する数値を取得する
   *
   * @return 値の配列
   */
  constexpr ValueT value(size_t i) const noexcept {
    if (i >= values_.size()) return 0;
    return values_[i];
  }

  /**
   * @brief ビットの状態を返す
   *
   * @param pos ビット列における位置
   * @retval true ビットが1
   * @retval false ビットが0。または、posが範囲外
   */
  constexpr bool operator[](const Position& pos) const noexcept {
    if (!pos.is_valid()) return false;
    return !!(values_[pos.row()] & (static_cast<ValueT>(1) << pos.col()));
  }

  /**
   * @brief ビット数を取得する
   */
  constexpr size_t size() const noexcept {
    return N;
  }

  /**
   * @brief ビットを更新する
   *
   * @param pos ビット列における位置
   * @param flag trueなら1、falseなら0
   */
  constexpr void set(const Position& pos, bool flag) noexcept {
    if (flag) {
      set(pos);
    } else {
      reset(pos);
    }
  }

  /**
   * @brief ビットを1にする
   *
   * @param pos ビット列における位置
   */
  constexpr void set(const Position& pos) noexcept {
    if (!pos.is_valid()) return;
    values_[pos.row()] |= static_cast<ValueT>(1) << pos.col();
  }

  /**
   * @brief ビットを0にする
   *
   * @param pos ビット列における位置
   */
  constexpr void reset(const Position& pos) noexcept {
    if (!pos.is_valid()) return;
    values_[pos.row()] &= ~(static_cast<ValueT>(1) << pos.col());
  }

  /**
   * @brief ビットを反転する
   *
   * @param pos ビット列における位置
   */
  constexpr void flip(const Position& pos) noexcept {
    if (!pos.is_valid()) return;
    values_[pos.row()] ^= static_cast<ValueT>(1) << pos.col();
  }

  /**
   * @brief ビットをすべて0にする
   */
  constexpr void clear() noexcept {
    for (auto& value : values_) {
      value = 0;
    }
  }

  /**
   * @brief 1になっているビットを列挙する
   *
   * @param pred Positionを引数とする関数オブジェクト
   */
  template <typename Pred>
  constexpr void scan(Pred pred) const noexcept {
    size_t index = 0;
    for (auto value : values_) {
      const auto next_index = index + VALUE_WIDTH;
      while (value != 0) {
        if (value & 1) {
          pred(Position{index});
        }
        value >>= 1;
        index++;
      }
      index = next_index;
    }
  }

  /**
   * @brief 二項演算を行う
   *
   * @param lhs 左辺のビットセット
   * @param rhs 右辺のビットセット
   * @param op ValueTの二項演算を行う関数オブジェクト
   * @return opの演算結果を格納するビットセットを返す
   */
  template <typename Op>
  friend constexpr Bitset binary_op(const Bitset& lhs, const Bitset& rhs, Op op) noexcept {
    Bitset result;
    for (size_t i = 0; i < result.values_.size(); ++i) {
      result.values_[i] = op(lhs.values_[i], rhs.values_[i]);
    }
    return result;
  }

  /**
   * @brief 1から0に変化するビットを抽出する
   *
   * @param prev 以前のビットセット
   * @param next 今回のビットセット
   * @return 1から0に変化したビットを1にしたビットセットを返す
   */
  friend constexpr Bitset set_to_reset(const Bitset& prev, const Bitset& next) noexcept {
    return binary_op(prev, next, [](auto lhs, auto rhs) { return lhs & ~rhs; });
  }

  /**
   * @brief 0から1に変化するビットを抽出する
   *
   * @param prev 以前のビットセット
   * @param next 今回のビットセット
   * @return 0から1に変化したビットを1にしたビットセットを返す
   */
  friend constexpr Bitset reset_to_set(const Bitset& prev, const Bitset& next) noexcept {
    return set_to_reset(next, prev);
  }

private:
  std::array<ValueT, VALUE_COUNT> values_;  ///< ビット列を格納する値の配列
};
}  // namespace tmk_desktop
