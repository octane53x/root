// Windows window

#ifndef WIN_WINDOW_HH
#define WIN_WINDOW_HH

#include "../../gl/env.hh"

struct window;

// Set in constructor. Enables global functions to call env.
// These functions must be global because they are passed to the OS.
window* _win;

// Physical window, sending I/O to environment
struct window : virtual env {

  // Window frame size
  int width, height;
  // Position of the window in the OS
  point win_pos;
  // Windows parameters coming in from wWinMain needed to display a window
  int win_param_2;
  HINSTANCE win_param_1;
  // Window handle returned by CreateWindow
  HWND hwnd;

  window();

  virtual void validate(const str& func);
  virtual void run();

  void init_members(const HINSTANCE wp1, const int wp2,
      const int w, const int h);
  void update_pos(const int x, const int y, const int w, const int h);
  void update_cursor(const int x, const int y);
  void send_key(const str& key, const bool down,
      const int cursor_x, const int cursor_y);
  void display();
  void main_loop(); };

// Set the global window pointer called by global functions
window::window(){
  type = "window";
  _win = this; }

// Draw the frame to the window with BitBlt
// Called by: _win_proc
void _win_paint(HWND hwnd){
  if(!_win->frame_updated) return;
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);
  image* frame = &_win->frame;
  assert(frame->width == _win->width && frame->height == _win->height,
      "_win_paint", "frame size not equal to window size");
  HBITMAP bmp = image_to_bmp(hdc, frame);
  HDC hdcMem = CreateCompatibleDC(NULL);
  HBITMAP bmpPrev = (HBITMAP)SelectObject(hdcMem, bmp);
  BitBlt(hdc, 0, 0, frame->width, frame->height, hdcMem, 0, 0, SRCCOPY);
  SelectObject(hdcMem, bmpPrev);
  DeleteDC(hdcMem);
  EndPaint(hwnd, &ps);
  _win->frame_updated = false; }

// Translate the key code into a string message
// Called by: _win_proc
str _win_key(WPARAM wParam){
  str s;
  switch(wParam){
    // Mouse clicks not handled like this:
    // case VK_LBUTTON: s = "LCLICK"; break;
    // case VK_RBUTTON: s = "RCLICK"; break;
    // case VK_MBUTTON: s = "MCLICK"; break;
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
LRESULT CALLBACK _win_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  // Update cursor position and draw
  POINT p;
  GetCursorPos(&p);
  _win->update_cursor(p.x, p.y);
  _win_paint(hwnd);
  RECT r;
  // Check uMsg for an event
  switch(uMsg){
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_PAINT:
    // Draw no matter what the uMsg since mouse movement is stopping WM_PAINT
    return 0;
  case WM_MOVE:
  case WM_SIZE:
    GetWindowRect(hwnd, &r);
    _win->update_pos(r.left, r.top, r.right - r.left, r.bottom - r.top);
    return 0;
  case WM_MOUSEMOVE:
    // Rarely called for some reason, prefer to update cursor each draw
    return 0;
  // Send a mouse click or key event
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
    _win->send_key("LCLICK", uMsg == WM_LBUTTONDOWN, p.x, p.y);
    return 0;
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
    _win->send_key("RCLICK", uMsg == WM_RBUTTONDOWN, p.x, p.y);
    return 0;
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
    _win->send_key("MCLICK", uMsg == WM_MBUTTONDOWN, p.x, p.y);
    return 0;
  case WM_MOUSEWHEEL:
    //!
    return 0;
  case WM_KEYDOWN:
  case WM_KEYUP:
    _win->send_key(_win_key(wParam), uMsg == WM_KEYDOWN, p.x, p.y);
    return 0;
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam); } }

// Ensure valid state
void window::validate(const str& func){
  env::validate(func);
  assert(width >= 0 && height >= 0, func, "window size negative"); }

// Load window with execution parameters
// Called by: PROJECT
void window::init_members(const HINSTANCE wp1, const int wp2,
    const int w, const int h){
  win_param_1 = wp1, win_param_2 = wp2;
  width = w, height = h;
  scene::win_w = w, scene::win_h = h; }

// Open the window and begin the main program loop
// Called by: PROJECT
void window::run(){
  system::run();
  display();
  main_loop(); }

// Report a change in window size or position
// Called by: _win_proc
void window::update_pos(const int x, const int y, const int w, const int h){
  win_pos = point(x, y);
  if(width == w && height == h) return;
  width = w, height = h;
  scene::win_w = w, scene::win_h = h;
  draw_bkgd();
  validate("window.update_pos"); }

// Report a change in the cursor position
// Called by: _win_proc
void window::update_cursor(const int x, const int y){
  cursor = point(x - win_pos.x - WIN_OFFSET_X,
      y - win_pos.y - WIN_OFFSET_Y);
  validate("window.update_cursor"); }

// Report a key event along with the cursor position at the time
// Called by: _win_proc
void window::send_key(const str& key, const bool down,
    const int cursor_x, const int cursor_y){
  env::key_event kp;
  kp.key = key, kp.down = down;
  kp.cursor = point((double)cursor_x - win_pos.x - WIN_OFFSET_X,
      (double)cursor_y - win_pos.y - WIN_OFFSET_Y);
  keys.push(kp);
  validate("window.send_key"); }

// Display the window in the OS
// Called by: PROJECT
void window::display(){
#ifdef _WIN32 // Just to compile
  const wchar_t CLASS[] = L"WindowClass";
  WNDCLASS wc = {};
  wc.lpfnWndProc = _win_proc;
  wc.hInstance = win_param_1;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);
  hwnd = CreateWindowEx(
      0, CLASS, L"Window", WS_OVERLAPPEDWINDOW, 1000, 200,
      _win->width, _win->height, NULL, NULL, win_param_1, NULL);
  assert(hwnd != NULL, "window.display", "could not create window");
  ShowWindow(hwnd, win_param_2);
#endif
  validate("window.display"); }

// The infinite loop driving the lifetime of the application
// Called by: PROJECT
void window::main_loop(){
  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    clock_t now = clock();
    double ms = (double)(now - last_update) * 1000.0 / CLOCKS_PER_SEC;
    _win->update(ms);
    _win->draw(&frame, viewport());
    InvalidateRect(hwnd, NULL, FALSE);
    validate("window.main_loop"); } }

#endif
