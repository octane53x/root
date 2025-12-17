// Execute text editor in a window

#include "../editor.hh"

// Main function called on application execution
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {
  debug_init();
  Editor edit;
  edit.init(hInst, nCmdShow);
  edit.run();
  return 0; }
