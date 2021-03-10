/**
 * @file settings_example.cpp
 * @brief 設定例
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include "../settings.hpp"
#include <tmk_desktop/action_map.hpp>
#include <tmk_desktop/engine/mapping.hpp>
#include "../key.hpp"

// 日本語キーボードにおける新規のキー
#define TMK_DESKTOP_WIN32_JP_NEW_KEYS(m) \
    m(HANKAKU_ZENKAKU, SHIFT_HANKAKU_ZENKAKU, GRAVE) \
    m(CIRCUMFLEX, TILDE, EQUAL) \
    m(YEN, VERTICAL_BAR, INT3) \
    m(AT_SIGN, GRAVE, LBRACKET) \
    m(LBRACKET, LBRACE, RBRACKET) \
    m(COLON, ASTERISK, QUOTE) \
    m(RBRACKET, RBRACE, BSLASH) \
    m(BSLASH, UNDERSCORE, INT1) \
    m(MUHENKAN, SHIFT_MUHENKAN, INT5) \
    m(HENKAN, SHIFT_HENKAN, INT4) \
    m(KATAKANA_HIRAGANA, SHIFT_KATAKANA_HIRAGANA, INT2)

// 日本語キーボードにおける新規のアクション
#define TMK_DESKTOP_WIN32_JP_NEW_SHIFT_ACTIONS(m) \
    m(DOUBLE_QUOTE, 2) \
    m(AMPERSAND, 6) \
    m(QUOTE, 7) \
    m(LPAREN, 8) \
    m(RPAREN, 9) \
    m(EQUAL, MINUS) \
    m(PLUS, SCOLON)

namespace tmk_desktop::win32 {
namespace {
using namespace tmk_desktop::engine;

// 日本語キーボードに合わせてキーを再定義する
#define M(name, shift_name, keycode) \
    [[maybe_unused]] constexpr Key K_##name{win32::K_##keycode}; \
    [[maybe_unused]] constexpr uint8_t KC_##name{::KC_##keycode}; \
    [[maybe_unused]] constexpr action_t AC_##name = ACTION_KEY(::KC_##keycode); \
    [[maybe_unused]] constexpr action_t AC_##shift_name = ACTION_MODS_KEY(MOD_LSFT, ::KC_##keycode);
TMK_DESKTOP_WIN32_JP_NEW_KEYS(M)
#undef M
#define M(shift_name, keycode) \
    [[maybe_unused]] constexpr action_t AC_##shift_name = ACTION_MODS_KEY(MOD_LSFT, ::KC_##keycode);
TMK_DESKTOP_WIN32_JP_NEW_SHIFT_ACTIONS(M)
#undef M

// アクション
constexpr action_t AC_COLON_COLON = ACTION_MACRO(0);
constexpr action_t AC_EQUAL_EQUAL = ACTION_MACRO(1);
constexpr action_t AC_BLOCK_COMMENT_BEGIN = ACTION_MACRO(2);
constexpr action_t AC_RIGHT_ARROW = ACTION_MACRO(3);
constexpr action_t AC_LTHUMB = ACTION_LAYER_TAP(9, KC_MUHENKAN);
constexpr action_t AC_RTHUMB = ACTION_LAYER_TAP(10, KC_HENKAN);
constexpr action_t AC_MUTE_MIC = ACTION_FUNCTION(FN_MUTE_MIC);

// マクロ
using Macro = std::array<macro_t, 32>;
constexpr std::array MACRO_MAP{
  /* :: */ Macro{TYPE(KC_COLON), TYPE(KC_COLON), END},
  /* == */ Macro{DOWN(KC_LSHIFT), TYPE(KC_MINUS), TYPE(KC_MINUS), UP(KC_LSHIFT), END},
  /* / ** */ Macro{TYPE(KC_SLASH), DOWN(KC_LSHIFT), TYPE(KC_COLON), TYPE(KC_COLON), UP(KC_LSHIFT), END},
  /* -> */ Macro{TYPE(KC_MINUS), DOWN(KC_LSHIFT), TYPE(KC_DOT), UP(KC_LSHIFT), END},
};

/**
 * @brief 自前のアクションマップクラス
 */
struct MyActionMap : ActionMap {
  static constexpr std::array KEYS{
    K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9, K_0, K_MINUS, K_CIRCUMFLEX, K_YEN,
    K_Q, K_W, K_E, K_R, K_T, K_Y, K_U, K_I, K_O, K_P, K_AT_SIGN, K_LBRACKET,
    K_A, K_S, K_D, K_F, K_G, K_H, K_J, K_K, K_L, K_SCOLON, K_COLON, K_RBRACKET,
    K_Z, K_X, K_C, K_V, K_B, K_N, K_M, K_COMMA, K_DOT, K_SLASH, K_BSLASH,
    K_HANKAKU_ZENKAKU, K_MUHENKAN, K_HENKAN,
  };

