// Window Test

#include "../core/util.hh"
#include "../gl/ipoint.hh"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

#define IDT_TIMER1 223

const int FONT_SIZES[] =
    {18, 18, 18, 18, 14};
const char* FONTS[] =
    {"Courier", "Courier New", "Consolas", "Cascadia Code", "Lucida Console"};

HWND _hWnd;
HDC hdcMem;
HBITMAP bmpDIB, bmpOld;
HFONT fontOld;
vec<HFONT> fonts;

ipoint win_pos, win_size;
void* buf = NULL;

clock_t last_draw = 0;

void draw_pixel(ipoint pos, ui color){
  ui* cbuf = (ui*)buf;
  cbuf[pos.y * win_size.x + pos.x] = color; }

void init(){
  RECT r;
  GetWindowRect(_hWnd, &r);
  win_size = ipoint(r.right - r.left, r.bottom - r.top);

  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = win_size.x;
  bmi.bmiHeader.biHeight = -win_size.y;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  HDC hdc = GetDC(_hWnd);
  hdcMem = CreateCompatibleDC(hdc);
  bmpDIB = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &buf, NULL, 0);
  ReleaseDC(_hWnd, hdc);
  bmpOld = (HBITMAP)SelectObject(hdcMem, bmpDIB);

  for(int y = 0; y < win_size.y; ++y)
    for(int x = 0; x < win_size.x; ++x)
      draw_pixel(ipoint(x, y), 0x0);

  for(int i = 0; i < 5; ++i){
    fonts.pb(CreateFont(
      FONT_SIZES[i], 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
      DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
      PROOF_QUALITY, FIXED_PITCH | FF_MODERN, FONTS[i])); }

  for(int i = 0; i < fonts.size(); ++i){
    fontOld = (HFONT)SelectObject(hdcMem, fonts[i]);
    SetTextColor(hdcMem, RGB(255, 255, 255));
    SetBkMode(hdcMem, TRANSPARENT);
    TextOut(hdcMem, 0, 50 + i * 20,
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "!@#$%^&*()-=[];,./_+{}|:<>?~"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "!@#$%^&*()-=[];,./_+{}|:<>?~", 160);
    SelectObject(hdcMem, fontOld); } }

void update(){
  clock_t t0 = clock();
  clock_t tf = clock();
  double sec = (double)(tf - t0) / CPS;
  debug(str("UPDATE: ") + to_string(sec)); }

void close(){
  SelectObject(hdcMem, bmpOld);
  DeleteDC(hdcMem);
  DeleteObject(bmpDIB); }

void paint(){
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(_hWnd, &ps);
  BitBlt(hdc, 0, 0, win_size.x, win_size.y, hdcMem, 0, 0, SRCCOPY);
  EndPaint(_hWnd, &ps);

  clock_t tf = clock();
  double sec = (double)(tf - last_draw) / CPS;
  debug(str("FRAME: ") + to_string(sec));
  last_draw = tf; }

LRESULT CALLBACK _win_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  _hWnd = hWnd;
  RECT r;

  switch(uMsg){
  case WM_TIMER:
    if(wParam != IDT_TIMER1)
      return 0;
    update();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    paint();
    return 0;

  case WM_MOVE:
    GetWindowRect(_hWnd, &r);
    win_pos = ipoint(r.left, r.top);
    return 0;
  case WM_SIZE:
    GetWindowRect(_hWnd, &r);
    win_size = ipoint(r.right - r.left, r.bottom - r.top);
    return 0;

  case WM_KEYDOWN:
    //!

  case WM_DESTROY:
    close();
    PostQuitMessage(0);
    return 0;
  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam); } }

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {

  const char* CLASS = "WindowClass";
  const char* WINDOW = "Window Test";
  WNDCLASS wc = {};
  wc.lpfnWndProc = _win_proc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);

  HWND hWnd = CreateWindowEx(
      0, CLASS, WINDOW, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
      NULL, NULL, hInst, NULL);
  if(hWnd == NULL) return -1;
  _hWnd = hWnd;
  ShowWindow(hWnd, SW_SHOWMAXIMIZED);

  debug_init();
  init();

  SetTimer(hWnd, IDT_TIMER1, 1, NULL);
  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg); }
  return 0; }
