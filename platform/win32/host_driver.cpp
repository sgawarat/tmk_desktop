/**
 * @file host_driver.cpp
 * @brief Win32でのホストドライバ
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include "host_driver.hpp"
#include <array>
#include <Windows.h>
#include <tmk_desktop/bitset.hpp>
#include <tmk_desktop/key.hpp>
#include <tmk_desktop/keycode.hpp>
#include <tmk_desktop/engine/host_driver.hpp>
#include "scancode.hpp"
#include "logging.hpp"
#include "injected.hpp"

namespace tmk_desktop::win32 {
namespace {
struct Input final : INPUT {
  constexpr Input() noexcept
    : INPUT{} {
    clear();
  }

  constexpr Input(WORD scancode, bool pressed, bool extended) noexcept
    : INPUT{
        .type = INPUT_KEYBOARD,
        .ki = {
          .wScan = scancode,
          .dwFlags = static_cast<DWORD>(KEYEVENTF_SCANCODE),
        },
      } {
    if (!pressed) ki.dwFlags |= KEYEVENTF_KEYUP;
    if (extended) ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    add_injected(ki);
  }

  constexpr Input(WORD scancode, bool pressed = false) noexcept
    : Input(scancode, pressed, scancode > 255) {}

  Input(Keycode keycode, bool pressed = false) noexcept
    : Input(make_scancode(keycode), pressed) {}

  constexpr void clear() noexcept {
    type = INPUT_HARDWARE;
  }

  constexpr bool is_valid() const noexcept {
    // INPUT_HARDWAREは使わないので無効値として扱う
    return type != INPUT_HARDWARE;
  }

  void send() noexcept {
    if (is_valid()) {
      if (type == INPUT_KEYBOARD) {
        debug_log("Output: {}{} {} ({:#04x})\n",
            (!!(ki.dwFlags & KEYEVENTF_KEYUP) ? "KeyUp" : "KeyDown"),
            (!!(ki.dwFlags & KEYEVENTF_EXTENDEDKEY) ? "Ex" : ""),
            get_scancode_string(ki.wScan),
            ki.wScan);
      }
      SendInput(1, this, sizeof(INPUT));
    }
  }

  // 押してすぐ離すキー入力を行う
  void send_tap() noexcept {
    if (is_valid()) {
      debug_log("Output: Tap{} {} ({:#04x})\n",
          (!!(ki.dwFlags & KEYEVENTF_EXTENDEDKEY) ? "Ex" : ""),
          get_scancode_string(ki.wScan),
          ki.wScan);
      ki.dwFlags &= ~KEYEVENTF_KEYUP;
      SendInput(1, this, sizeof(INPUT));
      ki.dwFlags |= KEYEVENTF_KEYUP;
      SendInput(1, this, sizeof(INPUT));
    }
  }
};

using Keyset = Bitset<256>;
using ModKeyset = Bitset<8, uint8_t>;

Keyset keyset_{};  ///< 最新のキー状態
ModKeyset mod_keyset_{};  ///< 最新の修飾キー状態
Input latest_press_input_{};  ///< 最後にキーを押したときの入力イベント

/**
 * @brief ONからOFFに切り替わったキーを抽出する
 * 
 * @param prev 以前のキー状態を格納するビットセット
 * @param next 今回のキー状態を格納するビットセット
 * @return ONからOFFに変化したキーのビットを1にしたビットセットを返す
 */
template <size_t N, typename ValueT>
constexpr Bitset<N, ValueT> set_to_reset(const Bitset<N, ValueT>& prev, const Bitset<N, ValueT>& next) noexcept {
  return binary_op(prev, next, [] (auto lhs, auto rhs) {return lhs & ~rhs;});
}

/**
 * @brief OFFからONに切り替わったキーを抽出する
 * 
 * @param prev 以前のキー状態を格納するビットセット
 * @param next 今回のキー状態を格納するビットセット
 * @return ONからOFFに変化したキーのビットを1にしたビットセットを返す
 */
template <size_t N, typename ValueT>
constexpr Bitset<N, ValueT> reset_to_set(const Bitset<N, ValueT>& prev, const Bitset<N, ValueT>& next) noexcept {
  return binary_op(prev, next, [] (auto lhs, auto rhs) {return ~lhs & rhs;});
}
}  // namespace

