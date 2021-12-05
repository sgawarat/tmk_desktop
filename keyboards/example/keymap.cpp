/**
 * @file keymap.cpp
 * @brief キーマップ
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <array>
#include <tmk_desktop/sink.hpp>
#include "jp109.hpp"
#include "utility.hpp"

extern "C" {
#include <common/action.h>
}  // extern "C"

namespace tmk_desktop {
using namespace jp109;

// レイヤー番号
enum Layer : uint8_t {
  L_COLEMAK_P,         // Colemak (配置調整あり)
  L_LSHIFT_COLEMAK_P,  // 左シフト付きColemak (配置調整あり)
  L_RSHIFT_COLEMAK_P,  // 右シフト付きColemak (配置調整あり)
  L_QWERTY,            // QWERTY (Ctrl,Alt,Gui用)
  L_LSHIFT_QWERTY,     // 左シフト付きQWERTY (Ctrl,Alt,Gui用)
  L_RSHIFT_QWERTY,     // 右シフト付きQWERTY (Ctrl,Alt,Gui用)
  L_QWERTY_JP,         // QWERTY (日本語入力用)
  L_LSHIFT_QWERTY_JP,  // 左シフト付きQWERTY (日本語入力用)
  L_RSHIFT_QWERTY_JP,  // 右シフト付きQWERTY (日本語入力用)
  L_LTHUMB,            // 左親指
  L_RTHUMB,            // 右親指
};

// レイヤー番号のビットマスク
constexpr uint32_t L_MASK_THUMBS = (1 << L_LTHUMB) | (1 << L_RTHUMB);

// マクロID
enum MacroId {
  M_BLOCK_COMMENT_BEGIN,  // ブロックコメント / **
  M_RIGHT_ARROW,          // 右矢印 ->
};

// 関数ID
enum FuncId {
  FN_LAYER_OFFSET_LCTRL,   // 修飾キーに基づくレイヤー調整 (左Ctrl用)
  FN_LAYER_OFFSET_LSHIFT,  // 修飾キーに基づくレイヤー調整 (左Shift用)
  FN_LAYER_OFFSET_LALT,    // 修飾キーに基づくレイヤー調整 (左Alt用)
  FN_LAYER_OFFSET_LGUI,    // 修飾キーに基づくレイヤー調整 (左Gui用)
  FN_LAYER_OFFSET_RCTRL,   // 修飾キーに基づくレイヤー調整 (右Ctrl用)
  FN_LAYER_OFFSET_RSHIFT,  // 修飾キーに基づくレイヤー調整 (右Shift用)
  FN_LAYER_OFFSET_RALT,    // 修飾キーに基づくレイヤー調整 (右Alt用)
  FN_LAYER_OFFSET_RGUI,    // 修飾キーに基づくレイヤー調整 (右Gui用)
  FN_MICROPHONE_MUTE,      // マイクのオン/オフを切り替える
  FN_LAYER_MOVE_HENKAN,    // レイヤー調整付き変換キー
  FN_LAYER_MOVE_MUHENKAN,  // レイヤー調整付き無変換キー
};

// 関数ID (TAP)
enum FuncTapId {
  FN_TAP_LTHUMB_MUHENKAN,  // 長押しで左親指レイヤー、単打で無変換キー
  FN_TAP_RTHUMB_HENKAN,    // 長押しで右親指レイヤー、単打で変換キー
};

// アクション
constexpr action_t AC_BLOCK_COMMENT_BEGIN = ACTION_MACRO(M_BLOCK_COMMENT_BEGIN);
constexpr action_t AC_RIGHT_ARROW = ACTION_MACRO(M_RIGHT_ARROW);
constexpr action_t AC_LAYER_OFFSET_LCTRL = ACTION_FUNCTION(FN_LAYER_OFFSET_LCTRL);
constexpr action_t AC_LAYER_OFFSET_LSHIFT = ACTION_FUNCTION(FN_LAYER_OFFSET_LSHIFT);
constexpr action_t AC_LAYER_OFFSET_LALT = ACTION_FUNCTION(FN_LAYER_OFFSET_LALT);
constexpr action_t AC_LAYER_OFFSET_LGUI = ACTION_FUNCTION(FN_LAYER_OFFSET_LGUI);
constexpr action_t AC_LAYER_OFFSET_RCTRL = ACTION_FUNCTION(FN_LAYER_OFFSET_RCTRL);
constexpr action_t AC_LAYER_OFFSET_RSHIFT = ACTION_FUNCTION(FN_LAYER_OFFSET_RSHIFT);
constexpr action_t AC_LAYER_OFFSET_RALT = ACTION_FUNCTION(FN_LAYER_OFFSET_RALT);
constexpr action_t AC_LAYER_OFFSET_RGUI = ACTION_FUNCTION(FN_LAYER_OFFSET_RGUI);
constexpr action_t AC_MICROPHONE_MUTE = ACTION_FUNCTION(FN_MICROPHONE_MUTE);
constexpr action_t AC_LAYER_MOVE_MUHENKAN = ACTION_FUNCTION(FN_LAYER_MOVE_MUHENKAN);
constexpr action_t AC_LAYER_MOVE_HENKAN = ACTION_FUNCTION(FN_LAYER_MOVE_HENKAN);
constexpr action_t AC_LTHUMB_MUHENKAN = ACTION_FUNCTION_TAP(FN_TAP_LTHUMB_MUHENKAN);
constexpr action_t AC_RTHUMB_HENKAN = ACTION_FUNCTION_TAP(FN_TAP_RTHUMB_HENKAN);
constexpr action_t AC_LAYER_TAP_LTHUMB_BSPACE = ACTION_LAYER_TAP(L_LTHUMB, KC_BSPACE);
constexpr action_t AC_LAYER_TAP_RTHUMB_ENTER = ACTION_LAYER_TAP(L_RTHUMB, KC_ENTER);
constexpr action_t AC_LAYER_ON_OFF_LSHIFT_QWERTY_JP = ACTION_LAYER_ON_OFF(L_LSHIFT_QWERTY_JP);
constexpr action_t AC_LAYER_ON_OFF_RSHIFT_QWERTY_JP = ACTION_LAYER_ON_OFF(L_RSHIFT_QWERTY_JP);

extern "C" {
action_t action_for_key(uint8_t layer, keypos_t pos) {
  /* clang-format off */
  static constexpr std::array COLEMAK_P_ACTIONMAP{
    AC_GRAVE, AC_1, AC_2, AC_3, AC_4, AC_5, AC_6, AC_7, AC_8, AC_9, AC_0, AC_LBRACKET, AC_RBRACKET, AC_YEN,
    AC_Q, AC_W, AC_F, AC_P, AC_G, AC_J, AC_L, AC_U, AC_Y, AC_SCOLON, AC_QUOTE, AC_EQUAL,
    AC_A, AC_R, AC_S, AC_T, AC_D, AC_H, AC_N, AC_E, AC_I, AC_O, AC_UNDERSCORE, AC_ENTER,
    AC_Z, AC_X, AC_C, AC_V, AC_B, AC_K, AC_M, AC_COMMA, AC_DOT, AC_SLASH, AC_LAYER_OFFSET_RSHIFT,
    // AC_LTHUMB_MUHENKAN, AC_RTHUMB_HENKAN
    AC_LAYER_TAP_LTHUMB_BSPACE, AC_LAYER_TAP_RTHUMB_ENTER, AC_DELETE,
    AC_LAYER_OFFSET_LCTRL, AC_LAYER_OFFSET_LSHIFT, AC_LAYER_OFFSET_LALT, AC_LAYER_OFFSET_LGUI,
    AC_LAYER_OFFSET_RCTRL, AC_LAYER_OFFSET_RSHIFT, AC_LAYER_OFFSET_RALT, AC_LAYER_OFFSET_RGUI,
  };
  static constexpr std::array SHIFTED_COLEMAK_P_ACTIONMAP{
    AC_TILDE, AC_EXCLAIM, AC_AT, AC_HASH, AC_DOLLAR, AC_PERCENT, AC_CIRCUMFLEX, AC_AMPERSAND, AC_ASTERISK, AC_LPAREN, AC_RPAREN, AC_LBRACE, AC_RBRACE, AC_PIPE,
    AC_SHIFT_Q, AC_SHIFT_W, AC_SHIFT_F, AC_SHIFT_P, AC_SHIFT_G, AC_SHIFT_J, AC_SHIFT_L, AC_SHIFT_U, AC_SHIFT_Y, AC_COLON, AC_DQUOTE, AC_PLUS,
    AC_SHIFT_A, AC_SHIFT_R, AC_SHIFT_S, AC_SHIFT_T, AC_SHIFT_D, AC_SHIFT_H, AC_SHIFT_N, AC_SHIFT_E, AC_SHIFT_I, AC_SHIFT_O, AC_EQUAL, AC_SHIFT_ENTER,
    AC_SHIFT_Z, AC_SHIFT_X, AC_SHIFT_C, AC_SHIFT_V, AC_SHIFT_B, AC_SHIFT_K, AC_SHIFT_M, AC_LT, AC_GT, AC_QUESTION, AC_LAYER_OFFSET_RSHIFT,
    // AC_SHIFT_MUHENKAN, AC_SHIFT_HENKAN,
    AC_SHIFT_BSPACE, AC_SHIFT_ENTER, AC_SHIFT_DELETE,
    AC_LAYER_OFFSET_LCTRL, AC_LAYER_OFFSET_LSHIFT, AC_LAYER_OFFSET_LALT, AC_LAYER_OFFSET_LGUI,
    AC_LAYER_OFFSET_RCTRL, AC_LAYER_OFFSET_RSHIFT, AC_LAYER_OFFSET_RALT, AC_LAYER_OFFSET_RGUI,
  };
  static constexpr std::array QWERTY_ACTIONMAP{
    AC_GRAVE, AC_1, AC_2, AC_3, AC_4, AC_5, AC_6, AC_7, AC_8, AC_9, AC_0, AC_MINUS, AC_EQUAL, AC_YEN,
    AC_Q, AC_W, AC_E, AC_R, AC_T, AC_Y, AC_U, AC_I, AC_O, AC_P, AC_LBRACKET, AC_RBRACKET,
    AC_A, AC_S, AC_D, AC_F, AC_G, AC_H, AC_J, AC_K, AC_L, AC_SCOLON, AC_QUOTE, AC_ENTER,
    AC_Z, AC_X, AC_C, AC_V, AC_B, AC_N, AC_M, AC_COMMA, AC_DOT, AC_SLASH, AC_LAYER_OFFSET_RSHIFT,
    // AC_LTHUMB_MUHENKAN, AC_RTHUMB_HENKAN,
    AC_LAYER_TAP_LTHUMB_BSPACE, AC_LAYER_TAP_RTHUMB_ENTER, AC_DELETE,
    AC_LAYER_OFFSET_LCTRL, AC_LAYER_OFFSET_LSHIFT, AC_LAYER_OFFSET_LALT, AC_LAYER_OFFSET_LGUI,
    AC_LAYER_OFFSET_RCTRL, AC_LAYER_OFFSET_RSHIFT, AC_LAYER_OFFSET_RALT, AC_LAYER_OFFSET_RGUI,
  };
  static constexpr std::array SHIFTED_QWERTY_ACTIONMAP{
    AC_TILDE, AC_EXCLAIM, AC_AT, AC_HASH, AC_DOLLAR, AC_PERCENT, AC_CIRCUMFLEX, AC_AMPERSAND, AC_ASTERISK, AC_LPAREN, AC_RPAREN, AC_UNDERSCORE, AC_PLUS, AC_PIPE,
    AC_SHIFT_Q, AC_SHIFT_W, AC_SHIFT_E, AC_SHIFT_R, AC_SHIFT_T, AC_SHIFT_Y, AC_SHIFT_U, AC_SHIFT_I, AC_SHIFT_O, AC_SHIFT_P, AC_LBRACE, AC_RBRACE,
    AC_SHIFT_A, AC_SHIFT_S, AC_SHIFT_D, AC_SHIFT_F, AC_SHIFT_G, AC_SHIFT_H, AC_SHIFT_J, AC_SHIFT_K, AC_SHIFT_L, AC_COLON, AC_DQUOTE, AC_SHIFT_ENTER,
    AC_SHIFT_Z, AC_SHIFT_X, AC_SHIFT_C, AC_SHIFT_V, AC_SHIFT_B, AC_SHIFT_N, AC_SHIFT_M, AC_LT, AC_GT, AC_QUESTION, AC_LAYER_OFFSET_RSHIFT,
    // AC_SHIFT_MUHENKAN, AC_SHIFT_HENKAN,
    AC_SHIFT_BSPACE, AC_SHIFT_ENTER, AC_SHIFT_DELETE,
    AC_LAYER_OFFSET_LCTRL, AC_LAYER_OFFSET_LSHIFT, AC_LAYER_OFFSET_LALT, AC_LAYER_OFFSET_LGUI,
    AC_LAYER_OFFSET_RCTRL, AC_LAYER_OFFSET_RSHIFT, AC_LAYER_OFFSET_RALT, AC_LAYER_OFFSET_RGUI,
  };
  static constexpr std::array QWERTY_JP_ACTIONMAP{
    AC_GRAVE, AC_1, AC_2, AC_3, AC_4, AC_5, AC_6, AC_7, AC_8, AC_9, AC_0, AC_MINUS, AC_EQUAL, AC_YEN,
    AC_Q, AC_W, AC_E, AC_R, AC_T, AC_Y, AC_U, AC_I, AC_O, AC_P, AC_LBRACKET, AC_RBRACKET,
    AC_A, AC_S, AC_D, AC_F, AC_G, AC_H, AC_J, AC_K, AC_L, AC_SCOLON, AC_QUOTE, AC_ENTER,
    AC_Z, AC_X, AC_C, AC_V, AC_B, AC_N, AC_M, AC_COMMA, AC_DOT, AC_SLASH, AC_BSLASH,
    // AC_LTHUMB_MUHENKAN, AC_RTHUMB_HENKAN,
    AC_LAYER_TAP_LTHUMB_BSPACE, AC_LAYER_TAP_RTHUMB_ENTER, AC_DELETE,
    AC_LCTRL, AC_LAYER_ON_OFF_LSHIFT_QWERTY_JP, AC_LALT, AC_LGUI,
    AC_RCTRL, AC_LAYER_ON_OFF_RSHIFT_QWERTY_JP, AC_RALT, AC_RGUI,
  };
  static constexpr std::array SHIFTED_QWERTY_JP_ACTIONMAP{
    AC_TILDE, AC_EXCLAIM, AC_AT, AC_HASH, AC_DOLLAR, AC_PERCENT, AC_CIRCUMFLEX, AC_AMPERSAND, AC_ASTERISK, AC_LPAREN, AC_RPAREN, AC_UNDERSCORE, AC_PLUS, AC_PIPE,
    AC_SHIFT_Q, AC_SHIFT_W, AC_SHIFT_E, AC_SHIFT_R, AC_SHIFT_T, AC_SHIFT_Y, AC_SHIFT_U, AC_SHIFT_I, AC_SHIFT_O, AC_SHIFT_P, AC_LBRACE, AC_RBRACE,
    AC_SHIFT_A, AC_SHIFT_S, AC_SHIFT_D, AC_SHIFT_F, AC_SHIFT_G, AC_SHIFT_H, AC_SHIFT_J, AC_SHIFT_K, AC_SHIFT_L, AC_COLON, AC_DQUOTE, AC_SHIFT_ENTER,
    AC_SHIFT_Z, AC_SHIFT_X, AC_SHIFT_C, AC_SHIFT_V, AC_SHIFT_B, AC_SHIFT_N, AC_SHIFT_M, AC_LT, AC_GT, AC_QUESTION, AC_UNDERSCORE,
    // AC_SHIFT_MUHENKAN, AC_SHIFT_HENKAN,
    AC_SHIFT_BSPACE, AC_SHIFT_ENTER, AC_SHIFT_DELETE,
    AC_LCTRL, AC_LAYER_ON_OFF_LSHIFT_QWERTY_JP, AC_LALT, AC_LGUI,
    AC_RCTRL, AC_LAYER_ON_OFF_RSHIFT_QWERTY_JP, AC_RALT, AC_RGUI,
  };
  static constexpr std::array LTHUMB_ACTIONMAP{  // WIP
    AC_HANKAKU_ZENKAKU, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO,
    AC_NO, AC_MINUS, AC_PLUS, AC_NO, AC_NO, AC_NO, AC_NO, AC_PIPE, AC_TILDE, AC_NO, AC_GRAVE, AC_NO,
    AC_LBRACKET, AC_ASTERISK, AC_LBRACE, AC_LPAREN, AC_HASH, AC_DOLLAR, AC_RPAREN, AC_RBRACE, AC_AMPERSAND, AC_RBRACKET, AC_NO, AC_NO,
    AC_NO, AC_PERCENT, AC_EXCLAIM, AC_BSLASH, AC_NO, AC_NO, AC_AT, AC_CIRCUMFLEX, AC_RIGHT_ARROW, AC_BLOCK_COMMENT_BEGIN, AC_NO,
    // AC_LTHUMB_MUHENKAN, AC_RTHUMB_HENKAN,
    AC_LAYER_TAP_LTHUMB_BSPACE, AC_LAYER_MOVE_HENKAN, AC_KATAKANA_HIRAGANA,
    AC_LCTRL, AC_LSHIFT, AC_LALT, AC_LGUI,
    AC_RCTRL, AC_RSHIFT, AC_RALT, AC_RGUI,
  };
  static constexpr std::array RTHUMB_ACTIONMAP{  // WIP
    AC_HANKAKU_ZENKAKU, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO,
    AC_NO, AC_MINUS, AC_PLUS, AC_NO, AC_NO, AC_NO, AC_NO, AC_PIPE, AC_TILDE, AC_NO, AC_GRAVE, AC_NO,
    AC_LBRACKET, AC_ASTERISK, AC_LBRACE, AC_LPAREN, AC_HASH, AC_DOLLAR, AC_RPAREN, AC_RBRACE, AC_AMPERSAND, AC_RBRACKET, AC_NO, AC_NO,
    AC_NO, AC_PERCENT, AC_EXCLAIM, AC_BSLASH, AC_NO, AC_NO, AC_AT, AC_CIRCUMFLEX, AC_RIGHT_ARROW, AC_BLOCK_COMMENT_BEGIN, AC_NO,
    // AC_LTHUMB_MUHENKAN, AC_RTHUMB_HENKAN,
    AC_LAYER_MOVE_MUHENKAN, AC_LAYER_TAP_RTHUMB_ENTER, AC_KATAKANA_HIRAGANA,
    AC_LCTRL, AC_LSHIFT, AC_LALT, AC_LGUI,
    AC_RCTRL, AC_RSHIFT, AC_RALT, AC_RGUI,
  };
  static constexpr std::array OTHERS_ACTIONMAP{
    AC_ESCAPE, AC_F1, AC_F2, AC_F3, AC_F4, AC_F5, AC_F6, AC_F7, AC_F8, AC_F9, AC_F10, AC_F11, AC_F12,
    AC_BSPACE, AC_TAB, AC_CAPSLOCK, AC_ENTER, AC_SPACE, AC_APPLICATION,

    AC_PSCREEN, AC_SCROLLLOCK, AC_PAUSE,
    AC_INSERT, AC_HOME, AC_PGUP,
    AC_DELETE, AC_END, AC_PGDOWN,

    AC_UP,
    AC_LEFT, AC_DOWN, AC_RIGHT,

    AC_NUMLOCK, AC_KP_SLASH, AC_KP_ASTERISK, AC_KP_MINUS,
    AC_KP_7, AC_KP_8, AC_KP_9,
    AC_KP_4, AC_KP_5, AC_KP_6, AC_KP_PLUS,
    AC_KP_1, AC_KP_2, AC_KP_3,
    AC_KP_0, AC_KP_DOT, AC_KP_ENTER,

    AC_AUDIO_MUTE, AC_AUDIO_VOL_DOWN, AC_AUDIO_VOL_UP, AC_MICROPHONE_MUTE, AC_MEDIA_PREV_TRACK, AC_MEDIA_PLAY_PAUSE, AC_MEDIA_NEXT_TRACK,
  };
  /* clang-format on */

  static constexpr size_t MAIN_ACTIONMAP_SIZE = COLEMAK_P_ACTIONMAP.size();
  static constexpr size_t OTHERS_ACTIONMAP_SIZE = OTHERS_ACTIONMAP.size();
  static constexpr size_t MAIN_ACTIONMAP_END = MAIN_ACTIONMAP_SIZE;
  static constexpr size_t OTHERS_ACTIONMAP_END = MAIN_ACTIONMAP_END + OTHERS_ACTIONMAP_SIZE;

  const size_t index = pos.row * MATRIX_COLS + pos.col;
  if (index < MAIN_ACTIONMAP_END) {
    switch (layer) {
      case L_COLEMAK_P:
        return COLEMAK_P_ACTIONMAP[index];
      case L_LSHIFT_COLEMAK_P:
        return SHIFTED_COLEMAK_P_ACTIONMAP[index];
      case L_RSHIFT_COLEMAK_P:
        return set_right_mods(SHIFTED_COLEMAK_P_ACTIONMAP[index]);
      case L_QWERTY:
        return QWERTY_ACTIONMAP[index];
      case L_LSHIFT_QWERTY:
        return SHIFTED_QWERTY_ACTIONMAP[index];
      case L_RSHIFT_QWERTY:
        return set_right_mods(SHIFTED_QWERTY_ACTIONMAP[index]);
      case L_QWERTY_JP:
        return QWERTY_JP_ACTIONMAP[index];
      case L_LSHIFT_QWERTY_JP:
        return SHIFTED_QWERTY_JP_ACTIONMAP[index];
      case L_RSHIFT_QWERTY_JP:
        return set_right_mods(SHIFTED_QWERTY_JP_ACTIONMAP[index]);
      case L_LTHUMB:
        return LTHUMB_ACTIONMAP[index];
      case L_RTHUMB:
        return RTHUMB_ACTIONMAP[index];
      default:
        return ACTION_NO;
    }
  } else if (index < OTHERS_ACTIONMAP_END) {
    // シフトをそのまま適用するキーを個別に扱う
    const size_t i = index - MAIN_ACTIONMAP_END;
    switch (layer) {
      case L_COLEMAK_P:
      case L_QWERTY:
      case L_QWERTY_JP:
      case L_LTHUMB:
      case L_RTHUMB:
        return OTHERS_ACTIONMAP[i];

      case L_LSHIFT_COLEMAK_P:
      case L_LSHIFT_QWERTY:
      case L_LSHIFT_QWERTY_JP:
        return add_mods(OTHERS_ACTIONMAP[i], MOD_LSFT);

      case L_RSHIFT_COLEMAK_P:
      case L_RSHIFT_QWERTY:
      case L_RSHIFT_QWERTY_JP:
        return add_mods(OTHERS_ACTIONMAP[i], MOD_RSFT);

      default:
        return ACTION_NO;
    }
  }
  return ACTION_NO;
}

