// Execute Tower Defense Game

#include "game.hh"

// Main function called on application execution
// Called by: OPERATING SYSTEM
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {
  debug_init();
  Game app;
  app.hInst = hInst;
  app.init();
  app.run();
  return 0; }
