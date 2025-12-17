// EDITOR

#ifndef EDITOR_HH
#define EDITOR_HH

#include "../os/win/util.hh" // load_bmp
#include "../gl/polygon.hh"

const int
    INIT_WIN_W = 1500,
    INIT_WIN_H = 750,
    LINE_HEIGHT = 20,
    CHAR_WIDTH = 10;
const double
    CURSOR_BLINK = 0.5;
const color
    INIT_BKGD_COLOR = BLACK,
    INIT_CURSOR_COLOR = CYAN;
const str
    _FONT_LOC = "../symbols.bmp",
    _SYMBOLS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        ".,/?:;'\"+=-_\\|~!@#$%^&*()[]{}<>";

struct Editor {

  HINSTANCE win_param_1;
  int win_param_2;

  bool updated, shift, ctrl, alt;
  int width, height;
  clock_t last_update;
  color bkgd_color;
  point win_pos, mouse_pos;
  vec<str> text;
  image frame;
  umap<char, image> font;

  struct Cursor : virtual polygon {
    bool blink;
    int x, y, xlast, ylast;
    color col;
  } cursor;

  void init(const HINSTANCE wp1, const int wp2);
  void run();
  void update(const double ms);
  void draw();
  void draw_bkgd();
  void load_font();
  void process_key(const str& key, const bool down, const point& mouse);

} edit;

void _win_paint(HWND hwnd){
  if(!edit.updated) return;
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);
  HBITMAP bmp = image_to_bmp(hdc, &edit.frame);
  HDC hdcMem = CreateCompatibleDC(NULL);
  HBITMAP bmpPrev = (HBITMAP)SelectObject(hdcMem, bmp);
  BitBlt(hdc, 0, 0, edit.width, edit.height, hdcMem, 0, 0, SRCCOPY);
  SelectObject(hdcMem, bmpPrev);
  DeleteDC(hdcMem);
  EndPaint(hwnd, &ps);
  edit.updated = false; }

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

LRESULT CALLBACK _win_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  POINT p;
  GetCursorPos(&p);
  edit.mouse_pos = point(p.x, p.y);
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
    edit.win_pos = point(r.left, r.top);
    edit.width = r.right - r.left, edit.height = r.bottom - r.top;
    return 0;
  case WM_MOUSEMOVE:
    return 0;
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
    edit.process_key("LCLICK", uMsg == WM_LBUTTONDOWN, point(p.x, p.y));
    return 0;
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
    edit.process_key("RCLICK", uMsg == WM_RBUTTONDOWN, point(p.x, p.y));
    return 0;
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
    edit.process_key("MCLICK", uMsg == WM_MBUTTONDOWN, point(p.x, p.y));
    return 0;
  case WM_MOUSEWHEEL:
    //!
    return 0;
  case WM_KEYDOWN:
  case WM_KEYUP:
    edit.process_key(_win_key(wParam), uMsg == WM_KEYDOWN, point(p.x, p.y));
    return 0;
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam); } }

void Editor::init(const HINSTANCE wp1, const int wp2){
  debug("init");
  updated = true;
  shift = ctrl = alt = false;
  win_param_1 = wp1, win_param_2 = wp2;
  width = INIT_WIN_W, height = INIT_WIN_H;
  last_update = 0;
  bkgd_color = INIT_BKGD_COLOR;
  text.pb("");

  load_font();
  draw_bkgd();

  cursor.x = cursor.y = 0;
  cursor.xlast = cursor.ylast = 0;
  cursor.add(point(0, 0));
  cursor.add(point(CHAR_WIDTH-1, 0));
  cursor.add(point(CHAR_WIDTH-1, LINE_HEIGHT-1));
  cursor.add(point(0, LINE_HEIGHT-1));
  cursor.col = INIT_CURSOR_COLOR;
  cursor.blink = true;
  cursor.last_update = 0; }

void Editor::run(){
  debug("run");
  const wchar_t CLASS[] = L"WindowClass";
  WNDCLASS wc = {};
  wc.lpfnWndProc = _win_proc;
  wc.hInstance = win_param_1;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);
  HWND hwnd = CreateWindowEx(
      0, CLASS, L"Window", WS_OVERLAPPEDWINDOW, 0, 0,
      width, height, NULL, NULL, win_param_1, NULL);
  assert(hwnd != NULL, "window.display", "could not create window");
  ShowWindow(hwnd, win_param_2);
  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    double ms = (double)(clock() - last_update) * 1000.0 / CLOCKS_PER_SEC;
    update(ms);
    draw();
    InvalidateRect(hwnd, NULL, FALSE); } }

