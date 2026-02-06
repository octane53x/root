// Application Test

#include "../os/win/application.hh"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

struct App : Application {

  void init(){}
  void update(const double sec){}
  void draw(){}
  void resize(){}
  void input(const KeyEvent& ke){} };

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {
  debug_init();
  App app;
  app.hInst = hInst;
  app.init();
  app.execute();
  return 0; }
