// Windows Window
// Owned by Application

#ifndef WINDOW_HH
#define WINDOW_HH

#include "../../ui/interface.hh"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

// Windows timer identifier
#define IDT_TIMER1 223

// Difference between reported window size and actual frame size
const int
    FRAME_X_OFFSET = -16,
    FRAME_Y_OFFSET = -39;

//-------- GLOBAL APPLICATION POINTER --------
// For static functions to call application
struct Window;
Window* _win;

// Window appearing in Windows OS
struct Window : Interface {

  // Windows application instance passed by WinMain
  HINSTANCE hInst;
  // Window handle
  HWND hWnd;
  // Frame memory device context
  HDC hdcMem;
  // Bitmaps in memory
  HBITMAP bmpDIB, bmpOld;

  // Whether to open the window maximized
  bool start_maximized;
  // Whether the frame has been updated
  bool updated;
  // Pixel position and size of window on screen, according to Windows
  ipoint win_pos, win_size;
  // Pixel size of image frame on screen (pos is always 0,0)
  // Differs from win_size due to Windows offsets
  ipoint frame_size;
  // Title displayed in top bar of window
  str title;
  // Color buffer for drawing
  ui* color_buf;

  virtual void init();
  virtual void update() = 0;
  virtual void draw() = 0;
  virtual void resize();

  BITMAPINFO get_bmi() const;

  void display();
  void set_title(const str& _title);
  void draw_pixel(const ipoint pos, const ui color);

  static void paint_proc();
  static str key_proc(WPARAM wParam);
  static LRESULT CALLBACK msg_proc(HWND _hWnd, UINT uMsg,
      WPARAM wParam, LPARAM lParam); };

// Set default member state and global pointer
// Called by: Application.init()
void Window::init(){
  _win = this;
  Interface::init();
  start_maximized = true;
  updated = true;
  color_buf = NULL; }

// Resize the window, recreating the frame buffer
// Called by: msg_proc() OR Application::X.resize()
void Window::resize(){
  frame_size = ipoint(win_size.x + FRAME_X_OFFSET, win_size.y + FRAME_Y_OFFSET);
  SelectObject(hdcMem, bmpOld);
  DeleteObject(bmpDIB);
  color_buf = NULL;
  BITMAPINFO bmi = get_bmi();
  HDC hdc = GetDC(hWnd);
  bmpDIB = CreateDIBSection(
      hdc, &bmi, DIB_RGB_COLORS, &(void*)color_buf, NULL, 0);
  ReleaseDC(hWnd, hdc);
  bmpOld = (HBITMAP)SelectObject(hdcMem, bmpDIB); }

// Set up parameters to device-independent bitmap creation
// Called by: resize(), display()
BITMAPINFO Window::get_bmi() const {
  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = frame_size.x;
  bmi.bmiHeader.biHeight = -frame_size.y;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  return bmi; }

// Display the window in the OS
// Called after Application.init() so pos and size are set
// Called by: Application.run()
void Window::display(){
  // Set up the window
  const char* CLASS = "WindowClass";
  WNDCLASS wc = {};
  wc.lpfnWndProc = msg_proc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);

  // Create the window
  hWnd = CreateWindowEx(
      0, CLASS, title.c_str(), WS_OVERLAPPEDWINDOW,
      start_maximized ? CW_USEDEFAULT : win_pos.x,
      start_maximized ? CW_USEDEFAULT : win_pos.y,
      start_maximized ? CW_USEDEFAULT : win_size.x,
      start_maximized ? CW_USEDEFAULT : win_size.y,
      NULL, NULL, hInst, NULL);
  assert(hWnd != NULL, "Application.display", "could not create window");

  // Prepare memory for drawing
  BITMAPINFO bmi = get_bmi();
  HDC hdc = GetDC(hWnd);
  hdcMem = CreateCompatibleDC(hdc);
  bmpDIB = CreateDIBSection(
      hdc, &bmi, DIB_RGB_COLORS, &(void*)color_buf, NULL, 0);
  ReleaseDC(hWnd, hdc);
  bmpOld = (HBITMAP)SelectObject(hdcMem, bmpDIB);

  // Display the window
  ShowWindow(hWnd, start_maximized ? SW_SHOWMAXIMIZED : SW_SHOWDEFAULT); }

// Set the window title in the top bar
// Called by: PROJECT
void Window::set_title(const str& _title){
  title = _title;
  SetWindowText(hWnd, title.c_str()); }

// Set a pixel in the frame memory
// Called by: PROJECT
void Window::draw_pixel(const ipoint pos, const ui color){
  color_buf[pos.y * frame_size.x + pos.x] = color; }

// Draw the frame to the window with BitBlt
// Called by: msg_proc()
void Window::paint_proc(){
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(_win->hWnd, &ps);
  BitBlt(hdc, 0, 0, _win->frame_size.x, _win->frame_size.y,
      _win->hdcMem, 0, 0, SRCCOPY);
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
    HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  _win->hWnd = hWnd;
  POINT pwin;
  GetCursorPos(&pwin);
  ipoint p(pwin.x, pwin.y);
  _win->cursor = p;
  clock_t msg_time = clock();
  int wheel;
  RECT r;

  switch(uMsg){
  // Update and draw
  case WM_TIMER:
    if(wParam != IDT_TIMER1) return 0;
    _win->update();
    if(_win->updated){
      _win->draw();
      InvalidateRect(hWnd, NULL, FALSE);
      _win->updated = false; }
    return 0;
  case WM_PAINT:
    paint_proc();
    return 0;

  // Adjust window
  case WM_MOVE:
    GetWindowRect(hWnd, &r);
    _win->win_pos = ipoint(r.left, r.top);
    return 0;
  case WM_SIZE:
    GetWindowRect(hWnd, &r);
    _win->win_size = ipoint(r.right - r.left, r.bottom - r.top);
    _win->resize();
    return 0;

  // Mouse input
  case WM_MOUSEMOVE:
    // Handled at top of function
    return 0;
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
    _win->send_key(KeyEvent("LCLICK", uMsg == WM_MBUTTONDOWN, p, msg_time));
    return 0;
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
    _win->send_key(KeyEvent("RCLICK", uMsg == WM_MBUTTONDOWN, p, msg_time));
    return 0;
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
    _win->send_key(KeyEvent("MCLICK", uMsg == WM_MBUTTONDOWN, p, msg_time));
    return 0;
  case WM_MOUSEWHEEL:
    wheel = (int)GET_WHEEL_DELTA_WPARAM(wParam);
    _win->send_key(KeyEvent("SCROLL", (wheel < 0), p, msg_time));
    return 0;

  // Keyboard input
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
    _win->send_key(KeyEvent(key_proc(wParam),
        uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN, p, msg_time));
    return 0;

  case WM_CHAR:
  case WM_SYSCHAR:
    // Stop beeping
    return 0;
  case WM_ACTIVATE:
    // Send alt release on alt+tab
    if(wParam == FALSE)
      _win->send_key(KeyEvent("ALT", false, p, msg_time));
    return 0;

  case WM_DESTROY:
    // Exit application
    print(str("Terminated at "));
    print_time(time(NULL));
    PostQuitMessage(0);
    return 0;
  default:
    // Defer to default Windows procedure
    return DefWindowProc(hWnd, uMsg, wParam, lParam); } }

#endif