void send_keyboard(report_keyboard_t* report_ptr) noexcept {
  if (!report_ptr) return;
  const auto& report = *report_ptr;

  // 前回のキー状態を保存する
  const auto prev_keyset = keyset_;  // 前回のキー状態
  const auto prev_mod_keyset = mod_keyset_;  // 前回の修飾キー状態

  // 今回のキー状態を記録する
  keyset_ = Keyset{report.nkro.bits};  // 今回のキー状態
  mod_keyset_ = ModKeyset{report.nkro.mods};  // 今回の修飾キー状態

  // 今回の更新で変化するキーを抽出する
  const auto pressed_keyset = reset_to_set(prev_keyset, keyset_);  // 押した
  const auto released_keyset = set_to_reset(prev_keyset, keyset_); // 離した
  const auto pressed_mod_keyset = reset_to_set(prev_mod_keyset, mod_keyset_);  // 押した
  const auto released_mod_keyset = set_to_reset(prev_mod_keyset, mod_keyset_); // 離した

  // キーイベントを送信する
  released_keyset.scan([](auto pos) {  // 非修飾キーを離す
    Input(Keycode(pos.index()), false).send();
  });
  released_mod_keyset.scan([](auto pos) {  // 修飾キーを離す
    Input(Keycode(KC_LCTRL + pos.index()), false).send();

    // TODO: キーリピート中に修飾キーを離すと、キーリピート入力にもその修飾キーの状態が適用される挙動について検証する
    // HACK: 本来のキーリピートの挙動とは異なるが、意図しない文字を出さないためにリピートをここで終わらせる
    latest_press_input_.clear();
  });
  pressed_mod_keyset.scan([](auto pos) {  // 修飾キーを押す
    latest_press_input_ = Input(Keycode(KC_LCTRL + pos.index()), true);
    latest_press_input_.send();
  });
  pressed_keyset.scan([](auto pos) {  // 非修飾キーを押す
    latest_press_input_ = Input(Keycode(pos.index()), true);
    latest_press_input_.send();
  });
}

void send_mouse(report_mouse_t* report_ptr) noexcept {
  if (!report_ptr) return;
  const auto& report = *report_ptr;

  debug_log("send_mouse: buttons:{:#02x} x:{}, y:{}, v:{}, h:{}\n", report.buttons, report.x, report.y, report.v, report.h);
  // TODO: send_mouseを実装する
}

void send_system(uint16_t val) noexcept {
  debug_log("send_system: val:{}\n", val);
  // TODO: send_systemを実装する
}

void send_consumer(uint16_t val) noexcept {
  // 間借りした値に割り振られた処理を行う
  switch (val) {
    case engine::CSMR_MICROPHONE_MUTE: {
      // HACK: 自前の環境のマイクキーの入力を模倣している
      Input(0x02, false, true).send_tap();
      Input(0x58, false, false).send();
      return;
    }
    default: break;
  }

  const auto keycode = [=]() -> Keycode {
    switch (val) {
      case AUDIO_MUTE: return KC_AUDIO_MUTE;
      case AUDIO_VOL_UP: return KC_AUDIO_VOL_UP;
      case AUDIO_VOL_DOWN: return KC_AUDIO_VOL_DOWN;
      case TRANSPORT_NEXT_TRACK: return KC_MEDIA_NEXT_TRACK;
      case TRANSPORT_PREV_TRACK: return KC_MEDIA_PREV_TRACK;
      case TRANSPORT_STOP: return KC_MEDIA_STOP;
      case TRANSPORT_STOP_EJECT: return KC_MEDIA_EJECT;
      case TRANSPORT_PLAY_PAUSE: return KC_MEDIA_PLAY_PAUSE;
      case APPLAUNCH_CC_CONFIG: return KC_MEDIA_SELECT;
      case APPLAUNCH_EMAIL: return KC_MAIL;
      case APPLAUNCH_CALCULATOR: return KC_CALCULATOR;
      case APPLAUNCH_LOCAL_BROWSER: return KC_MY_COMPUTER;
      case APPCONTROL_SEARCH: return KC_WWW_SEARCH;
      case APPCONTROL_HOME: return KC_WWW_HOME;
      case APPCONTROL_BACK: return KC_WWW_BACK;
      case APPCONTROL_FORWARD: return KC_WWW_FORWARD;
      case APPCONTROL_STOP: return KC_WWW_STOP;
      case APPCONTROL_REFRESH: return KC_WWW_REFRESH;
      case APPCONTROL_BOOKMARKS: return KC_WWW_FAVORITES;
    }
    return KC_NO;
  }();
  if (keycode != KC_NO) Input(keycode).send_tap();
}

void send_key_repeat() noexcept {
  latest_press_input_.send();
}

void clear_key_repeat() noexcept {
  latest_press_input_.clear();
}
}  //namespace tmk_desktop::win32
