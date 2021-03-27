/**
 * @file sender.hpp
 * @brief Win32の入力イベントを送信するやつ
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <Windows.h>
#include <tmk_desktop/win32/settings.hpp>
#include "injected.hpp"

namespace tmk_desktop::inline win32 {
inline uint16_t keycode_to_scancode(uint8_t keycode) noexcept {
  return keycode_to_scancode_table[keycode];
}

struct Input final : INPUT {
  constexpr Input() noexcept : INPUT{.type = INPUT_HARDWARE} {}

  explicit Input(uint8_t keycode, bool pressed = true) noexcept
      : INPUT{
            .type = INPUT_KEYBOARD,
            .ki =
                {
                    .wScan = keycode_to_scancode(keycode),
                    .dwFlags = static_cast<DWORD>(KEYEVENTF_SCANCODE),
                },
        } {
    if (ki.wScan > 0xff) ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;  // この関係性が本当に正しいかは不明
    if (!pressed) ki.dwFlags |= KEYEVENTF_KEYUP;
    add_injected(ki);
  }

  constexpr void clear() noexcept {
    type = INPUT_HARDWARE;
  }

  constexpr bool is_valid() const noexcept {
    // INPUT_HARDWAREは使わないので無効値として扱う
    return type != INPUT_HARDWARE;
  }

  void send() noexcept {
    if (is_valid()) SendInput(1, this, sizeof(INPUT));
  }

  void send_tap() noexcept {
    if (is_valid()) {
      ki.dwFlags &= ~KEYEVENTF_KEYUP;
      SendInput(1, this, sizeof(INPUT));
      ki.dwFlags |= KEYEVENTF_KEYUP;
      SendInput(1, this, sizeof(INPUT));
    }
  }
};

class EventSender final {
public:
  void enable() noexcept {}

  void disable() noexcept {}

  /**
   * @brief 押すイベントを送信する
   */
  void send_key_press(uint8_t keycode) noexcept {
    latest_press_keycode_ = keycode;
    latest_press_input_ = Input(keycode, true);
    latest_press_input_.send();
  }

  /**
   * @brief 離すイベントを送信する
   */
  void send_key_release(uint8_t keycode) noexcept {
    Input(keycode, false).send();
    if (keycode == latest_press_keycode_) {
      latest_press_keycode_ = KC_NO;
      latest_press_input_.clear();
    }
  }

  /**
   * @brief 押してすぐ離すイベントを送信する
   */
  void send_key_tap(uint8_t keycode) noexcept {
    Input(keycode).send_tap();
    latest_press_keycode_ = KC_NO;
    latest_press_input_.clear();
  }

  /**
   * @brief イベントをそのまま送信する
   */
  void send_native_event(INPUT input) noexcept {
    SendInput(1, &input, sizeof(INPUT));
    latest_press_keycode_ = KC_NO;
    latest_press_input_.clear();
  }

  /**
   * @brief キーリピートを表すイベントを送信する
   */
  void send_key_repeat() noexcept {
    latest_press_input_.send();
  }

  /**
   * @brief キーリピート情報をクリアする
   */
  void clear_key_repeat() noexcept {
    latest_press_keycode_ = KC_NO;
    latest_press_input_.clear();
  }

private:
  uint8_t latest_press_keycode_ = KC_NO;  ///< 最後に押したキー
  Input latest_press_input_{};            ///< 最後に押したキーイベント
};
}  // namespace tmk_desktop::inline win32