  static constexpr std::array MOD_KEYS{
    K_LCTRL, K_LSHIFT, K_LALT, K_LGUI,
    K_RCTRL, K_RSHIFT, K_RALT, K_RGUI,
  };

  // キーをそのままアクションにする
  MyActionMap& passthrough(uint16_t mods = 0) noexcept {
    update([&](Key key) -> action_t {return to_action(key, mods);});

    // HACK: 自前環境でのマイクミュートキーのスキャンコードを使っている
    set(Key{0x102}, AC_MUTE_MIC);
    return *this;
  }

  // すべて
  constexpr MyActionMap& all(action_t action) noexcept {
    update([&](Key) constexpr -> action_t {return action;});
    return *this;
  }

  // キー（個別）
  constexpr MyActionMap& keys(std::span<const action_t> actions) noexcept {
    update(KEYS, [&](size_t i, Key) constexpr -> action_t {
      if (i >= actions.size()) return AC_NO;
      return actions[i];
    });
    return *this;
  }

  // キー（個別）
  constexpr MyActionMap& keys(std::initializer_list<action_t> actions) noexcept {
    return keys(std::span{actions.begin(), actions.end()});
  }

  // キー（修飾キー調整あり）
  constexpr MyActionMap& keys(std::span<const action_t> actions, uint16_t mods) noexcept {
    update(KEYS, [&](size_t i, Key) constexpr -> action_t {
      if (i >= actions.size()) return AC_NO;
      auto action = actions[i];
      switch (action.kind.id) {
        case ACT_LMODS:
        case ACT_RMODS:
          action.code |= mods & 0x1f00;
          break;
        default: break;
      }
      return action;
    });
    return *this;
  }

  // キー（修飾キー調整あり）
  constexpr MyActionMap& keys(std::initializer_list<action_t> actions, uint16_t mods) noexcept {
    return keys(std::span{actions.begin(), actions.end()}, mods);
  }

  // 修飾キー（一括）
  constexpr MyActionMap& mods(action_t action) noexcept {
    update(MOD_KEYS, [&](size_t, Key) constexpr -> action_t {return action;});
    return *this;
  }

  // 修飾キー（個別）
  constexpr MyActionMap& mods(std::span<const action_t> actions) noexcept {
    update(MOD_KEYS, [&](size_t i, Key) constexpr -> action_t {
      if (i >= actions.size()) return AC_NO;
      return actions[i];
    });
    return *this;
  }

  // 修飾キー（個別）
  constexpr MyActionMap& mods(std::initializer_list<action_t> actions) noexcept {
    return mods(std::span{actions.begin(), actions.end()});
  }
};