void Editor::update(const double ms){
  debug("update");
  cursor.pos = point(cursor.x * CHAR_WIDTH, cursor.y * LINE_HEIGHT);
  double sec = (double)(clock() - cursor.last_update) / CLOCKS_PER_SEC;
  if(sec >= CURSOR_BLINK){
    cursor.blink = !cursor.blink;
    cursor.fill = cursor.blink ? cursor.col : CLEAR;
    cursor.last_update = clock(); }
  updated = true; }

void Editor::draw(){
  debug("draw");
  for(int y = 0; y < text.size(); ++y)
    for(int x = 0; x < text[y].size(); ++x){
      image c = font[text[y][x]];
      c.pos = point(x * CHAR_WIDTH, y * LINE_HEIGHT);
      c.draw(&frame, viewport()); }
  if(cursor.xlast != cursor.x || cursor.ylast != cursor.y){
    color c = cursor.fill;
    cursor.fill = bkgd_color;
    cursor.pos = point(cursor.xlast * CHAR_WIDTH, cursor.ylast * LINE_HEIGHT);
    cursor.draw(&frame, viewport());
    cursor.pos = point(cursor.x * CHAR_WIDTH, cursor.y * LINE_HEIGHT);
    cursor.fill = c;
    cursor.xlast = cursor.x, cursor.ylast = cursor.y; }
  cursor.draw(&frame, viewport()); }

void Editor::draw_bkgd(){
  frame.set_size(width, height);
  for(int y = 0; y < height; ++y)
    for(int x = 0; x < width; ++x)
      frame.set_pixel(x, y, bkgd_color); }

void Editor::load_font(){
  image font_img = load_bmp(_FONT_LOC);
  for(int i = 0; i < _SYMBOLS.size(); ++i){
    image new_char(CHAR_WIDTH, LINE_HEIGHT);
    for(int xi = i * CHAR_WIDTH, xo = 0; xo < CHAR_WIDTH; ++xi, ++xo)
      for(int y = 0; y < LINE_HEIGHT; ++y)
        new_char.set_pixel(xo, y, font_img.data[y][xi]);
    font[_SYMBOLS[i]] = new_char; }
  // Add space manually
  image space(CHAR_WIDTH, LINE_HEIGHT);
  for(int y = 0; y < space.height; ++y)
    for(int x = 0; x < space.width; ++x)
      space.set_pixel(x, y, CLEAR_PEN);
  font[' '] = space; }

void Editor::process_key(const str& key, const bool down, const point& mouse){
  // Modifiers
  if(key == "SHIFT")
    shift = down;
  else if(key == "CONTROL")
    ctrl = down;
  else if(key == "ALT")
    alt = down;

  // 0-9, A-Z, a-z, Space
  else if(key.size() == 1 && down && !ctrl && !alt){
    char ci = key[0], co;
    if(ci >= '0' && ci <= '9')
      co = ci;
    else{
      assert(ci >= 'A' && ci <= 'Z', "Editor.process_key", "bad character");
      if(shift)
        co = ci;
      else
        co = ci - 'A' + 'a'; }
    str line = text[cursor.y];
    str new_line = line.substr(0, cursor.x) + str(1, co)
        + line.substr(cursor.x);
    text[cursor.y] = new_line;
    ++cursor.x;
  }else if(key == "SPACE" && down){
    text[cursor.y] += str(1, ' ');
    ++cursor.x; }

  // Backspace
  else if(key == "BACKSPACE" && down && !ctrl && !alt){
    if(text[cursor.y].size() > 0){
      text[cursor.y] = text[cursor.y].substr(0, text[cursor.y].size() - 1);
      --cursor.x;
    }else if(text.size() > 1){
      text.erase(text.begin() + cursor.y);
      --cursor.y;
      cursor.x = text[cursor.y].size();
    }else
      assert(text.size() == 1 && text[0] == "", "Editor.process_key",
          "text should be empty");

  // Enter
  }else if(key == "ENTER" && down && !ctrl && !alt){
    str tail = text[cursor.y].substr(cursor.x);
    text[cursor.y] = text[cursor.y].substr(0, cursor.x);
    ++cursor.y;
    cursor.x = 0;
    text.insert(text.begin() + cursor.y, tail); }

  //! more
}

#endif
