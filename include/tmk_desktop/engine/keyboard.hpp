/**
 * @file keyboard.hpp
 * @brief キーボード
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <tmk_desktop/key.hpp>

namespace tmk_desktop::engine::inline v1 {
/**
 * @brief キーボードを始動させる
 */
void start_keyboard() noexcept;

/**
 * @brief キーボードを停止させる
 */
void stop_keyboard() noexcept;

/**
 * @brief キーボードの状態を更新する
 * 
 * @param key 操作するキー
 * @param pressed キーを押したか
 */
void update_keyboard(Key key, bool pressed) noexcept;
}  // namespace tmk_desktop::engine::inline v1
