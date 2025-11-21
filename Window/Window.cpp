// WINDOW

#include "../imp/impact.hh"

const int INIT_WIN_W = 500,
          INIT_WIN_H = 500;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

  bool dir;
  str s;
  POINT p;
  RECT r;

  // Update cursor position
  GetCursorPos(&p);
  update_cursor(p.x, p.y);

  // Update window position and size
  GetWindowRect(hwnd, &r);
  update_window(r.left, r.top, r.right - r.left, r.bottom - r.top);

  switch(uMsg){
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // Copy frame to screen
    image* frame = get_frame();
    HBITMAP bmp = image_to_bmp(hdc, frame);
    HDC hdcMem = CreateCompatibleDC(NULL);
    HBITMAP bmpPrev = (HBITMAP)SelectObject(hdcMem, bmp);
    BitBlt(hdc, 0, 0, frame->width, frame->height, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, bmpPrev);
    DeleteDC(hdcMem);

    EndPaint(hwnd, &ps);
    return 0; }
  case WM_MOUSEMOVE:
    // This is handled up top
    return 0;

  // Send a key event or mouse click
  case WM_LBUTTONDOWN:
    send_key("LCLICK", true, p.x, p.y);
    return 0;
  case WM_LBUTTONUP:
    send_key("LCLICK", false, p.x, p.y);
    return 0;
  case WM_RBUTTONDOWN:
    send_key("RCLICK", true, p.x, p.y);
    return 0;
  case WM_RBUTTONUP:
    send_key("RCLICK", false, p.x, p.y);
    return 0;
  case WM_MBUTTONDOWN:
    send_key("MCLICK", true, p.x, p.y);
    return 0;
  case WM_MBUTTONUP:
    send_key("MCLICK", false, p.x, p.y);
    return 0;
  case WM_MOUSEWHEEL:
    //!
    return 0;

  case WM_KEYDOWN:
  case WM_KEYUP:
    dir = (uMsg == WM_KEYDOWN);
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
    }
    if(wParam >= '0' && wParam <= 'Z') s = (char)wParam;
    GetCursorPos(&p);
    update_cursor(p.x, p.y);
    send_key(s, dir, p.x, p.y);

    return 0;
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam); } }

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                    PWSTR pCmdLine, int nCmdShow) {
  init_impact(INIT_WIN_W, INIT_WIN_H); // Initialize environment
  const wchar_t CLASS[] = L"WindowClass";
  WNDCLASS wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);
  HWND hwnd = CreateWindowEx(
      0, CLASS, L"Window", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, INIT_WIN_W, INIT_WIN_H,
      nullptr, nullptr, hInst, nullptr);
  if(hwnd == nullptr) return 0;
  ShowWindow(hwnd, nCmdShow);
  MSG msg = {};
  while(GetMessage(&msg, nullptr, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    update_impact(); // Update environment
    InvalidateRect(hwnd, nullptr, FALSE); }
  return 0; }
