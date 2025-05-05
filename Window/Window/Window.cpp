// WINDOW

#include <windows.h>
#include "../../lang/core/util.hh"

const int WIN_W = 1024,
          WIN_H = 768;

void draw(const HDC& hdc){
  //!
  int x = 0;
  while(1){
    RECT r;
    r.left = 50+x;
    r.top = 50+x;
    r.right = 100+x;
    r.bottom = 100+x;
    for(int i = 0; i < 100; ++i){
      FillRect(hdc, &r, (HBRUSH)(COLOR_WINDOW + 1));
      ++x;
      r.left = 50+x;
      r.top = 50+x;
      r.right = 100+x;
      r.bottom = 100+x;
      FillRect(hdc, &r, (HBRUSH)(COLOR_HIGHLIGHT + 1));
      sleep(10);
    }
    for(int i = 0; i < 100; ++i){
      FillRect(hdc, &r, (HBRUSH)(COLOR_WINDOW + 1));
      --x;
      r.left = 50+x;
      r.top = 50+x;
      r.right = 100+x;
      r.bottom = 100+x;
      FillRect(hdc, &r, (HBRUSH)(COLOR_HIGHLIGHT + 1));
      sleep(10);
    }
  }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  switch (uMsg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    draw(hdc);
    EndPaint(hwnd, &ps);
    return 0; }
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam); } }

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                    PWSTR pCmdLine, int nCmdShow) {
  const wchar_t CLASS[] = L"WindowClass";
  WNDCLASS wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);
  HWND hwnd = CreateWindowEx(
      0, CLASS, L"Window", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, WIN_W, WIN_H,
      nullptr, nullptr, hInst, nullptr);
  if(hwnd == nullptr) return 0;
  ShowWindow(hwnd, nCmdShow);
  MSG msg = {};
  while(GetMessage(&msg, nullptr, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg); }
  return 0; }
