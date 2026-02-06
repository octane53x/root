// Execute custom Windows application

//! #include "custom_application.hh"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

// Main function called on application execution
// Called by: OPERATING SYSTEM
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    PWSTR pCmdLine, int nCmdShow) {
  debug_init();
  //! CustomApplication app;
  app.hInst = hInst;
  app.init();
  app.execute();
  return 0; }
