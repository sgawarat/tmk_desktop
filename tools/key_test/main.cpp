/**
 * @file main.cpp
 * @brief キーの値を確認するためのアプリケーション
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <string_view>
#include <iostream>
#include <Windows.h>
#include <tmk_desktop/win32/event.hpp>

namespace tmk_desktop {
static LRESULT CALLBACK hook_proc(int code, WPARAM wparam, LPARAM lparam) noexcept {
  switch (code) {
    case HC_ACTION: {
      KBDLLHOOKSTRUCT* info_ptr = reinterpret_cast<KBDLLHOOKSTRUCT*>(lparam);
      if (!info_ptr) break;

      // 押したキーの番号を表示する
      const auto event = KeyEvent{*info_ptr};
      if (event.is_pressed()) {
        std::cout << "Press Key 0x" << std::hex << event.key() << '\n';
      } else {
        std::cout << "Release Key 0x" << std::hex << event.key() << '\n';
      }

      return TRUE;
    }
  }

  return CallNextHookEx(NULL, code, wparam, lparam);
}

std::string_view get_vk_name(UINT vk) noexcept;

extern "C" int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE, LPWSTR, int) {
  AllocConsole();
  [[maybe_unused]] const auto conout_fp = freopen("CONOUT$", "w", stdout);
  SetWindowPos(GetConsoleWindow(), NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  SetConsoleOutputCP(CP_UTF8);

  // 有効なスキャンコードを列挙する
  for (uint32_t sc = 0x0000; sc <= 0xffff; ++sc) {
    const UINT vk = MapVirtualKey(sc, MAPVK_VSC_TO_VK_EX);
    if (vk > 0) std::cout << "Scancode 0x" << std::hex << sc << " = " << get_vk_name(vk) << " (" << vk << ")\n";
  }

  // 押したキーの番号を表示するため、キーイベントを覗き見る
  const HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, hook_proc, instance, 0);
  MSG msg{};
  while (true) {
    const auto result = GetMessage(&msg, NULL, 0, 0);
    if (result == 0) break;  // WM_QUITを受け取った
    if (result > 0) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  UnhookWindowsHookEx(hook);

  system("PAUSE");
  return static_cast<int>(msg.wParam);
}

std::string_view get_vk_name(UINT vk) noexcept {
  switch (vk) {
    case VK_LBUTTON: return "VK_LBUTTON";
    case VK_RBUTTON: return "VK_RBUTTON";
    case VK_CANCEL: return "VK_CANCEL";
    case VK_MBUTTON: return "VK_MBUTTON";
    case VK_XBUTTON1: return "VK_XBUTTON1";
    case VK_XBUTTON2: return "VK_XBUTTON2";
    case VK_BACK: return "VK_BACK";
    case VK_TAB: return "VK_TAB";
    case VK_CLEAR: return "VK_CLEAR";
    case VK_RETURN: return "VK_RETURN";
    case VK_SHIFT: return "VK_SHIFT";
    case VK_CONTROL: return "VK_CONTROL";
    case VK_MENU: return "VK_MENU";
    case VK_PAUSE: return "VK_PAUSE";
    case VK_CAPITAL: return "VK_CAPITAL";
    case VK_KANA: return "VK_KANA";
    // case VK_HANGUL: return "VK_HANGUL";
    case VK_IME_ON: return "VK_IME_ON";
    case VK_JUNJA: return "VK_JUNJA";
    case VK_FINAL: return "VK_FINAL";
    // case VK_HANJA: return "VK_HANJA";
    case VK_KANJI: return "VK_KANJI";
    case VK_IME_OFF: return "VK_IME_OFF";
    case VK_ESCAPE: return "VK_ESCAPE";
    case VK_CONVERT: return "VK_CONVERT";
    case VK_NONCONVERT: return "VK_NONCONVERT";
    case VK_ACCEPT: return "VK_ACCEPT";
    case VK_MODECHANGE: return "VK_MODECHANGE";
    case VK_SPACE: return "VK_SPACE";
    case VK_PRIOR: return "VK_PRIOR";
    case VK_NEXT: return "VK_NEXT";
    case VK_END: return "VK_END";
    case VK_HOME: return "VK_HOME";
    case VK_LEFT: return "VK_LEFT";
    case VK_UP: return "VK_UP";
    case VK_RIGHT: return "VK_RIGHT";
    case VK_DOWN: return "VK_DOWN";
    case VK_SELECT: return "VK_SELECT";
    case VK_PRINT: return "VK_PRINT";
    case VK_EXECUTE: return "VK_EXECUTE";
    case VK_SNAPSHOT: return "VK_SNAPSHOT";
    case VK_INSERT: return "VK_INSERT";
    case VK_DELETE: return "VK_DELETE";
    case VK_HELP: return "VK_HELP";
    case '0': return "0";
    case '1': return "1";
    case '2': return "2";
    case '3': return "3";
    case '4': return "4";
    case '5': return "5";
    case '6': return "6";
    case '7': return "7";
    case '8': return "8";
    case '9': return "9";
    case 'A': return "A";
    case 'B': return "B";
    case 'C': return "C";
    case 'D': return "D";
    case 'E': return "E";
    case 'F': return "F";
    case 'G': return "G";
    case 'H': return "H";
    case 'I': return "I";
    case 'J': return "J";
    case 'K': return "K";
    case 'L': return "L";
    case 'M': return "M";
    case 'N': return "N";
    case 'O': return "O";
    case 'P': return "P";
    case 'Q': return "Q";
    case 'R': return "R";
    case 'S': return "S";
    case 'T': return "T";
    case 'U': return "U";
    case 'V': return "V";
    case 'W': return "W";
    case 'X': return "X";
    case 'Y': return "Y";
    case 'Z': return "Z";
    case VK_LWIN: return "VK_LWIN";
    case VK_RWIN: return "VK_RWIN";
    case VK_APPS: return "VK_APPS";
    case VK_SLEEP: return "VK_SLEEP";
    case VK_NUMPAD0: return "VK_NUMPAD0";
    case VK_NUMPAD1: return "VK_NUMPAD1";
    case VK_NUMPAD2: return "VK_NUMPAD2";
    case VK_NUMPAD3: return "VK_NUMPAD3";
    case VK_NUMPAD4: return "VK_NUMPAD4";
    case VK_NUMPAD5: return "VK_NUMPAD5";
    case VK_NUMPAD6: return "VK_NUMPAD6";
    case VK_NUMPAD7: return "VK_NUMPAD7";
    case VK_NUMPAD8: return "VK_NUMPAD8";
    case VK_NUMPAD9: return "VK_NUMPAD9";
    case VK_MULTIPLY: return "VK_MULTIPLY";
    case VK_ADD: return "VK_ADD";
    case VK_SEPARATOR: return "VK_SEPARATOR";
    case VK_SUBTRACT: return "VK_SUBTRACT";
    case VK_DECIMAL: return "VK_DECIMAL";
    case VK_DIVIDE: return "VK_DIVIDE";
    case VK_F1: return "VK_F1";
    case VK_F2: return "VK_F2";
    case VK_F3: return "VK_F3";
    case VK_F4: return "VK_F4";
    case VK_F5: return "VK_F5";
    case VK_F6: return "VK_F6";
    case VK_F7: return "VK_F7";
    case VK_F8: return "VK_F8";
    case VK_F9: return "VK_F9";
    case VK_F10: return "VK_F10";
    case VK_F11: return "VK_F11";
    case VK_F12: return "VK_F12";
    case VK_F13: return "VK_F13";
    case VK_F14: return "VK_F14";
    case VK_F15: return "VK_F15";
    case VK_F16: return "VK_F16";
    case VK_F17: return "VK_F17";
    case VK_F18: return "VK_F18";
    case VK_F19: return "VK_F19";
    case VK_F20: return "VK_F20";
    case VK_F21: return "VK_F21";
    case VK_F22: return "VK_F22";
    case VK_F23: return "VK_F23";
    case VK_F24: return "VK_F24";
    case VK_NAVIGATION_VIEW: return "VK_NAVIGATION_VIEW";
    case VK_NAVIGATION_MENU: return "VK_NAVIGATION_MENU";
    case VK_NAVIGATION_UP: return "VK_NAVIGATION_UP";
    case VK_NAVIGATION_DOWN: return "VK_NAVIGATION_DOWN";
    case VK_NAVIGATION_LEFT: return "VK_NAVIGATION_LEFT";
    case VK_NAVIGATION_RIGHT: return "VK_NAVIGATION_RIGHT";
    case VK_NAVIGATION_ACCEPT: return "VK_NAVIGATION_ACCEPT";
    case VK_NAVIGATION_CANCEL: return "VK_NAVIGATION_CANCEL";
    case VK_NUMLOCK: return "VK_NUMLOCK";
    case VK_SCROLL: return "VK_SCROLL";
    case VK_OEM_NEC_EQUAL: return "VK_OEM_NEC_EQUAL";
    // case VK_OEM_FJ_JISHO: return "VK_OEM_FJ_JISHO";
    case VK_OEM_FJ_MASSHOU: return "VK_OEM_FJ_MASSHOU";
    case VK_OEM_FJ_TOUROKU: return "VK_OEM_FJ_TOUROKU";
    case VK_OEM_FJ_LOYA: return "VK_OEM_FJ_LOYA";
    case VK_OEM_FJ_ROYA: return "VK_OEM_FJ_ROYA";
    case VK_LSHIFT: return "VK_LSHIFT";
    case VK_RSHIFT: return "VK_RSHIFT";
    case VK_LCONTROL: return "VK_LCONTROL";
    case VK_RCONTROL: return "VK_RCONTROL";
    case VK_LMENU: return "VK_LMENU";
    case VK_RMENU: return "VK_RMENU";
    case VK_BROWSER_BACK: return "VK_BROWSER_BACK";
    case VK_BROWSER_FORWARD: return "VK_BROWSER_FORWARD";
    case VK_BROWSER_REFRESH: return "VK_BROWSER_REFRESH";
    case VK_BROWSER_STOP: return "VK_BROWSER_STOP";
    case VK_BROWSER_SEARCH: return "VK_BROWSER_SEARCH";
    case VK_BROWSER_FAVORITES: return "VK_BROWSER_FAVORITES";
    case VK_BROWSER_HOME: return "VK_BROWSER_HOME";
    case VK_VOLUME_MUTE: return "VK_VOLUME_MUTE";
    case VK_VOLUME_DOWN: return "VK_VOLUME_DOWN";
    case VK_VOLUME_UP: return "VK_VOLUME_UP";
    case VK_MEDIA_NEXT_TRACK: return "VK_MEDIA_NEXT_TRACK";
    case VK_MEDIA_PREV_TRACK: return "VK_MEDIA_PREV_TRACK";
    case VK_MEDIA_STOP: return "VK_MEDIA_STOP";
    case VK_MEDIA_PLAY_PAUSE: return "VK_MEDIA_PLAY_PAUSE";
    case VK_LAUNCH_MAIL: return "VK_LAUNCH_MAIL";
    case VK_LAUNCH_MEDIA_SELECT: return "VK_LAUNCH_MEDIA_SELECT";
    case VK_LAUNCH_APP1: return "VK_LAUNCH_APP1";
    case VK_LAUNCH_APP2: return "VK_LAUNCH_APP2";
    case VK_OEM_1: return "VK_OEM_1";
    case VK_OEM_PLUS: return "VK_OEM_PLUS";
    case VK_OEM_COMMA: return "VK_OEM_COMMA";
    case VK_OEM_MINUS: return "VK_OEM_MINUS";
    case VK_OEM_PERIOD: return "VK_OEM_PERIOD";
    case VK_OEM_2: return "VK_OEM_2";
    case VK_OEM_3: return "VK_OEM_3";
    case VK_GAMEPAD_A: return "VK_GAMEPAD_A";
    case VK_GAMEPAD_B: return "VK_GAMEPAD_B";
    case VK_GAMEPAD_X: return "VK_GAMEPAD_X";
    case VK_GAMEPAD_Y: return "VK_GAMEPAD_Y";
    case VK_GAMEPAD_RIGHT_SHOULDER: return "VK_GAMEPAD_RIGHT_SHOULDER";
    case VK_GAMEPAD_LEFT_SHOULDER: return "VK_GAMEPAD_LEFT_SHOULDER";
    case VK_GAMEPAD_LEFT_TRIGGER: return "VK_GAMEPAD_LEFT_TRIGGER";
    case VK_GAMEPAD_RIGHT_TRIGGER: return "VK_GAMEPAD_RIGHT_TRIGGER";
    case VK_GAMEPAD_DPAD_UP: return "VK_GAMEPAD_DPAD_UP";
    case VK_GAMEPAD_DPAD_DOWN: return "VK_GAMEPAD_DPAD_DOWN";
    case VK_GAMEPAD_DPAD_LEFT: return "VK_GAMEPAD_DPAD_LEFT";
    case VK_GAMEPAD_DPAD_RIGHT: return "VK_GAMEPAD_DPAD_RIGHT";
    case VK_GAMEPAD_MENU: return "VK_GAMEPAD_MENU";
    case VK_GAMEPAD_VIEW: return "VK_GAMEPAD_VIEW";
    case VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON: return "VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON";
    case VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON: return "VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON";
    case VK_GAMEPAD_LEFT_THUMBSTICK_UP: return "VK_GAMEPAD_LEFT_THUMBSTICK_UP";
    case VK_GAMEPAD_LEFT_THUMBSTICK_DOWN: return "VK_GAMEPAD_LEFT_THUMBSTICK_DOWN";
    case VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT: return "VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT";
    case VK_GAMEPAD_LEFT_THUMBSTICK_LEFT: return "VK_GAMEPAD_LEFT_THUMBSTICK_LEFT";
    case VK_GAMEPAD_RIGHT_THUMBSTICK_UP: return "VK_GAMEPAD_RIGHT_THUMBSTICK_UP";
    case VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN: return "VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN";
    case VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT: return "VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT";
    case VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT: return "VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT";
    case VK_OEM_4: return "VK_OEM_4";
    case VK_OEM_5: return "VK_OEM_5";
    case VK_OEM_6: return "VK_OEM_6";
    case VK_OEM_7: return "VK_OEM_7";
    case VK_OEM_8: return "VK_OEM_8";
    case VK_OEM_AX: return "VK_OEM_AX";
    case VK_OEM_102: return "VK_OEM_102";
    case VK_ICO_HELP: return "VK_ICO_HELP";
    case VK_ICO_00: return "VK_ICO_00";
    case VK_PROCESSKEY: return "VK_PROCESSKEY";
    case VK_ICO_CLEAR: return "VK_ICO_CLEAR";
    case VK_PACKET: return "VK_PACKET";
    case VK_OEM_RESET: return "VK_OEM_RESET";
    case VK_OEM_JUMP: return "VK_OEM_JUMP";
    case VK_OEM_PA1: return "VK_OEM_PA1";
    case VK_OEM_PA2: return "VK_OEM_PA2";
    case VK_OEM_PA3: return "VK_OEM_PA3";
    case VK_OEM_WSCTRL: return "VK_OEM_WSCTRL";
    case VK_OEM_CUSEL: return "VK_OEM_CUSEL";
    case VK_OEM_ATTN: return "VK_OEM_ATTN";
    case VK_OEM_FINISH: return "VK_OEM_FINISH";
    case VK_OEM_COPY: return "VK_OEM_COPY";
    case VK_OEM_AUTO: return "VK_OEM_AUTO";
    case VK_OEM_ENLW: return "VK_OEM_ENLW";
    case VK_OEM_BACKTAB: return "VK_OEM_BACKTAB";
    case VK_ATTN: return "VK_ATTN";
    case VK_CRSEL: return "VK_CRSEL";
    case VK_EXSEL: return "VK_EXSEL";
    case VK_EREOF: return "VK_EREOF";
    case VK_PLAY: return "VK_PLAY";
    case VK_ZOOM: return "VK_ZOOM";
    case VK_NONAME: return "VK_NONAME";
    case VK_PA1: return "VK_PA1";
    case VK_OEM_CLEAR: return "VK_OEM_CLEAR";
  };
  return "Unknown";
}
}  // namespace tmk_desktop
