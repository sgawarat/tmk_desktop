/**
 * @file keyboard.cpp
 * @brief キーボードの管理
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <tmk_desktop/engine/keyboard.hpp>
#include <cstdint>
#include <tmk_desktop/bitset.hpp>

extern "C" {
#include <tmk_core/common/keyboard.h>
#include <tmk_core/common/action.h>
#include <tmk_core/common/matrix.h>
}  // extern "C"

namespace tmk_desktop::engine::inline v1 {
namespace {
 Bitset<MATRIX_SIZE, matrix_row_t> matrix_;  ///< キーボード状態のビット列
}  // namespace

void start_keyboard() noexcept {
  keyboard_init();
}

void stop_keyboard() noexcept {
  clear_keyboard();
}

void update_keyboard(Key key, bool pressed) noexcept {
  matrix_.set(key.value(), pressed);
  keyboard_task();
}

extern "C" {
/**
 * @brief キー状態をすべて記録させるフラグ
 */
uint8_t keyboard_protocol = 1;

void matrix_init() {
  matrix_.clear();
}

uint8_t matrix_scan() {
  // 更新処理は外で行われる
  return 0;
}

matrix_row_t matrix_get_row(uint8_t row) {
  return matrix_.value(row);
}
}  // extern "C"
}  // namespace tmk_desktop::engine::inline v1
