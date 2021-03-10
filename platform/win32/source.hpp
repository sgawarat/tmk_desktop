/**
 * @file source.hpp
 * @brief OSから入力イベントを受け取る側
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

namespace tmk_desktop::win32 {
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
 * @brief 例外が発生したときにcatch内で呼ばれる関数の型
 */
using OnSourceError = void (*)() noexcept;

/**
 * @brief Sourceを始動させる
 * 
 * @param on_error 例外発生時に呼ばれる関数
 * @retval true 始動に成功
 * @retval false すでに始動している
 * @exception system_error スレッドの生成に失敗
 */
bool start_source(OnSourceError on_error = nullptr);

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
}  // namespace tmk_desktop::win32
