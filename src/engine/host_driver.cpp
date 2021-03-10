/**
 * @file host_driver.cpp
 * @brief ホストドライバの管理
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <tmk_desktop/engine/host_driver.hpp>

extern "C" {
#include <tmk_core/common/host.h>
}  // extern "C"

namespace tmk_desktop::engine::inline v1 {
namespace {
HostDriver driver_{};
}  // namespace

void set_host_driver(const HostDriver& driver) noexcept {
  driver_ = driver;
  driver_.validate();
  host_set_driver(&driver_);
}

void reset_host_driver() noexcept {
  host_set_driver(nullptr);
  driver_ = {};
}

void send_keyboard_event(const report_keyboard_t& report) noexcept {
  if (driver_.send_keyboard) {
    auto r = report;
    driver_.send_keyboard(&r);
  }
}

void send_mouse_event(const report_mouse_t& report) noexcept {
  if (driver_.send_mouse) {
    auto r = report;
    driver_.send_mouse(&r);
  }
}

void send_system_event(uint16_t id) noexcept {
  if (driver_.send_system) driver_.send_system(id);
}

void send_consumer_event(uint16_t id) noexcept {
  if (driver_.send_consumer) driver_.send_consumer(id);
}
}  // namespace tmk_desktop::engine::inline v1
