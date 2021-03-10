/**
 * @file config.h
 * @brief TMKの設定
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <stdint.h>

// キー状態を格納する領域のビット数を指定するマクロ
#ifndef MATRIX_SIZE
  #ifdef _WIN32
    #define MATRIX_SIZE 512
  #endif
#endif

// キー状態を格納する二次元ビットセットの大きさを指定するマクロ
#if defined(MATRIX_SIZE) && !defined(MATRIX_ROWS) && !defined(MATRIX_COLS)
  #if MATRIX_SIZE <= 32
    #define MATRIX_ROWS 1
    #define MATRIX_COLS MATRIX_SIZE
  #else
    #define MATRIX_ROWS ((MATRIX_SIZE + 31) / 32)
    #define MATRIX_COLS 32
  #endif
#endif

/**
 * @brief キー状態を報告するための領域のバイト数を指定するマクロ
 */
#define KEYBOARD_REPORT_BITS 32

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief NKRO_ENABLEが有効の場合にすべてのキー状態を記録させるためのフラグ
 */
extern uint8_t keyboard_protocol;

/**
 * @brief ミリ秒単位で待機する
 */
extern void wait_ms(int ms);

// 非組込系で不足するメモリ関連のマクロ
#define PROGMEM
#define pgm_read_byte(p) *((unsigned char*)p)
#define pgm_read_word(p) *((uint16_t*)p)

#ifdef __cplusplus
}  // extern "C"
#endif

#ifdef _WIN32
#include <Windows.h>

// TMKと干渉するマクロを取り除く
#undef IS_ERROR
#undef DELETE
#undef OUT
#endif