// キー配列
constexpr std::array COLEMAK_P_ACTIONS{
  AC_1, AC_2, AC_3, AC_4, AC_5, AC_6, AC_7, AC_8, AC_9, AC_0, AC_LBRACKET, AC_RBRACKET, AC_YEN,
  AC_Q, AC_W, AC_F, AC_P, AC_G, AC_J, AC_L, AC_U, AC_Y, AC_SCOLON, AC_EQUAL, AC_QUOTE,
  AC_A, AC_R, AC_S, AC_T, AC_D, AC_H, AC_N, AC_E, AC_I, AC_O, AC_UNDERSCORE, AC_ENTER,
  AC_Z, AC_X, AC_C, AC_V, AC_B, AC_K, AC_M, AC_COMMA, AC_DOT, AC_SLASH, AC_BSLASH,
  AC_GRAVE, AC_LTHUMB, AC_RTHUMB,
};
constexpr std::array COLEMAK_P_SHIFT_ACTIONS{
  AC_EXCLAMATION, AC_AT_SIGN, AC_HASH, AC_DOLLAR, AC_PERCENT, AC_CIRCUMFLEX, AC_AMPERSAND, AC_ASTERISK, AC_LPAREN, AC_RPAREN, AC_LBRACE, AC_RBRACE, AC_VERTICAL_BAR,
  AC_SHIFT_Q, AC_SHIFT_W, AC_SHIFT_F, AC_SHIFT_P, AC_SHIFT_G, AC_SHIFT_J, AC_SHIFT_L, AC_SHIFT_U, AC_SHIFT_Y, AC_COLON, AC_PLUS, AC_DOUBLE_QUOTE,
  AC_SHIFT_A, AC_SHIFT_R, AC_SHIFT_S, AC_SHIFT_T, AC_SHIFT_D, AC_SHIFT_H, AC_SHIFT_N, AC_SHIFT_E, AC_SHIFT_I, AC_SHIFT_O, AC_MINUS, AC_SHIFT_ENTER,
  AC_SHIFT_Z, AC_SHIFT_X, AC_SHIFT_C, AC_SHIFT_V, AC_SHIFT_B, AC_SHIFT_K, AC_SHIFT_M, AC_LESS_THAN, AC_GREATER_THAN, AC_QUESTION, AC_UNDERSCORE,
  AC_TILDE, AC_SHIFT_MUHENKAN, AC_SHIFT_HENKAN,
};
constexpr std::array QWERTY_ACTIONS{
  AC_1, AC_2, AC_3, AC_4, AC_5, AC_6, AC_7, AC_8, AC_9, AC_0, AC_MINUS, AC_EQUAL, AC_YEN,
  AC_Q, AC_W, AC_E, AC_R, AC_T, AC_Y, AC_U, AC_I, AC_O, AC_P, AC_LBRACKET, AC_RBRACKET,
  AC_A, AC_S, AC_D, AC_F, AC_G, AC_H, AC_J, AC_K, AC_L, AC_SCOLON, AC_QUOTE, AC_ENTER,
  AC_Z, AC_X, AC_C, AC_V, AC_B, AC_N, AC_M, AC_COMMA, AC_DOT, AC_SLASH, AC_BSLASH,
  AC_GRAVE, AC_MUHENKAN, AC_HENKAN,
};
constexpr std::array QWERTY_SHIFT_ACTIONS{
  AC_EXCLAMATION, AC_AT_SIGN, AC_HASH, AC_DOLLAR, AC_PERCENT, AC_CIRCUMFLEX, AC_AMPERSAND, AC_ASTERISK, AC_LPAREN, AC_RPAREN, AC_UNDERSCORE, AC_PLUS, AC_VERTICAL_BAR,
  AC_SHIFT_Q, AC_SHIFT_W, AC_SHIFT_E, AC_SHIFT_R, AC_SHIFT_T, AC_SHIFT_Y, AC_SHIFT_U, AC_SHIFT_I, AC_SHIFT_O, AC_SHIFT_P, AC_LBRACE, AC_RBRACE,
  AC_SHIFT_A, AC_SHIFT_S, AC_SHIFT_D, AC_SHIFT_F, AC_SHIFT_G, AC_SHIFT_H, AC_SHIFT_J, AC_SHIFT_K, AC_SHIFT_L, AC_COLON, AC_DOUBLE_QUOTE, AC_SHIFT_ENTER,
  AC_SHIFT_Z, AC_SHIFT_X, AC_SHIFT_C, AC_SHIFT_V, AC_SHIFT_B, AC_SHIFT_N, AC_SHIFT_M, AC_LESS_THAN, AC_GREATER_THAN, AC_QUESTION, AC_UNDERSCORE,
  AC_TILDE, AC_SHIFT_MUHENKAN, AC_SHIFT_HENKAN,
};
constexpr std::array THUMB_ACTIONS{
  AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO, AC_NO,
  AC_NO, AC_LBRACKET, AC_EXCLAMATION, AC_NO, AC_NO, AC_NO, AC_NO, AC_VERTICAL_BAR, AC_RBRACKET, AC_COLON_COLON, AC_EQUAL_EQUAL, AC_NO,
  AC_DOUBLE_QUOTE, AC_ASTERISK, AC_LBRACE, AC_LPAREN, AC_HASH, AC_DOLLAR, AC_RPAREN, AC_RBRACE, AC_AMPERSAND, AC_QUOTE, AC_TILDE, AC_NO,
  AC_NO, AC_GRAVE, AC_PERCENT, AC_BSLASH, AC_NO, AC_NO, AC_AT_SIGN, AC_CIRCUMFLEX, AC_RIGHT_ARROW, AC_BLOCK_COMMENT_BEGIN, AC_NO,
  AC_HANKAKU_ZENKAKU, AC_TRANSPARENT, AC_TRANSPARENT,
};

