// ABSTRACT EDITOR WINDOW

#ifndef WINDOW_HH
#define WINDOW_HH

#include "util.hh"
#include "../gl/image.hh"
#include "../os/win/util.hh"

#define IDT_TIMER1 223

struct Window {

  struct KeyPress {
    str key;
    bool down;
    ipoint mouse;
    KeyPress(const str& k, const bool d, const ipoint& m); };

  HINSTANCE win_param_1;
  int win_param_2;
  HWND hwnd;

  bool updated;
  clock_t last_update, last_key;
  ipoint size, win_pos, mouse_pos;
  image frame;
  queue<KeyPress> keys;

  virtual void update(const double ms) = 0;
  virtual void draw() = 0;
  virtual void resize(const ipoint& npos, const ipoint& nsize) = 0; };

Window::KeyPress::KeyPress(const str& k, const bool d, const ipoint& m):
  key(k), down(d), mouse(m) {}

// GLOBAL WINDOW POINTER: SET IN EDITOR INIT
Window* _win;

void _win_paint(){
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(_win->hwnd, &ps);
  RECT r = ps.rcPaint;
  HBITMAP bmp = image_to_bmp(hdc, &_win->frame, ipoint(r.left, r.top),
      ipoint(r.right - r.left, r.bottom - r.top));
  HDC hdcMem = CreateCompatibleDC(NULL);
  HBITMAP bmpPrev = (HBITMAP)SelectObject(hdcMem, bmp);
  BitBlt(hdc, r.left, r.top, r.right - r.left, r.bottom - r.top,
      hdcMem, 0, 0, SRCCOPY);
  SelectObject(hdcMem, bmpPrev);
  DeleteObject(bmp);
  DeleteDC(hdcMem);
  EndPaint(_win->hwnd, &ps); }

str _win_key(WPARAM wParam){
  str s;
  switch(wParam){
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
    default: s = "OTHER"; break; }
  if(wParam >= '0' && wParam <= 'Z') s = (char)wParam;
  return s; }

LRESULT CALLBACK _win_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  POINT pwin;
  GetCursorPos(&pwin);
  ipoint p(pwin.x, pwin.y);
  _win->mouse_pos = p;
  RECT r;

  switch(uMsg){
  // Update and draw
  case WM_TIMER:
    if(wParam != IDT_TIMER1)
      return 0;
    SetTimer(hwnd, IDT_TIMER1, UPDATE_MS, NULL);
    _win->update((double)(clock() - _win->last_update) * 1000.0 / CPS);
    return 0;
  case WM_PAINT:
    _win_paint();
    return 0;

  // Adjust window
  case WM_MOVE:
  case WM_SIZE:
    GetWindowRect(hwnd, &r);
    _win->resize(ipoint(r.left, r.top),
        ipoint(r.right - r.left, r.bottom - r.top));
    return 0;

  // Mouse input
  case WM_MOUSEMOVE:
    return 0;
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
    _win->keys.push(Window::KeyPress("LCLICK", uMsg == WM_MBUTTONDOWN, p));
    _win->last_key = clock();
    return 0;
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
    _win->keys.push(Window::KeyPress("RCLICK", uMsg == WM_MBUTTONDOWN, p));
    _win->last_key = clock();
    return 0;
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
    _win->keys.push(Window::KeyPress("MCLICK", uMsg == WM_MBUTTONDOWN, p));
    _win->last_key = clock();
    return 0;
  case WM_MOUSEWHEEL:
    //!
    return 0;

  // Keyboard input
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
    _win->keys.push(Window::KeyPress(_win_key(wParam),
        uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN, p));
    _win->last_key = clock();
    return 0;

  case WM_CHAR:
  case WM_SYSCHAR:
    // Stop beeping
    return 0;
  case WM_ACTIVATE:
    // Send alt release on alt+tab
    if(wParam == FALSE)
      _win->keys.push(Window::KeyPress("ALT", false, p));
    return 0;

  case WM_DESTROY:
    // Exit application
    print(str("Terminated at "));
    print_time(time(NULL));
    PostQuitMessage(0);
    return 0;
  default:
    // Defer to default Windows procedure
    return DefWindowProc(hwnd, uMsg, wParam, lParam); } }

void _win_init(){
  const char* CLASS = "WindowClass";
  WNDCLASS wc = {};
  wc.lpfnWndProc = _win_proc;
  wc.hInstance = _win->win_param_1;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);
  _win->hwnd = CreateWindowEx(0, CLASS, "Window", WS_OVERLAPPEDWINDOW,
      _win->win_pos.x, _win->win_pos.y, _win->size.x, _win->size.y,
      NULL, NULL, _win->win_param_1, NULL);
  assert(_win->hwnd != NULL, "window.display", "could not create window");
  ShowWindow(_win->hwnd, _win->win_param_2); }

void _win_run(){
  MSG msg = {};
  SetTimer(_win->hwnd, IDT_TIMER1, UPDATE_MS, NULL);
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg); } }

#endif
