// ABSTRACT Windows Application

//! Test mouse_pos = pixel (0, 0) to find OFFSET

#ifndef WIN_APP_HH
#define WIN_APP_HH

#include "../../gl/image.hh"
#include "util.hh"

#define IDT_TIMER1 223

struct Application;

// ---- GLOBAL APPLICATION POINTER ----
Application* _app;

// Window I/O handler
struct Application {

  // Maximum frames per second
  double max_fps;
  // Time of prior update/draw completion
  clock_t last_update, last_draw;
  // Pixel position and size of window on screen
  ipoint pos, size;
  // Pixel position of cursor on screen
  ipoint cursor;
  // Title displayed in top bar of window
  str title;
  // Image frame passed in draw
  image frame;

  // Pushed to a queue when any key is pressed or released
  struct KeyEvent {
    // Key identifier
    str key;
    // Key down if true, up if false
    bool down;
    // Cursor pos at event time
    ipoint cursor;
    // Time of event
    clock_t time;

    KeyEvent(const str& k, const bool d, const ipoint& c, const clock_t t); };

  // Repeat which non-modifier keys when held
  enum {NONE, ALL, FIRST, LAST} key_mode;
  // Time of last input sent to derived application
  clock_t last_key_send;
  // Key event queue, processed on update
  queue<KeyEvent> keys;

  // Windows application instance passed by WinMain
  HINSTANCE hInst;
  // Window handle
  HWND hWnd;

  Application();

  virtual void init() = 0;
  virtual void update(const double sec) = 0;
  virtual void draw() = 0;
  virtual void resize() = 0;
  virtual void input(const KeyEvent& ke) = 0;

  void refresh();
  void display();
  void execute();

  static void paint_proc();
  static str key_proc(WPARAM wParam);
  static LRESULT CALLBACK msg_proc(HWND hWnd, UINT uMsg,
      WPARAM wParam, LPARAM lParam); };

// Set default member state and global pointer
// Called by: wWinMain()
Application::Application():
    last_update(0), last_draw(0), max_fps(60.0) {
    _app = this; }

// Set default member state
// Called by msg_proc()
Application::KeyEvent::KeyEvent(
    const str& k, const bool d, const ipoint& c, const clock_t t):
    key(k), down(d), cursor(c), time(t) {}

// Refresh window
// Called by: PROJECT
void Application::refresh(){
  SetWindowText(hWnd, title.c_str());
  InvalidateRect(hWnd, NULL, FALSE); }

// Display the window in the OS
// Called by: execute()
void Application::display(){
  const char* CLASS = "WindowClass";
  WNDCLASS wc = {};
  wc.lpfnWndProc = msg_proc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);
  hWnd = CreateWindowEx(
      0, CLASS, title.c_str(), WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, NULL, hInst, NULL);
  assert(hWnd != NULL, "Application.display", "could not create window");
  ShowWindow(hWnd, SW_SHOWMAXIMIZED); }

// Display window and run main loop
// Called by: wWinMain()
void Application::execute(){
  display();
  MSG msg = {};
  SetTimer(hWnd, IDT_TIMER1, 1, NULL);
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    double sec = (double)(clock() - last_update) / CPS;
    update(sec); //* time
    sec = (double)(clock() - last_draw) / CPS;
    if(sec < 1.0 / max_fps) continue;
    draw(); //* time
    InvalidateRect(hWnd, NULL, FALSE); } }

// Draw the frame to the window with BitBlt
// Called by: msg_proc()
void Application::paint_proc(){
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(_app->hWnd, &ps);
  HBITMAP bmp = image_to_bmp(hdc, &_app->frame, ipoint(0, 0), _app->frame.size);
  HDC hdcMem = CreateCompatibleDC(NULL);
  HBITMAP bmpPrev = (HBITMAP)SelectObject(hdcMem, bmp);
  BitBlt(hdc, 0, 0, _app->frame.size.x, _app->frame.size.y,
      hdcMem, 0, 0, SRCCOPY);
  SelectObject(hdcMem, bmpPrev);
  DeleteObject(bmp);
  DeleteDC(hdcMem);
  EndPaint(_app->hWnd, &ps); }

// Translate the key code into a string message
// Called by: msg_proc()
str Application::key_proc(WPARAM wParam){
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
LRESULT CALLBACK Application::msg_proc(
    HWND _hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  _app->hWnd = _hWnd;
  POINT pwin;
  GetCursorPos(&pwin);
  ipoint p(pwin.x, pwin.y);
  _app->cursor = p;
  clock_t msg_time = clock();
  RECT r;

  switch(uMsg){
  // Update and draw
  case WM_TIMER:
    if(wParam != IDT_TIMER1) return 0;
    SetTimer(_hWnd, IDT_TIMER1, 1, NULL);
    return 0;
  case WM_PAINT:
    paint_proc();
    return 0;

  // Adjust window
  case WM_MOVE:
    GetWindowRect(_hWnd, &r);
    _app->pos = ipoint(r.left, r.top);
    return 0;
  case WM_SIZE:
    GetWindowRect(_hWnd, &r);
    _app->size = ipoint(r.right - r.left, r.bottom - r.top);
    _app->resize();
    return 0;

  // Mouse input
  case WM_MOUSEMOVE:
    // Handled at top of function
    return 0;
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
    _app->keys.push(KeyEvent("LCLICK", uMsg == WM_MBUTTONDOWN, p, msg_time));
    return 0;
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
    _app->keys.push(KeyEvent("RCLICK", uMsg == WM_MBUTTONDOWN, p, msg_time));
    return 0;
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
    _app->keys.push(KeyEvent("MCLICK", uMsg == WM_MBUTTONDOWN, p, msg_time));
    return 0;
  case WM_MOUSEWHEEL:
    //!
    return 0;

  // Keyboard input
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
    _app->keys.push(KeyEvent(key_proc(wParam),
        uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN, p, msg_time));
    return 0;

  case WM_CHAR:
  case WM_SYSCHAR:
    // Stop beeping
    return 0;
  case WM_ACTIVATE:
    // Send alt release on alt+tab
    if(wParam == FALSE)
      _app->keys.push(KeyEvent("ALT", false, p, msg_time));
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