const macro_t* action_get_macro(keyrecord_t* record, uint8_t id, [[maybe_unused]] uint8_t opt) {
  if (record && record->event.pressed) {
    switch (id) {
      case M_BLOCK_COMMENT_BEGIN:
        return MACRO(T(SLASH), SFT_(T(COLON), T(COLON)), END);
      case M_RIGHT_ARROW:
        return MACRO(T(MINUS), SFT_(T(DOT)), END);
    }
  }
  return nullptr;
}

void action_function(keyrecord_t* record, uint8_t id, uint8_t opt) {
  if (!record) return;
  const auto& event = record->event;
  const auto& tap = record->tap;

  if (opt & FUNC_TAP) {
    switch (id) {
      case FN_TAP_LTHUMB_MUHENKAN:
        layer_tap_thumb(event, tap, KC_MUHENKAN, L_COLEMAK_P, L_LTHUMB);
        break;
      case FN_TAP_RTHUMB_HENKAN:
        layer_tap_thumb(event, tap, KC_HENKAN, L_QWERTY_JP, L_RTHUMB);
        break;
    }
  } else {
    switch (id) {
      case FN_LAYER_OFFSET_LSHIFT:
        layer_on_off_offset(1, KC_LSHIFT, event.pressed, L_QWERTY);
        break;
      case FN_LAYER_OFFSET_RSHIFT:
        layer_on_off_offset(2, KC_RSHIFT, event.pressed, L_QWERTY);
        break;
      case FN_LAYER_OFFSET_LCTRL:
        layer_translate(KC_LCTRL, event.pressed, L_QWERTY);
        break;
      case FN_LAYER_OFFSET_LALT:
        layer_translate(KC_LALT, event.pressed, L_QWERTY);
        break;
      case FN_LAYER_OFFSET_LGUI:
        layer_translate(KC_LGUI, event.pressed, L_QWERTY);
        break;
      case FN_LAYER_OFFSET_RCTRL:
        layer_translate(KC_RCTRL, event.pressed, L_QWERTY);
        break;
      case FN_LAYER_OFFSET_RALT:
        layer_translate(KC_RALT, event.pressed, L_QWERTY);
        break;
      case FN_LAYER_OFFSET_RGUI:
        layer_translate(KC_RGUI, event.pressed, L_QWERTY);
        break;
      case FN_MICROPHONE_MUTE:
        if (event.pressed) {
#ifdef _WIN32
          // HACK: 自環境のマイクキーの入力を模倣している
          send_to_sink(NativeSinkEvent{0x2, KEYEVENTF_EXTENDEDKEY});
          send_to_sink(NativeSinkEvent{0x2, KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY});
          send_to_sink(NativeSinkEvent{0x58, KEYEVENTF_KEYUP});
#endif
        }
        break;
      case FN_LAYER_MOVE_HENKAN:
        layer_move_with_key(event, L_QWERTY_JP, L_MASK_THUMBS, KC_HENKAN);
        break;
      case FN_LAYER_MOVE_MUHENKAN:
        layer_move_with_key(event, L_COLEMAK_P, L_MASK_THUMBS, KC_MUHENKAN);
        break;
    }
  }
}
}  // extern "C"
}  // namespace tmk_desktop

