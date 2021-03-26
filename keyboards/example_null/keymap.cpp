#include <cstdint>

extern "C" {
#include <common/action_code.h>

/**
 * @brief アクションマップ
 * 
 * 既定ではaction_for_key()の中で参照される。
 */
extern const action_t actionmaps[/* layers */][MATRIX_ROWS][MATRIX_COLS] = {};
}  // extern "C"

// Win32アプリケーションに必要なもの
#ifdef _WIN32
#include <tmk_desktop/win32/settings.hpp>

namespace tmk_desktop::inline win32 {

/**
 * @brief キーからkeypos_tへの変換表
 */
const KeyToKeyposTable key_to_keypos_table{};

/**
 * @brief 押すと同時に離すキーを示すフラグ列
 */
const TappingKeyTable tapping_key_table{};

/**
 * @brief キーコードからスキャンコードへの変換表
 * 
 * デフォルト値が用意されるが、独自の対応表が欲しければここで定義する。
 */
// const KeycodeToScancodeTable keycode_to_scancode_table{};
}  // namespace tmk_desktop::win32
#endif
