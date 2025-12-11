// Execute graphical Impact on Windows

#include "../fn_input.hh"

// Initial window size
const int
    INIT_WIN_W = 500,
    INIT_WIN_H = 500;

// Main function called on application execution
// Called by: OPERATING SYSTEM
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {
  debug_init(time_exec = time(NULL));
  imp.UI_MODE = Impact::UI_WINDOW;
  imp.init_members(hInst, nCmdShow, INIT_WIN_W, INIT_WIN_H);
  imp.init();
  imp.run();
  return 0; }
