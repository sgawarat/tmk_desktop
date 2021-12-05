/**
 * @file config.h
 * @brief config
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

// 日本語配列(109) + 音量操作(3) + メディア操作(3) + マイク操作(1)
#define MATRIX_SIZE (109 + 7)

// 取り扱うキーの数
#define MATRIX_COLS (32)
#define MATRIX_ROWS ((MATRIX_SIZE + MATRIX_COLS - 1) / MATRIX_COLS)
