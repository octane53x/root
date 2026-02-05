// Windows CL Test Window

#include <time.h>
#include <vector>
#include <windows.h>
#include "../gl/color.hh"
#include "../gl/ipoint.hh"

#include "../core/util.hh"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

#define vec std::vector
#define pb push_back

const double MAX_FPS = 60.0;

ui bmp_data[10000000]={0};

HBITMAP image_to_bmp(HDC hdc, const vec<vec<color> >& f,
    const ipoint& pos, const ipoint& size){
  for(int i = pos.y; i < pos.y + size.y; ++i)
    for(int j = pos.x; j < pos.x + size.x; ++j)
      bmp_data[(i - pos.y) * size.x + j - pos.x] = ((ui)f[i][j].r << 16)
          | ((ui)f[i][j].g << 8) | f[i][j].b;
  return CreateBitmap(size.x, size.y, 1, 32, bmp_data); }

HWND _hwnd;
clock_t last_update;
ipoint win_size;
vec<vec<color> > frame;

const ipoint frame_size = ipoint(2000, 2000);
ipoint frame_pos = ipoint(0, 0);
int phase = 1;
int shift = 5;

void init(){
  last_update = 0;
  RECT r;
  GetWindowRect(_hwnd, &r);
  win_size = ipoint(r.right - r.left, r.bottom - r.top);
  for(int y = 0; y < frame_size.y; ++y){
    frame.pb(vec<color>());
    for(int x = 0; x < frame_size.x; ++x)
      frame[y].pb((((y/100)&1) && ((x/100)&1)) ? BLUE : RED); } }

void update(){
  switch(phase){
  case 1:
    frame_pos.x += shift;
    if(frame_pos.x == 200)
      phase = 2;
    break;
  case 2:
    frame_pos.y += shift;
    if(frame_pos.y == 200)
      phase = 3;
    break;
  case 3:
    frame_pos.x -= shift;
    if(frame_pos.x == 0)
      phase = 4;
    break;
  case 4:
    frame_pos.y -= shift;
    if(frame_pos.y == 0)
      phase = 1;
    break;
  default:
    break; } }

void paint(){
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(_hwnd, &ps);
  RECT r = ps.rcPaint;
  HBITMAP bmp = image_to_bmp(hdc, frame, frame_pos, win_size);
  HDC hdcMem = CreateCompatibleDC(NULL);
  HBITMAP bmpPrev = (HBITMAP)SelectObject(hdcMem, bmp);
  BitBlt(hdc, r.left, r.top, r.right - r.left, r.bottom - r.top,
      hdcMem, 0, 0, SRCCOPY);
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
    //!
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam); } }

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {

  const char* CLASS = "WindowClass";
  const char* WINDOW = "Speed Test";
  WNDCLASS wc = {};
  wc.lpfnWndProc = _win_proc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS;
  RegisterClass(&wc);

  HWND hwnd = CreateWindowEx(
      0, CLASS, WINDOW, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
      NULL, NULL, hInst, NULL);
  if(hwnd == NULL) return -1;
  _hwnd = hwnd;
  ShowWindow(hwnd, SW_SHOWMAXIMIZED);

  init();

  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    double sec = (double)(clock() - last_update) / CLOCKS_PER_SEC;
    if(sec < 1.0 / MAX_FPS) continue;
    update();
    InvalidateRect(hwnd, NULL, FALSE);
    last_update = clock(); }

  return 0; }
