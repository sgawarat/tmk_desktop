/**
 * @file wait.cpp
 * @brief 待機関数
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <thread>

extern "C" {
void wait_ms(uintptr_t ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void wait_us(uintptr_t us) {
  std::this_thread::sleep_for(std::chrono::microseconds(us));
}
}  // extern "C"
