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
#include <tmk_desktop/engine/host_driver.hpp>
#include <tmk_desktop/engine/keyboard.hpp>
#include "source.hpp"
#include "sink.hpp"
#include "host_driver.hpp"
#include "utility.hpp"
#include "logging.hpp"
#include "resource.h"

namespace tmk_desktop::win32 {
namespace {
const WCHAR* const TITLE = L"TMK Desktop";  // アプリケーション名
const WCHAR* const CLASS_NAME = L"TMK Desktop WNDCLASS";  // ウィンドウクラス名
const WCHAR* const WINDOW_NAME = L"TMK Desktop WINDOW";  // ウィンドウ名
constexpr UINT WM_APP_NOTIFY_ICON = WM_APP + 1;  // 通知アイコンのメッセージID

HWND wnd_ = NULL;  // ウィンドウ
HMENU context_menu_ = NULL;  // コンテキストメニュー
std::exception_ptr main_ep_ = nullptr;  ///< メインスレッドで投げられた例外
std::exception_ptr source_ep_ = nullptr;  ///< Sourceスレッドで投げられた例外
std::exception_ptr sink_ep_ = nullptr;  ///< Sinkスレッドで投げられた例外

/**
 * @brief Sourceスレッドで例外発生時に呼ばれる関数
 */
void on_source_error() noexcept {
  source_ep_ = std::current_exception();
  if (wnd_) PostMessage(wnd_, WM_QUIT, 0, 0);
}

/**
 * @brief Sinkスレッドで例外発生時に呼ばれる関数
 */
void on_sink_error() noexcept {
  sink_ep_ = std::current_exception();
  if (wnd_) PostMessage(wnd_, WM_QUIT, 0, 0);
}

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
              debug_log("Enable\n");
              start_sink();
              start_source();
            } else {
              debug_log("Disable\n");
              stop_source();
              stop_sink();
            }
          } catch (...) {
            main_ep_ = std::current_exception();
            PostQuitMessage(0);
          }
          break;
        }
        // すべてのキーを離す
        case ID_RESET_KEYBOARD: {
          debug_log("Reset Keyboard\n");
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
#ifdef TMK_DESKTOP_ENABLE_LOGGING
        // コンソールを表示/非表示する
        case ID_SHOW_HIDE_CONSOLE_WINDOW: {
          const HWND console = GetConsoleWindow();
          if (IsWindowVisible(console)) {
            ShowWindow(console, SW_HIDE);
          } else {
            ShowWindow(console, SW_SHOW);
          }
          break;
        }
#endif
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
    default: return DefWindowProc(hwnd, msg, wparam, lparam);
  }
}
}  // namespace

extern "C" {
/**
 * @brief メイン関数
 */
int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE, LPWSTR, int) {
  // 重複起動を防止する
  if (FindWindow(CLASS_NAME, WINDOW_NAME) != NULL) return 0;

#ifdef TMK_DESKTOP_ENABLE_LOGGING
  // コンソールウィンドウ
  AllocConsole();
  [[maybe_unused]] const auto conout_fp = freopen("CONOUT$", "w", stdout);
  SetWindowPos(GetConsoleWindow(), NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
  SetConsoleOutputCP(CP_UTF8);
  auto console_dtor = make_dtor([] {
    // 異常停止した場合、終了するのにユーザーの同意を求める
    if (main_ep_ || source_ep_ || sink_ep_) {
      ShowWindow(GetConsoleWindow(), SW_SHOW);
      system("PAUSE");
    }
  });
#endif

  // ウィンドウクラス
  const ATOM cls = [&] {
    const WNDCLASS wc{0, window_proc, 0, 0, instance, NULL, LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1), NULL, CLASS_NAME};
    return RegisterClass(&wc);
  }();
  if (cls == 0) return EXIT_FAILURE;
  auto cls_dtor = make_dtor([&] {UnregisterClass(MAKEINTATOM(cls), instance);});

  // ウィンドウ
  wnd_ = CreateWindow(MAKEINTATOM(cls), WINDOW_NAME, WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, NULL, NULL, instance, 0);
  if (!wnd_) return EXIT_FAILURE;
  auto wnd_dtor = make_dtor([&] {DestroyWindow(wnd_);});

  // アイコン
  const HICON icon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
  if (!icon) return EXIT_FAILURE;
  auto icon_dtor = make_dtor([&] {DestroyIcon(icon);});

  // コンテキストメニュー
  const HMENU menu = LoadMenu(instance, MAKEINTRESOURCE(IDR_CONTEXT_MENU));
  if (!menu) return EXIT_FAILURE;
  auto menu_dtor = make_dtor([&] {DestroyMenu(menu);});
  if (!(context_menu_ = GetSubMenu(menu, 0))) return EXIT_FAILURE;

  // 通知アイコン
  if (!add_notify_icon(wnd_, 0, WM_APP_NOTIFY_ICON, icon, TITLE)) return EXIT_FAILURE;
  auto notify_icon_dtor = make_dtor([&] {remove_notify_icon(wnd_, 0);});

  // ホストドライバ
  engine::set_host_driver(host_driver_t{
    .send_keyboard = send_keyboard,
    .send_mouse = send_mouse,
    .send_system = send_system,
    .send_consumer = send_consumer,
  });
  auto driver_dtor = make_dtor([] {engine::reset_host_driver();});

  // キーボードエンジン
  engine::start_keyboard();
  auto engine_dtor = make_dtor([] {engine::stop_keyboard();});

  // Sink
  start_sink(on_sink_error);
  auto sink_thread_dtor = make_dtor([] {
    stop_sink();
    try {
      if (sink_ep_) std::rethrow_exception(sink_ep_);
    } catch (std::exception& e) {
      debug_log("Quit the sink thread unsuccessfully: {}\n", e.what());
    }
  });

  // Source
  start_source(on_source_error);
  auto source_thread_dtor = make_dtor([] {
    stop_source();
    try {
      if (source_ep_) std::rethrow_exception(source_ep_);
    } catch (std::exception& e) {
      debug_log("Quit the source thread unsuccessfully: {}\n", e.what());
    }
  });

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
}  // extern "C"
}  // namespace tmk_desktop::win32
