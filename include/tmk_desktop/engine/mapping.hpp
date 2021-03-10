/**
 * @file mapping.hpp
 * @brief マッピング
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <cstdint>
#include <tmk_desktop/action.hpp>
#include <tmk_desktop/macro.hpp>

namespace tmk_desktop::engine::inline v1 {
/**
 * @brief 関数アクションのID
 */
enum FunctionActionId {
  /**
   * @brief 押している間だけレイヤーを有効化する（オフセット調整付）
   */
  FN_LAYER_ON_OFF_OFFSET_LSHIFT,
  FN_LAYER_ON_OFF_OFFSET_RSHIFT,

  /**
   * @brief 左Ctrlを操作する（オフセット調整付）
   */
  FN_LAYER_TRANSLATE_LCTRL,
  FN_LAYER_TRANSLATE_LALT,
  FN_LAYER_TRANSLATE_LGUI,
  FN_LAYER_TRANSLATE_RCTRL,
  FN_LAYER_TRANSLATE_RALT,
  FN_LAYER_TRANSLATE_RGUI,

  /**
   * @brief マイクの有効/無効を切り替える
   */
  FN_MUTE_MIC,
};

/**
 * @brief マッピング設定をクリアする
 */
void clear_mapping();

/**
 * @brief アクションマップを更新する
 * 
 * @param layer レイヤー
 * @param actions アクションの配列
 * @param size actionsの大きさ
 * @return 成功したか
 */
bool update_action_map(uint8_t layer, const action_t* actions, size_t size);

/**
 * @brief マクロを更新する
 * 
 * @param id マクロID
 * @param macro マクロのポインタ
 * @param size マクロの大きさ
 * @return 成功したか
 */
bool update_macro(uint8_t id, const macro_t* macro, size_t size);
}  // namespace tmk_desktop::engine::inline v1
