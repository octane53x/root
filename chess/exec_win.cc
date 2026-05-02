// Execute Chess Simulator

#include "input.hh"
#include "draw.hh"
#include "game.hh"
#include "bot.hh"

// Main function called on application execution
// Called by: OPERATING SYSTEM
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {
  debug_init();
  Chess app;
  _app = &app;
  app.hInst = hInst;
  app.init();
  app.run();
  return 0; }