#ifdef _WIN32
#include <tmk_desktop/win32/settings.hpp>

namespace tmk_desktop::inline win32 {
using namespace jp109;

const KeyToKeyposTable key_to_keypos_table = [] {
  /* clang-format off */
  const std::array KEYS{
    K_HANKAKU_ZENKAKU, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9, K_0, K_MINUS, K_CIRCUMFLEX, K_YEN,
    K_Q, K_W, K_E, K_R, K_T, K_Y, K_U, K_I, K_O, K_P, K_AT, K_LBRACKET,
    K_A, K_S, K_D, K_F, K_G, K_H, K_J, K_K, K_L, K_SCOLON, K_COLON, K_RBRACKET,
    K_Z, K_X, K_C, K_V, K_B, K_N, K_M, K_COMMA, K_DOT, K_SLASH, K_BSLASH,
    K_MUHENKAN, K_HENKAN, K_KATAKANA_HIRAGANA,
    K_LCTRL, K_LSHIFT, K_LALT, K_LGUI,
    K_RCTRL, K_RSHIFT, K_RALT, K_RGUI,

    K_ESCAPE, K_F1, K_F2, K_F3, K_F4, K_F5, K_F6, K_F7, K_F8, K_F9, K_F10, K_F11, K_F12,
    K_BSPACE, K_TAB, K_CAPSLOCK, K_ENTER, K_SPACE, K_APPLICATION,
    K_PSCREEN, K_SCROLLLOCK, K_PAUSE,
    K_INSERT, K_HOME, K_PGUP,
    K_DELETE, K_END, K_PGDOWN,
    K_UP,
    K_LEFT, K_DOWN, K_RIGHT,
    K_NUMLOCK, K_KP_SLASH, K_KP_ASTERISK, K_KP_MINUS,
    K_KP_7, K_KP_8, K_KP_9,
    K_KP_4, K_KP_5, K_KP_6, K_KP_PLUS,
    K_KP_1, K_KP_2, K_KP_3,
    K_KP_0, K_KP_DOT, K_KP_ENTER,

    // Key{0x102}は自環境でのマイクミュートキーのコード
    K_AUDIO_MUTE, K_AUDIO_VOL_DOWN, K_AUDIO_VOL_UP, Key{0x102}, K_MEDIA_PREV_TRACK, K_MEDIA_PLAY_PAUSE, K_MEDIA_NEXT_TRACK,
  };
  /* clang-format on */

  KeyToKeyposTable t{};
  std::fill(t.begin(), t.end(), keypos_t{0xff, 0xff});
  for (size_t i = 0; auto key : KEYS) {
    t[key] = keypos_t{static_cast<uint8_t>(i % MATRIX_COLS), static_cast<uint8_t>(i / MATRIX_COLS)};
    i++;
  }
  return t;
}();

const TappingKeyTable tapping_key_table = [] {
  const std::array KEYS{K_HANKAKU_ZENKAKU, K_CAPSLOCK, K_KATAKANA_HIRAGANA};
  TappingKeyTable t{};
  for (auto key : KEYS) {
    t[key] = true;
  }
  return t;
}();

// const KeycodeToScancodeTable keycode_to_scancode_table{};
}  // namespace tmk_desktop::inline win32
#endif
