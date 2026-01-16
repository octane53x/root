// Visual Studio Test Environment

#include <windows.h>
#include "../../os/win/util.hh"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

const int
    INIT_WIN_W = 1200,
    INIT_WIN_H = 800;

HWND _hwnd;
int win_w, win_h;

image frame;
int c;

void init(){
  win_w = INIT_WIN_W, win_h = INIT_WIN_H;
  frame.set_size(ipoint(win_w, win_h));
  for(int y = 0; y < frame.size.y; ++y)
    for(int x = 0; x < frame.size.x; ++x)
      frame.set_pixel(ipoint(x, y), BLACK);
  c = 0;
}

void code(){
  for(int y = 0; y < frame.size.y; ++y)
    frame.set_pixel(ipoint(c, y), RED);
}

void paint(){
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(_hwnd, &ps);
  HBITMAP bmp = image_to_bmp(hdc, &frame);
  HDC hdcMem = CreateCompatibleDC(NULL);
  HBITMAP bmpPrev = (HBITMAP)SelectObject(hdcMem, bmp);
  BitBlt(hdc, 0, 0, frame.size.x, frame.size.y, hdcMem, 0, 0, SRCCOPY);
  SelectObject(hdcMem, bmpPrev);
  DeleteObject(bmp);
  DeleteDC(hdcMem);
  EndPaint(_hwnd, &ps); }

LRESULT CALLBACK _win_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  _hwnd = hwnd;
  switch(uMsg){
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_PAINT:
    paint();
    return 0;
  case WM_KEYDOWN:
    ++c;
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam); } }

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {

  init();

#ifdef _MSC_VER
  const char* CLASS = "WindowClass";
  const char* WINDOW = "Window";
#else
  const wchar_t CLASS[] = L"WindowClass";
  const wchar_t CLASS[] = L"Window";
#endif
  WNDCLASS wc = {};
  wc.lpfnWndProc = _win_proc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);

  HWND hwnd = CreateWindowEx(
      0, CLASS, WINDOW, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
      INIT_WIN_W, INIT_WIN_H, NULL, NULL, hInst, NULL);
  if(hwnd == NULL) return -1;
  _hwnd = hwnd;
  ShowWindow(hwnd, nCmdShow);

  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    code();
    InvalidateRect(hwnd, NULL, FALSE); }

  return 0; }
