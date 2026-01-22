// ABSTRACT EDITOR WINDOW

#ifndef WINDOW_HH
#define WINDOW_HH

#include "../gl/image.hh"
#include "../os/win/util.hh"

struct window : system {

  struct KeyPress {
    str key;
    bool down;
    ipoint mouse; };

  HINSTANCE win_param_1;
  int win_param_2;
  HWND hwnd;

  ipoint size, win_pos, mouse_pos;
  image frame;
  queue<KeyPress> keys;

  virtual void resize(const ipoint& npos, const ipoint& nsize) = 0;
  virtual void process_key(const str& key, const bool down,
      const ipoint& mouse) = 0; };

// GLOBAL WINDOW POINTER: SET IN EDITOR INIT
window* _win;

void _win_paint(HWND hwnd){
  if(!_win->updated) return;
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);
  HBITMAP bmp = image_to_bmp(hdc, &_win->frame);
  HDC hdcMem = CreateCompatibleDC(NULL);
  HBITMAP bmpPrev = (HBITMAP)SelectObject(hdcMem, bmp);
  BitBlt(hdc, 0, 0, _win->size.x, _win->size.y, hdcMem, 0, 0, SRCCOPY);
  SelectObject(hdcMem, bmpPrev);
  DeleteObject(bmp);
  DeleteDC(hdcMem);
  EndPaint(hwnd, &ps);
  _win->updated = false; }

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
  POINT p;
  GetCursorPos(&p);
  _win->mouse_pos = ipoint(p.x, p.y);
  RECT r;
  switch(uMsg){
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_PAINT:
    _win_paint(hwnd);
    return 0;
  case WM_MOVE:
  case WM_SIZE:
    GetWindowRect(hwnd, &r);
    _win->resize(
        ipoint(r.left, r.top),
        ipoint(r.right - r.left, r.bottom - r.top));
    return 0;
  case WM_MOUSEMOVE:
    return 0;
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
    _win->process_key("LCLICK", uMsg == WM_LBUTTONDOWN, ipoint(p.x, p.y));
    return 0;
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
    _win->process_key("RCLICK", uMsg == WM_RBUTTONDOWN, ipoint(p.x, p.y));
    return 0;
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
    _win->process_key("MCLICK", uMsg == WM_MBUTTONDOWN, ipoint(p.x, p.y));
    return 0;
  case WM_MOUSEWHEEL:
    //!
    return 0;
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
    _win->process_key(_win_key(wParam),
        uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN, ipoint(p.x, p.y));
    return 0;
  case WM_CHAR:
  case WM_SYSCHAR:
    // Stop beeping
    return 0;
  case WM_ACTIVATE:
    if(wParam == FALSE)
      _win->process_key("ALT", false, ipoint(p.x, p.y));
    return 0;
  default:
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
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    double ms = (double)(clock() - _win->last_update) * 1000.0 / CLOCKS_PER_SEC;
    _win->update(ms); } }

#endif
