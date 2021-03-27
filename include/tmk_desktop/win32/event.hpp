/**
 * @file event.hpp
 * @brief Win32のキーイベント
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#pragma once

#include <cstdint>
#include <Windows.h>

namespace tmk_desktop::inline win32 {
/**
 * @brief キーを表す値の型
 */
using Key = uint16_t;

/**
 * @brief キーの個数
 */
static constexpr size_t KEY_COUNT = 0x200;

/**
 * @brief キーイベントを格納するクラス
 */
class KeyEvent final {
public:
  KeyEvent() = default;

  KeyEvent(const KBDLLHOOKSTRUCT& info) noexcept : vk_(info.vkCode), sc_(info.scanCode), flags_(info.flags) {}

  Key key() const noexcept {
    // HACK: 8ビットより大きなスキャンコードが現れないことを前提としている
    return ((flags_ & LLKHF_EXTENDED) ? 0x100 : 0) | (sc_ & 0xff);
  }

  bool is_pressed() const noexcept {
    return !(flags_ & LLKHF_UP);
  }

private:
  [[maybe_unused]] WORD vk_ = 0;
  WORD sc_ = 0;
  DWORD flags_ = 0;
};

/**
 * @brief Sinkで使うOSネイティブなイベント
 */
struct NativeSinkEvent : INPUT {
  constexpr explicit NativeSinkEvent(WORD sc, DWORD flags) noexcept
      : INPUT{
            .type = INPUT_KEYBOARD,
            .ki =
                {
                    .wScan = sc,
                    .dwFlags = KEYEVENTF_SCANCODE | flags,
                },
        } {}
};
}  // namespace tmk_desktop::inline win32
