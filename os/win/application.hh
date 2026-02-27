// ABSTRACT Windows Application

//! TODO
// 2 Threads:
// 1 repeatedly calls update()
// 1 handles Windows msg loop and calls BitBlt

#ifndef WIN_APP_HH
#define WIN_APP_HH

#include "util.hh"
#include "window.hh"

const double MAX_FPS = 60.0;

// Window I/O handler
struct Application : virtual Window {

  // Maximum frames per second
  double max_fps;
  // Time of prior update/draw completion
  clock_t last_update, last_draw;

  virtual void init();
  virtual bool update();
  virtual bool draw();
  virtual void input(const KeyEvent& ke) = 0;
  virtual void run(); };

// Set default member state
// Called by: PROJECT init()
void Application::init(){
  Window::init();
  max_fps = MAX_FPS;
  last_update = last_draw = 0; }

// Send key events to derived application
// Returns true if key events were routed
// Called by: PROJECT update()
bool Application::update(){
  key_router.update();
  int n = key_router.keys.size();
  for(const KeyEvent& ke : key_router.keys)
    input(ke); //* time
  key_router.keys.clear();
  last_update = clock();
  return n > 0; }

// Manage draw calls to derived application
// Returns false if draw should not be performed
// Called by: msg_proc()
bool Application::draw(){
  double sec = (double)(clock() - last_draw) / CPS;
  if(sec < 1.0 / max_fps)
    return false;
  last_draw = clock();
  return true; }

// Display window and run main loop
// Called by: PROJECT
void Application::run(){
  display();
  // Windows increases this timer resolution to its minimum (~16ms?)
  SetTimer(hWnd, IDT_TIMER1, 1 /*ms*/, NULL);
  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg); } }

#endif
