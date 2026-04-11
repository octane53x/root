// Execute Chess Simulator

#include "chess.hh"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

// Main function called on application execution
// Called by: OPERATING SYSTEM
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {
  debug_init();
  Chess app;
  app.hInst = hInst;
  app.init();
  app.run();
  return 0; }
