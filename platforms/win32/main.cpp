/**
 * @file main.cpp
 * @brief Win32アプリケーション
 * @copyright Copyright 2021 sgawarat <sgawarat@gmail.com>
 * @license This program is licensed under the GPLv2 or later. For more details, see LICENSE.
 */
#include <thread>
#include <utility>
#include <cstdio>
#include <Windows.h>
#include <tmk_desktop/source.hpp>
#include <tmk_desktop/keyboard.hpp>
#include <tmk_desktop/sink.hpp>
#include "utility.hpp"
#include "resource.h"

namespace tmk_desktop {
namespace {
const WCHAR* const TITLE = L"TMK Desktop";                // アプリケーション名
const WCHAR* const CLASS_NAME = L"TMK Desktop WNDCLASS";  // ウィンドウクラス名
const WCHAR* const WINDOW_NAME = L"TMK Desktop WINDOW";   // ウィンドウ名
constexpr UINT WM_APP_NOTIFY_ICON = WM_APP + 1;           // 通知アイコンのメッセージID

HMENU context_menu_ = NULL;  // コンテキストメニュー

DWORD main_thread_id_ = 0;                  ///< メインスレッドID
std::exception_ptr main_ep_ = nullptr;      ///< メインスレッドで投げられた例外
std::exception_ptr source_ep_ = nullptr;    ///< Sourceスレッドで投げられた例外
std::exception_ptr keyboard_ep_ = nullptr;  ///< Keyboardスレッドで投げられた例外
std::exception_ptr sink_ep_ = nullptr;      ///< Sinkスレッドで投げられた例外

/**
 * @brief スコープ終わりに関数を呼び出すクラス
 */
template <typename Dtor>
class Scoped {
public:
  Scoped(Dtor dtor) : dtor_(std::move(dtor)) {}
  ~Scoped() {
    dtor_();
  }

private:
  Dtor dtor_;
};

/**
 * @brief ウィンドウプロシージャ
 */
LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept {
  switch (msg) {
    case WM_COMMAND: {
      const WORD id = LOWORD(wparam);
      switch (id) {
        // 終了
        case ID_EXIT: {
          PostQuitMessage(0);
          break;
        }
        // 有効無効の切り替え
        case ID_ENABLE_DISABLE: {
          try {
            if (get_source_status() == SourceStatus::RESET) {
              start_sink();
              start_keyboard();
              start_source();
            } else {
              stop_source();
              stop_keyboard();
              stop_sink();
            }
          } catch (...) {
            main_ep_ = std::current_exception();
            PostQuitMessage(0);
          }
          break;
        }
        // すべてのキーを離す
        case ID_RELEASE_ALL_KEYS: {
          INPUT input{.type = INPUT_KEYBOARD, .ki = {}};
          for (WORD i = 1; i < 255; ++i) {
            input.ki.wVk = i;
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));
            input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY;
            SendInput(1, &input, sizeof(INPUT));
          }
          break;
        }
      }
      return 0;
    }
    case WM_APP_NOTIFY_ICON: {
      switch (LOWORD(lparam)) {
        // コンテキストメニューを表示させる
        case WM_CONTEXTMENU: {
          SetForegroundWindow(hwnd);
          TrackPopupMenuEx(context_menu_, TPM_RIGHTBUTTON, LOWORD(wparam), HIWORD(wparam), hwnd, NULL);
          PostMessage(hwnd, WM_NULL, 0, 0);
          break;
        }
      }
      return 0;
    }
    default:
      return DefWindowProc(hwnd, msg, wparam, lparam);
  }
}
}  // namespace

void on_source_error(std::exception&) noexcept {
  source_ep_ = std::current_exception();
  if (main_thread_id_ > 0) PostThreadMessage(main_thread_id_, WM_QUIT, 0, 0);
}

void on_keyboard_error(std::exception&) noexcept {
  keyboard_ep_ = std::current_exception();
  if (main_thread_id_ > 0) PostThreadMessage(main_thread_id_, WM_QUIT, 0, 0);
}

void on_sink_error(std::exception&) noexcept {
  sink_ep_ = std::current_exception();
  if (main_thread_id_ > 0) PostThreadMessage(main_thread_id_, WM_QUIT, 0, 0);
}

/**
 * @brief メイン関数
 */
extern "C" int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE, LPWSTR, int) {
  // 重複起動を防止する
  if (FindWindow(CLASS_NAME, WINDOW_NAME) != NULL) return 0;

  main_thread_id_ = GetCurrentThreadId();

  // エラーハンドリング
  const Scoped error_handling{[] {
    try {
      if (main_ep_) std::rethrow_exception(main_ep_);
      if (source_ep_) std::rethrow_exception(source_ep_);
      if (keyboard_ep_) std::rethrow_exception(keyboard_ep_);
      if (sink_ep_) std::rethrow_exception(sink_ep_);
    } catch (std::exception& e) {
      // TODO: 異常停止を安全にユーザーへ知らせる方法を考える
      throw;
    }
  }};

  // ウィンドウクラス
  const ATOM cls = [&] {
    const WNDCLASS wc{0, window_proc, 0, 0, instance, NULL, LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1), NULL, CLASS_NAME};
    return RegisterClass(&wc);
  }();
  if (cls == 0) return EXIT_FAILURE;
  const Scoped cls_dtor{[&] { UnregisterClass(MAKEINTATOM(cls), instance); }};

  // ウィンドウ
  const HWND wnd = CreateWindow(MAKEINTATOM(cls), WINDOW_NAME, WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, NULL, NULL, instance, 0);
  if (!wnd) return EXIT_FAILURE;
  const Scoped wnd_dtor{[&] { DestroyWindow(wnd); }};

  // アイコン
  const HICON icon = LoadIcon(NULL, IDI_APPLICATION);
  if (!icon) return EXIT_FAILURE;
  const Scoped icon_dtor{[&] { DestroyIcon(icon); }};

  // コンテキストメニュー
  const HMENU menu = LoadMenu(instance, MAKEINTRESOURCE(IDR_CONTEXT_MENU));
  if (!menu) return EXIT_FAILURE;
  const Scoped menu_dtor{[&] { DestroyMenu(menu); }};
  if (!(context_menu_ = GetSubMenu(menu, 0))) return EXIT_FAILURE;

  // 通知アイコン
  if (!add_notify_icon(wnd, 0, WM_APP_NOTIFY_ICON, icon, TITLE)) return EXIT_FAILURE;
  const Scoped notify_icon_dtor{[&] { remove_notify_icon(wnd, 0); }};

  // Sink
  start_sink();
  const Scoped sink_dtor{[] { stop_sink(); }};

  // Keyboard
  start_keyboard();
  const Scoped keyboard_dtor{[] { stop_keyboard(); }};

  // Source
  start_source();
  const Scoped source_dtor{[] { stop_source(); }};

  // メッセージループ
  MSG msg{};
  while (true) {
    const auto result = GetMessage(&msg, NULL, 0, 0);
    if (result == 0) break;  // WM_QUITを受け取った
    if (result > 0) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    std::this_thread::yield();
  }

  return static_cast<int>(msg.wParam);
}
}  // namespace tmk_desktop