// アクションマップ
static std::array ACTION_MAP{
  // 0: Colemak (調整あり)
  MyActionMap{}
      .passthrough()
      .keys(COLEMAK_P_ACTIONS)
      .mods({
        ACTION_FUNCTION_OPT(FN_LAYER_TRANSLATE_LCTRL, 3),
        ACTION_FUNCTION_OPT(FN_LAYER_ON_OFF_OFFSET_LSHIFT, 3),
        ACTION_FUNCTION_OPT(FN_LAYER_TRANSLATE_LALT, 3),
        ACTION_FUNCTION_OPT(FN_LAYER_TRANSLATE_LGUI, 3),
        ACTION_FUNCTION_OPT(FN_LAYER_TRANSLATE_RCTRL, 3),
        ACTION_FUNCTION_OPT(FN_LAYER_ON_OFF_OFFSET_RSHIFT, 3),
        ACTION_FUNCTION_OPT(FN_LAYER_TRANSLATE_RALT, 3),
        ACTION_FUNCTION_OPT(FN_LAYER_TRANSLATE_RGUI, 3),
      }),
  // 1: 左シフト付きColemak (調整あり)
  MyActionMap{}
      .passthrough(MOD_LSFT)
      .keys(COLEMAK_P_SHIFT_ACTIONS)
      .mods(AC_TRANSPARENT),
  // 2: 右シフト付きColemak (調整あり)
  MyActionMap{}
      .passthrough(MOD_RSFT)
      .keys(COLEMAK_P_SHIFT_ACTIONS, MOD_RIGHT_BIT)
      .mods(AC_TRANSPARENT),
  // 3: US式QWERTY (CTRL,ALT,GUI用)
  MyActionMap{}
      .passthrough()
      .keys(QWERTY_ACTIONS)
      .mods(AC_TRANSPARENT),
  // 4: 左シフト付きUS式QWERTY (CTRL,ALT,GUI用)
  MyActionMap{}
      .passthrough(MOD_LSFT)
      .keys(QWERTY_SHIFT_ACTIONS)
      .mods(AC_TRANSPARENT),
  // 5: 右シフト付きUS式QWERTY (CTRL,ALT,GUI用)
  MyActionMap{}
      .passthrough(MOD_RSFT)
      .keys(QWERTY_SHIFT_ACTIONS, MOD_RIGHT_BIT)
      .mods(AC_TRANSPARENT),
  // 6: US式QWERTY (日本語入力用)
  MyActionMap{}
      .passthrough()
      .keys(QWERTY_ACTIONS)
      .mods({
        AC_LCTRL,
        ACTION_LAYER_ON_OFF(7),
        AC_LALT,
        AC_LGUI,
        AC_RCTRL,
        ACTION_LAYER_ON_OFF(8),
        AC_RALT,
        AC_RGUI,
      }),
  // 7: 左シフト付きUS式QWERTY (日本語入力用)
  MyActionMap{}
      .passthrough(MOD_LSFT)
      .keys(QWERTY_SHIFT_ACTIONS)
      .mods(AC_TRANSPARENT),
  // 8: 右シフト付きUS式QWERTY (日本語入力用)
  MyActionMap{}
      .passthrough(MOD_RSFT)
      .keys(QWERTY_SHIFT_ACTIONS, MOD_RIGHT_BIT)
      .mods(AC_TRANSPARENT),
  // 9: 左親指
  MyActionMap{}
      .all(AC_NO)
      .keys(THUMB_ACTIONS)
      .mods(AC_NO),
  // 9: 右親指
  MyActionMap{}
      .all(AC_NO)
      .keys(THUMB_ACTIONS)
      .mods(AC_NO),
};
}  // namespace

GlobalSettingsInfo get_global_settings_info() noexcept {
  return {
    .layout_for_input_method = 6,  // QWERTYで日本語入力を行う
  };
}

KeyBehaviorMapInfo get_key_behavior_map_info() noexcept {
  // 離したときにreleaseイベントが来ないキーたち
  static constexpr std::array PRESS_ONLY_KEYS{
    K_HANKAKU_ZENKAKU,  // 半角全角
    K_CAPSLOCK,  // Caps Lock
    K_KATAKANA_HIRAGANA,  // カタカナひらがな
  };
  static constexpr std::array<KeyBehaviorMapInfo::Entry, 1> ENTRIES{{
    {KeyBehavior::PRESS_ONLY, PRESS_ONLY_KEYS},
  }};
  return {ENTRIES};
}

ActionMapInfo get_action_map_info() noexcept {
  static auto ENTRIES = [] {
    std::array<ActionMapInfo::Entry, ACTION_MAP.size()> entries{};
    size_t i = 0;
    for (const auto& m : ACTION_MAP) {
      entries[i] = ActionMapInfo::Entry{static_cast<uint8_t>(i), m};
      i++;
    }
    return entries;
  }();
  return {ENTRIES};
}

MacroMapInfo get_macro_map_info() noexcept {
  static auto ENTRIES = [] {
    std::array<MacroMapInfo::Entry, MACRO_MAP.size()> entries{};
    size_t i = 0;
    for (const auto& m : MACRO_MAP) {
      entries[i] = MacroMapInfo::Entry{static_cast<uint8_t>(i), m};
      i++;
    }
    return entries;
  }();
  return {ENTRIES};
}
}  // namespace tmk_desktop::win32
