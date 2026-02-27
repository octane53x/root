// APPLICATION TEST

#include "../os/win/application.hh"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

struct App : Application {

  ipoint box_pos, box_size;

  void init(){
    Application::init();
    box_pos = ipoint(0, 0);
    box_size = ipoint(50, 50); }

  bool update(){
    Application::update();
    for(int y = 0; y < frame_size.y; ++y)
      for(int x = 0; x < frame_size.x; ++x)
        draw_pixel(ipoint(x, y), 0x00008888);
    for(int y = 0; y < frame_size.y; ++y)
      draw_pixel(ipoint(0, y), 0x00FF0000),
      draw_pixel(ipoint(frame_size.x - 1, y), 0x00FF0000);
    for(int x = 0; x < frame_size.x; ++x)
      draw_pixel(ipoint(x, 0), 0x00FF0000),
      draw_pixel(ipoint(x, frame_size.y - 1), 0x00FF0000);

    int dir = rand() % 4;
    if(dir == 0 && box_pos.y > 0)
      --box_pos.y;
    else if(dir == 1 && box_pos.y + box_size.y < frame_size.y)
      ++box_pos.y;
    else if(dir == 2 && box_pos.x > 0)
      --box_pos.x;
    else if(dir == 3 && box_pos.x + box_size.x < frame_size.x)
      ++box_pos.x;

    for(int y = box_pos.y; y < box_pos.y + box_size.y; ++y)
      for(int x = box_pos.x; x < box_pos.x + box_size.x; ++x)
        draw_pixel(ipoint(x, y), 0x00FFFF00);
    return true; }

  void input(const KeyEvent& ke){
    if(ke.key == "Q")
      quit(); } };

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {

  App app;
  app.hInst = hInst;
  app.init();
  app.run();

  return 0; }
