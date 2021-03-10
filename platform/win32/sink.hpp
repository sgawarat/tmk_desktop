/**
 * @file sink.hpp
 * @brief OSに入力イベントを送り出す側
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <Windows.h>

namespace tmk_desktop::win32 {
/**
 * @brief Sinkの状態
 */
enum class SinkStatus {
  RESET,  ///< リセット済み
  RUNNING,  ///< 動作中
  STOPPING,  ///< 停止しようとしている
  STOPPED,  ///< 停止した
};

/**
 * @brief 例外が発生したときにcatch内で呼ばれる関数の型
 */
using OnSinkError = void (*)() noexcept;

/**
 * @brief Sinkを始動させる
 * 
 * @param on_error 例外発生時に呼ばれる関数
 * @retval true 始動に成功
 * @retval false すでに始動している
 * @exception system_error スレッドの生成に失敗
 * @exception 設定の読み込みに失敗
 */
bool start_sink(OnSinkError on_error = nullptr);

/**
 * @brief Sinkを停止させる
 * 
 * @retval true 停止に成功
 * @retval false すでに停止しているか、停止しようとしている
 * @exception system_error スレッドのjoinに失敗
 */
bool stop_sink();

/**
 * @brief Sinkにイベントを送る
 * 
 * @param info 入力イベント
 * @exception bad_alloc メモリ確保に失敗
 * @exception system_error mutexのロックに失敗
 */
void send_to_sink(const KBDLLHOOKSTRUCT& info);

/**
 * @brief Sinkの状態を取得する
 * 
 * @return 現在のSinkの状態 
 */
SinkStatus get_sink_status() noexcept;
}  // namespace tmk_desktop::win32
