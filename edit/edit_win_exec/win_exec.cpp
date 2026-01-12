// Execute text editor in a window

#include "../code/draw.hh"
#include "../code/input.hh"
#include "../code/cmd.hh"
#include "../code/highlight.hh"

// Main function called on application execution
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {
  debug_init();
  Editor edit;
  edit.init_members(hInst, nCmdShow);
  edit.init();
  edit.run();
  return 0; }
