#include <Windows.h>
#include <cstdint>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  WNDCLASS wc{};
  wc.lpfnWndProc = WindowProc;
  wc.lpszClassName = L"CG2WindowClass";
  wc.hInstance = GetModuleHandle(nullptr);
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  RegisterClass(&wc);

  const int32_t kClientWidth = 1280;
  const int32_t kClientHeight = 720;

  RECT wrc = { 0,0,kClientWidth, kClientHeight };
  AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

  HWND hwnd = CreateWindow(
    wc.lpszClassName,       // 利用するクラス名
    L"CG2",                 // タイトルバーの文字（何でも良い）
    WS_OVERLAPPEDWINDOW,    // よく見るウィンドウスタイル
    CW_USEDEFAULT,          // 表示X座標（Windowsに任せる）
    CW_USEDEFAULT,          // 表示Y座標（WindowsOSに任せる）
    wrc.right - wrc.left,   // ウィンドウ横幅
    wrc.bottom - wrc.top,   // ウィンドウ縦幅
    nullptr,                // 親ウィンドウハンドル
    nullptr,                // メニューハンドル
    wc.hInstance,           // インスタンスハンドル
    nullptr);               // オプション
    // ウィンドウを表示する
  ShowWindow(hwnd, SW_SHOW);

  MSG msg{};
  // ウィンドウの×ボタンが押されるまでループ
  while (msg.message != WM_QUIT) {
    // Windowにメッセージが来てたら最優先で処理させる
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else {
      // ゲームの処理
    }
  }
  // 出力ウィンドウへの文字出力
  OutputDebugStringA("Hello,DirectX!\n");

  return 0;
}
