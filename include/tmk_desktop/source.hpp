/**
 * @file source.hpp
 * @brief OSから入力イベントを受け取る側
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <exception>

namespace tmk_desktop {
/**
 * @brief Sourceの状態
 */
enum class SourceStatus {
  RESET,  ///< リセット済み
  RUNNING,  ///< 動作中
  STOPPING,  ///< 停止しようとしている
  STOPPED,  ///< 停止した
};

/**
 * @brief Sourceを始動させる
 * 
 * @retval true 始動に成功
 * @retval false すでに始動している
 * @exception system_error スレッドの生成に失敗
 */
bool start_source();

/**
 * @brief Sourceを停止させる
 * 
 * @retval true 停止に成功
 * @retval false すでに停止しているか、停止しようとしている
 * @exception system_error スレッドのjoinに失敗
 */
bool stop_source();

/**
 * @brief Sourceの状態を取得する
 * 
 * @return 現在のSourceの状態 
 */
SourceStatus get_source_status() noexcept;

/**
 * @brief Sourceが異常停止したときに呼ばれる関数
 * 
 * アプリケーション側で実装される。
 */
void on_source_error(std::exception& e) noexcept;
}  // namespace tmk_desktop
