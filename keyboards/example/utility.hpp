/**
 * @file utility.hpp
 * @brief ユーティリティ
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <cstdint>

extern "C" {
#include <common/keycode.h>
#include <common/action.h>
#include <common/action_util.h>
#include <common/action_layer.h>
}  // extern "C"

namespace tmk_desktop {
/**
 * @brief 修飾キーの状態に応じてずらしたレイヤーをON/OFFする
 *
 * Ctrl,Alt,Guiに別のレイヤーを用意したいときのShiftキーに用いる。
 *
 * @param layer レイヤー番号
 * @param keycode 入力されるキー
 * @param pressed 押したか
 * @param offset ずらす量
 */
inline void layer_on_off_offset(uint8_t layer, uint8_t keycode, bool pressed, size_t offset) noexcept {
  // Ctrl,Alt,Guiのいずれかが押されていれば、レイヤー番号を調整する
  if ((get_mods() & 0xdd) != 0) layer += offset;

  if (pressed) {
    layer_on(layer);
  } else {
    layer_off(layer);
  }
}

/**
 * @brief 修飾キーの状態に応じてレイヤーをずらす
 *
 * Ctrl,Alt,Guiに別のレイヤーを用意したい場合に用いる。
 *
 * @param keycode 入力されるキー
 * @param pressed 押したか
 * @param offset ずらす量
 */
inline void layer_translate(uint8_t keycode, bool pressed, size_t offset) noexcept {
  if (pressed) {
    const auto mods = get_mods();  // キーが押されるかどうかを調べるため、更新前の状態を保存しておく
    register_mods(MOD_BIT(keycode));

    // Ctrl,Alt,Guiのいずれかが押されていれば、レイヤーを左にずらす
    if ((mods & 0xdd) == 0) {
      if (layer_state == 0) layer_state |= 1;  // 空のときに暗黙的に使われるビット0を立てる
      layer_state <<= offset;
    }
  } else {
    unregister_mods(MOD_BIT(keycode));

    // Ctrl,Alt,Guiのいずれも押されていなければ、レイヤーを右にずらす
    if ((get_mods() & 0xdd) == 0) layer_state >>= offset;
  }
}

/**
 * @brief アクションに修飾キーを追加する
 */
inline action_t add_mods(action_t action, uint16_t mods) noexcept {
  switch (action.kind.id) {
    case ACT_LMODS:
    case ACT_RMODS:
      action.code |= mods & 0x1f00;
      break;
  }
  return action;
}

/**
 * @brief アクションが右修飾キーを使うようにする
 */
inline action_t set_right_mods(action_t action) noexcept {
  switch (action.kind.id) {
    case ACT_LMODS:
      action.key.kind = ACT_RMODS;
      break;
  }
  return action;
}

/**
 * @brief 親指キー用LAYER_TAP
 *
 * 長押しで専用レイヤーをON/OFF、単打で指定レイヤーに切り替えてキー入力を行う。
 * 変換/無変換をIMEの有効/無効として使うときにIMEの状態に応じたレイヤーを用意したい場合に使う。
 *
 * @param keycode 単打のときに入力されるキー
 * @param im_layer 単打のときに切り替わるレイヤー
 * @param thumb_layer 長押しのときにON/OFFされるレイヤー
 */
inline void layer_tap_thumb(const keyevent_t& event, const tap_t& tap, uint8_t keycode, uint8_t im_layer, uint8_t thumb_layer) noexcept {
  if (event.pressed) {
    if (tap.count > 0) {
      layer_move(im_layer);
      register_code(keycode);
    } else {
      layer_on(thumb_layer);
    }
  } else {
    if (tap.count > 0) {
      unregister_code(keycode);
    } else {
      layer_off(thumb_layer);
    }
  }
}

/**
 * @brief レイヤーをオンにしてキーを押す
 * 
 * 親指キーのLAYER_TAPを維持しながら日本語/英語のレイアウト調整を行うのに使う。
 * 
 * @param event キーイベント
 * @param layer オンにするレイヤー番号
 * @param mask 保護するレイヤーのビットマスク
 * @param keycode 押すキーコード
 */
inline void layer_move_with_key(const keyevent_t& event, uint8_t layer, uint32_t mask, uint8_t keycode) noexcept {
  if (event.pressed) {
    layer_and(mask);
    layer_on(layer);
    register_code(keycode);
  } else {
    unregister_code(keycode);
  }
}
}  // namespace tmk_desktop
