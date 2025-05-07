// WINDOW

#include <windows.h>
#include <windowsx.h>
#include "../../imp/imp.hh"

Env* env;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  bool dir;
  str s;
  switch (uMsg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    //!
    HBITMAP bmp = CreateBitmap(1024, 768, 1, 32,
        env.active_scene->next_frame());
    //HBITMAP hBmp = (HBITMAP)LoadImage(NULL, TEXT("../../gl/fonts/aldo/A.bmp"),
    //    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //
    HDC hdcMem = CreateCompatibleDC(NULL);
    HBITMAP hBmpPrev = (HBITMAP)SelectObject(hdcMem, hBmp);
    StretchBlt(hdc, 0, 0, 1024, 768, hdcMem, 0, 0, 60, 90, SRCCOPY);

    EndPaint(hwnd, &ps);
    return 0; }
  case WM_MOUSEMOVE:
    env->cursor = point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    return 0;
  case WM_MOUSEWHEEL:
    //!
    return 0;
  case WM_KEYDOWN:
  case WM_KEYUP:
    dir = (uMsg == WM_KEYDOWN);
    switch(wParam){
      case VK_LBUTTON: s = "LCLICK"; break;
      case VK_RBUTTON: s = "RCLICK"; break;
      case VK_MBUTTON: s = "MCLICK"; break;
      case VK_BACK: s = "BACKSPACE"; break;
      case VK_TAB: s = "TAB"; break;
      case VK_RETURN: s = "ENTER"; break;
      case VK_SHIFT: s = "SHIFT"; break;
      case VK_CONTROL: s = "CONTROL"; break;
      case VK_MENU: s = "ALT"; break;
      case VK_CAPITAL: s = "CAPSLOCK"; break;
      case VK_ESCAPE: s = "ESCAPE"; break;
      case VK_SPACE: s = "SPACE"; break;
      case VK_LEFT: s = "LEFT"; break;
      case VK_UP: s = "UP"; break;
      case VK_RIGHT: s = "RIGHT"; break;
      case VK_DOWN: s = "DOWN"; break;
      case VK_DELETE: s = "DELETE"; break;
      case VK_OEM_1: s = "COLON"; break;
      case VK_OEM_PLUS: s = "EQUALS"; break;
      case VK_OEM_COMMA: s = "COMMA"; break;
      case VK_OEM_MINUS: s = "MINUS"; break;
      case VK_OEM_PERIOD: s = "PERIOD"; break;
      case VK_OEM_2: s = "SLASH"; break;
      case VK_OEM_3: s = "TILDE"; break;
      case VK_OEM_4: s = "LBRACKET"; break;
      case VK_OEM_5: s = "BACKSLASH"; break;
      case VK_OEM_6: s = "RBRACKET"; break;
      case VK_OEM_7: s = "QUOTE"; break;
    }
    if(wParam >= '0' && wParam <= 'Z') s = (char)wParam;
    env->keys.push(pair<str,bool>(s, dir));
    return 0;
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam); } }

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                    PWSTR pCmdLine, int nCmdShow) {
  env = new Env();
  env->active_scene = new TitleScreen();
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
