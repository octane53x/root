// Execute graphical Impact on Windows

#include "../global.hh"
#include "../fn_input.hh" // Leaf file, not used here

// Initial window size
const int
    INIT_WIN_W = 500,
    INIT_WIN_H = 500;

// Main function called on application execution
// Called by: OPERATING SYSTEM
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {
  time_exec = clock();
  UI_MODE = UI_WINDOW;
  imp.init_members(hInst, nCmdShow, INIT_WIN_W, INIT_WIN_H);
  imp.init();
  imp.run();
  return 0; }
