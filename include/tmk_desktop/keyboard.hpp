/**
 * @file keyboard.hpp
 * @brief キーボードを動かす側
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <exception>
#include "event.hpp"

namespace tmk_desktop {
/**
 * @brief Keyboardの状態
 */
enum class KeyboardStatus {
  RESET,  ///< リセット済み
  RUNNING,  ///< 動作中
  STOPPING,  ///< 停止しようとしている
  STOPPED,  ///< 停止した
};

/**
 * @brief Keyboardを始動させる
 * 
 * @retval true 始動に成功
 * @retval false すでに始動している
 * @exception system_error スレッドの生成に失敗
 * @exception 設定の読み込みに失敗
 */
bool start_keyboard();

/**
 * @brief Keyboardを停止させる
 * 
 * @retval true 停止に成功
 * @retval false すでに停止しているか、停止しようとしている
 * @exception system_error スレッドのjoinに失敗
 */
bool stop_keyboard();

/**
 * @brief Keyboardにイベントを送る
 * 
 * @param event 入力イベント
 * @exception bad_alloc メモリ確保に失敗
 * @exception system_error mutexのロックに失敗
 */
void send_to_keyboard(const KeyEvent& event);

/**
 * @brief Keyboardの状態を取得する
 * 
 * @return 現在のKeyboardの状態 
 */
KeyboardStatus get_keyboard_status() noexcept;

/**
 * @brief Keyboardが異常停止したときに呼ばれる関数
 * 
 * アプリケーション側で実装される。
 */
void on_keyboard_error(std::exception& e) noexcept;
}  // namespace tmk_desktop
