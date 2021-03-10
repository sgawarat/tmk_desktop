/**
 * @file misc.cpp
 * @brief TMKが必要とするその他の関数
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <chrono>
#include <thread>
#include <cstdint>

extern "C" {
#include <tmk_core/common/timer.h>
}  // extern "C"

namespace tmk_desktop::engine::inline v1 {
namespace {
using Clock = std::chrono::high_resolution_clock;
using Milliseconds16 = std::chrono::duration<uint16_t, std::milli>;
using Milliseconds32 = std::chrono::duration<uint32_t, std::milli>;

Clock::time_point zero_tp_{};  // 始点となるtime_point
}  // namespace

extern "C" {
void timer_init() {
  zero_tp_ = Clock::now();
}

void timer_clear() {
  zero_tp_ = Clock::now();
}

uint16_t timer_read() {
  return std::chrono::duration_cast<Milliseconds16>(Clock::now() - zero_tp_).count();
}

uint32_t timer_read32() {
  return std::chrono::duration_cast<Milliseconds32>(Clock::now() - zero_tp_).count();
}

uint16_t timer_elapsed(uint16_t last) {
  return std::chrono::duration_cast<Milliseconds16>(Clock::now() - zero_tp_).count() - last;
}

uint32_t timer_elapsed32(uint32_t last) {
  return std::chrono::duration_cast<Milliseconds32>(Clock::now() - zero_tp_).count() - last;
}

// void led_set([[maybe_unused]] uint8_t usb_led) {}

void bootloader_jump() {}

void wait_ms(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
}  // extern "C"
}  // namespace tmk_desktop::engine::inline v1
