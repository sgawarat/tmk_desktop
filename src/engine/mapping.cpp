/**
 * @file mapping.cpp
 * @brief アクションマップの管理
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <tmk_desktop/engine/mapping.hpp>
#include <array>
#include <vector>
#include <cstdint>
#include <tmk_desktop/key.hpp>
#include <tmk_desktop/action.hpp>
#include <tmk_desktop/macro.hpp>
#include <tmk_desktop/engine/host_driver.hpp>

extern "C" {
#include <tmk_core/common/action_layer.h>
#include <tmk_core/common/action_util.h>
}  // extern "C"

namespace tmk_desktop::engine::inline v1 {
namespace {
std::array<std::array<action_t, Key::COUNT>, 32> action_maps_;
std::array<std::vector<macro_t>, 256> macro_map_;

/**
 * @brief 修飾キーの状態に応じてずらしたレイヤーをON/OFFする
 * 
 * Ctrl,Alt,Gui押下時のShiftに個別のキー配列を割り当てたいときに使う。
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
 * Ctrl,Alt,GuiにShiftと別のキー配列を割り当てたいときに使う。
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
      if (layer_state == 0) layer_state |= 1;  // 暗黙的に使われるビット0を立てる
      layer_state <<= offset;
    }
  } else {
    unregister_mods(MOD_BIT(keycode));

    // Ctrl,Alt,Guiのいずれも押されていなければ、レイヤーを右にずらす
    if ((get_mods() & 0xdd) == 0) layer_state >>= offset;
  }
}
}  // namespace

void clear_mapping() {
  for (auto& action_map : action_maps_) {
    action_map.fill(AC_NO);
  }
  for (auto& macro : macro_map_) {
    macro.clear();
  }
}

bool update_action_map(uint8_t layer, const action_t* actions, size_t action_count) {
  if (layer >= action_maps_.size()) return false;
  auto& action_map = action_maps_[layer];

  if (!actions) return false;

  for (auto& action : action_map) {
    if (action_count == 0) break;
    action = *actions++;
    action_count--;
  }
  return true;
}

bool update_macro(uint8_t id, const macro_t* macro, size_t size) {
  if (!macro) return false;

  const macro_t* macro_end = macro + size;
  if (size == 0) {
    // マクロの長さを測る
    while (*macro_end++ != END) {
      if (++size >= 32) return false;  // マクロが長すぎる
    }
  }
  macro_map_[id].assign(macro, macro_end);
  return true;
}

extern "C" {
action_t action_for_key(uint8_t layer, keypos_t keypos) {
  if (layer >= action_maps_.size()) return AC_NO;
  auto& action_map = action_maps_[layer];

  if (keypos.row >= MATRIX_ROWS || keypos.col >= MATRIX_COLS) return AC_NO;
  const size_t index = static_cast<size_t>(keypos.row) * MATRIX_COLS + keypos.col;

  if (index >= action_map.size()) return AC_NO;
  return action_map[index];
}

const macro_t* action_get_macro(keyrecord_t* record, uint8_t id, [[maybe_unused]] uint8_t opt) {
  if (!record || !record->event.pressed) return nullptr;
  auto& macro = macro_map_[id];

  if (macro.empty()) return nullptr;
  return macro.data();
}

void action_function(keyrecord_t* record, uint8_t id, [[maybe_unused]] uint8_t opt) {
  if (!record) return;
  const auto& event = record->event;
  // const auto& tap = record->tap;

  switch (id) {
    case FN_LAYER_ON_OFF_OFFSET_LSHIFT:
      layer_on_off_offset(1, KC_LSHIFT, event.pressed, opt);
      break;
    case FN_LAYER_ON_OFF_OFFSET_RSHIFT:
      layer_on_off_offset(2, KC_RSHIFT, event.pressed, opt);
      break;
    case FN_LAYER_TRANSLATE_LCTRL:
      layer_translate(KC_LCTRL, event.pressed, opt);
      break;
    case FN_LAYER_TRANSLATE_LALT:
      layer_translate(KC_LALT, event.pressed, opt);
      break;
    case FN_LAYER_TRANSLATE_LGUI:
      layer_translate(KC_LGUI, event.pressed, opt);
      break;
    case FN_LAYER_TRANSLATE_RCTRL:
      layer_translate(KC_RCTRL, event.pressed, opt);
      break;
    case FN_LAYER_TRANSLATE_RALT:
      layer_translate(KC_RALT, event.pressed, opt);
      break;
    case FN_LAYER_TRANSLATE_RGUI:
      layer_translate(KC_RGUI, event.pressed, opt);
      break;
    case FN_MUTE_MIC:
      if (event.pressed) send_consumer_event(CSMR_MICROPHONE_MUTE);
      break;
  }
}
}  // extern "C"
}  // namespace tmk_desktop::engine::inline v1
