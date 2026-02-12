// Windows Window
// Owned by Application

#ifndef WINDOW_HH
#define WINDOW_HH

#include "keyrouter.hh"

// Windows timer identifier
#define IDT_TIMER1 223

//-------- GLOBAL APPLICATION POINTER --------
// For static functions to call application
struct Window;
Window* _win;

// Window appearing in Windows OS
struct Window {

  // Windows application instance passed by WinMain
  HINSTANCE hInst;
  // Window handle
  HWND hWnd;

  // Whether to open the window maximized
  bool start_maximized;
  // Pixel position and size of window on screen
  ipoint pos, size;
  // Pixel position of cursor on screen
  ipoint cursor;
  // Title displayed in top bar of window
  str title;
  // Image frame passed in draw
  image frame;

  // Input processor
  KeyRouter key_router;

  virtual void init() = 0;
  virtual void update() = 0;
  virtual bool draw() = 0;
  virtual void resize() = 0;

  void display();
  void refresh();

  static void paint_proc();
  static str key_proc(WPARAM wParam);
  static LRESULT CALLBACK msg_proc(HWND _hWnd, UINT uMsg,
      WPARAM wParam, LPARAM lParam); };

// Set default member state and global pointer
// Called by: Application.init()
void Window::init(){
  _win = this;
  start_maximized = true;
  key_router.init(); }

// Display the window in the OS
// Called by: Application.run()
void Window::display(){
  const char* CLASS = "WindowClass";
  WNDCLASS wc = {};
  wc.lpfnWndProc = msg_proc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);
  hWnd = CreateWindowEx(
      0, CLASS, title.c_str(), WS_OVERLAPPEDWINDOW,
      start_maximized ? CW_USEDEFAULT : pos.x,
      start_maximized ? CW_USEDEFAULT : pos.y,
      start_maximized ? CW_USEDEFAULT : size.x,
      start_maximized ? CW_USEDEFAULT : size.y,
      NULL, NULL, hInst, NULL);
  assert(hWnd != NULL, "Application.display", "could not create window");
  ShowWindow(hWnd, start_maximized ? SW_SHOWMAXIMIZED : SW_SHOWDEFAULT); }

// Refresh window
// Called by: PROJECT
void Window::refresh(){
  SetWindowText(hWnd, title.c_str());
  InvalidateRect(hWnd, NULL, FALSE); }

// Draw the frame to the window with BitBlt
// Called by: msg_proc()
void Window::paint_proc(){
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(_win->hWnd, &ps);
  RECT r = ps.rcPaint;
  HBITMAP bmp = image_to_bmp(&_win->frame, ipoint(r.left, r.top),
      ipoint(r.right - r.left, r.bottom - r.top));
  HDC hdcMem = CreateCompatibleDC(NULL);
  HBITMAP bmpPrev = (HBITMAP)SelectObject(hdcMem, bmp);
  BitBlt(hdc, r.left, r.top, r.right - r.left, r.bottom - r.top,
      hdcMem, 0, 0, SRCCOPY);
  SelectObject(hdcMem, bmpPrev);
  DeleteObject(bmp);
  DeleteDC(hdcMem);
  EndPaint(_win->hWnd, &ps); }

// Translate the key code into a string message
// Called by: msg_proc()
str Window::key_proc(WPARAM wParam){
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

// Sent to Windows for window message processing
// Called by: OPERATING SYSTEM
LRESULT CALLBACK Window::msg_proc(
    HWND _hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  _win->hWnd = _hWnd;
  POINT pwin;
  GetCursorPos(&pwin);
  ipoint p(pwin.x, pwin.y);
  _win->cursor = _win->key_router.cursor = p;
  clock_t msg_time = clock();
  int wheel;
  RECT r;

  switch(uMsg){
  // Update and draw
  case WM_TIMER:
    if(wParam != IDT_TIMER1) return 0;
    _win->update();
    _win->draw();
    return 0;
  case WM_PAINT:
    paint_proc();
    return 0;

  // Adjust window
  case WM_MOVE:
    GetWindowRect(_hWnd, &r);
    _win->pos = ipoint(r.left, r.top);
    return 0;
  case WM_SIZE:
    GetWindowRect(_hWnd, &r);
    _win->size = ipoint(r.right - r.left, r.bottom - r.top);
    _win->resize();
    return 0;

  // Mouse input
  case WM_MOUSEMOVE:
    // Handled at top of function
    return 0;
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
    _win->key_router.send(
        KeyEvent("LCLICK", uMsg == WM_MBUTTONDOWN, p, msg_time));
    return 0;
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
    _win->key_router.send(
        KeyEvent("RCLICK", uMsg == WM_MBUTTONDOWN, p, msg_time));
    return 0;
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
    _win->key_router.send(
        KeyEvent("MCLICK", uMsg == WM_MBUTTONDOWN, p, msg_time));
    return 0;
  case WM_MOUSEWHEEL:
    wheel = (int)GET_WHEEL_DELTA_WPARAM(wParam);
    _win->key_router.send(KeyEvent("SCROLL", (wheel < 0), p, msg_time));
    return 0;

  // Keyboard input
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
    _win->key_router.send(KeyEvent(key_proc(wParam),
        uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN, p, msg_time));
    return 0;

  case WM_CHAR:
  case WM_SYSCHAR:
    // Stop beeping
    return 0;
  case WM_ACTIVATE:
    // Send alt release on alt+tab
    if(wParam == FALSE)
      _win->key_router.send(KeyEvent("ALT", false, p, msg_time));
    return 0;

  case WM_DESTROY:
    // Exit application
    print(str("Terminated at "));
    print_time(time(NULL));
    PostQuitMessage(0);
    return 0;
  default:
    // Defer to default Windows procedure
    return DefWindowProc(_hWnd, uMsg, wParam, lParam); } }

#endif
