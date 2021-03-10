/**
 * @file host_driver.hpp
 * @brief ホストドライバ
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

extern "C" {
#include <tmk_core/common/host_driver.h>
}  // extern "C"

namespace tmk_desktop::engine::inline v1 {
class HostDriver final : public host_driver_t {
public:
  HostDriver() noexcept
    : host_driver_t{
      .keyboard_leds = default_keyboard_leds,
      .send_keyboard = default_send_keyboard,
      .send_mouse = default_send_mouse,
      .send_system = default_send_system,
      .send_consumer = default_send_consumer,
    } {}

  HostDriver(const host_driver_t& driver) noexcept
    : host_driver_t(driver) {}

  /**
   * @brief 各ポインタのnullptrを何もしない関数に置き換える
   */
  void validate() noexcept {
    if (!keyboard_leds) keyboard_leds = default_keyboard_leds;
    if (!send_keyboard) send_keyboard = default_send_keyboard;
    if (!send_mouse) send_mouse = default_send_mouse;
    if (!send_system) send_system = default_send_system;
    if (!send_consumer) send_consumer = default_send_consumer;
  }

private:
  // 何もしない関数
  static uint8_t default_keyboard_leds() {return 0;}
  static void default_send_keyboard(report_keyboard_t*) {}
  static void default_send_mouse(report_mouse_t*) {}
  static void default_send_system(uint16_t) {}
  static void default_send_consumer(uint16_t) {}
};

// HACK: 独自処理をねじ込むために未使用の値を使って定義する
constexpr uint16_t CSMR_MICROPHONE_MUTE = 0x04e2;

void set_host_driver(const HostDriver& driver) noexcept;

void reset_host_driver() noexcept;

void send_keyboard_event(const report_keyboard_t& report) noexcept;

void send_mouse_event(const report_mouse_t& report) noexcept;

void send_system_event(uint16_t id) noexcept;

void send_consumer_event(uint16_t id) noexcept;
}  // namespace tmk_desktop::engine::inline v1
