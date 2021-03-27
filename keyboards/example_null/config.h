#pragma once

/**
 * @brief キーボードの状態を格納する2次元配列の列数 (1~32)
 *
 * matrix_row_tの型を決定するのに使われる。
 */
#define MATRIX_COLS 1

/**
 * @brief キーボードの状態を格納する2次元配列の行数
 *
 * matrix_row_tの配列の大きさを決定するのに使われる。
 */
#define MATRIX_ROWS 1

/**
 * @brief 既定のmatrix関数を定義しない
 */
// #define TMK_DESKTOP_NOIMPL_MATRIX

/**
 * @brief 既定のスキャンコード表を定義しない
 */
// #define TMK_DESKTOP_NOIMPL_KEYCODE_TO_SCANCODE_TABLE
