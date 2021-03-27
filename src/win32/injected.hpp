/**
 * @file injected.hpp
 * @brief 自前のINJECTEDフラグ
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <Windows.h>

namespace tmk_desktop::inline win32 {
/**
 * @brief 自前のINJECTEDフラグ
 *
 * IMEの状態遷移を含むキー入力をSendInputすると、INJECTEDフラグを持たない補助的なキー入力を発生させることがある。それらはフックプロシージャ内で区別できないので、このフラグを使って無理矢理に識別できるようにする。
 */
static constexpr ULONG_PTR EXTRA_INFO_INJECTED = 0xfffffffc;

/**
 * @brief INJECTEDフラグを追加する
 *
 * @param ki キー入力情報
 */
constexpr void add_injected(KEYBDINPUT& ki) noexcept {
  // HACK: 用途外の方法で自前のINJECTEDフラグを立てる
  ki.dwExtraInfo = EXTRA_INFO_INJECTED;
}

/**
 * @brief INJECTEDフラグを取り除いて無害化する
 *
 * @param info キー入力イベント
 * @return true 除去に成功
 * @return false INJECTEDされていなかった
 */
constexpr bool remove_injected(KBDLLHOOKSTRUCT& info) noexcept {
  if (info.dwExtraInfo != EXTRA_INFO_INJECTED) return false;
  info.flags |= LLMHF_INJECTED;
  info.dwExtraInfo = 0;
  return true;
}
}  // namespace tmk_desktop::inline win32
