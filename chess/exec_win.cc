// Execute Chess Simulator

#include "game.hh"
#include "input.hh"
#include "draw.hh"

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
