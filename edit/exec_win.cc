// Execute text editor in a window

#include "draw.hh"
#include "input.hh"
#include "cmd.hh"
#include "highlight.hh"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

// Main function called on application execution
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {
  debug_init();
  Editor app;
  app.hInst = hInst;
  app.init();
  app.run();
  return 0; }
