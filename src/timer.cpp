/**
 * @file timer.cpp
 * @brief タイマー関数
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <chrono>
#include <cstdint>

extern "C" {
#include <common/timer.h>
}  // extern "C"

namespace {
using Clock = std::chrono::high_resolution_clock;

Clock::time_point zero_tp_{};  // 始点となるtime_point

template <typename T>
inline auto get_elapsed_time() noexcept {
  return std::chrono::duration_cast<std::chrono::duration<T, std::milli>>(Clock::now() - zero_tp_);
}
}  // namespace

extern "C" {
void timer_init() {
  zero_tp_ = Clock::now();
}

void timer_clear() {
  zero_tp_ = Clock::now();
}

uint16_t timer_read() {
  return get_elapsed_time<uint16_t>().count();
}

uint32_t timer_read32() {
  return get_elapsed_time<uint32_t>().count();
}

uint16_t timer_elapsed(uint16_t last) {
  return get_elapsed_time<uint16_t>().count() - last;
}

uint32_t timer_elapsed32(uint32_t last) {
  return get_elapsed_time<uint32_t>().count() - last;
}
}  // extern "C"
