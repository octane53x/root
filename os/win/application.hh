// ABSTRACT Windows Application

//! Test mouse_pos = pixel (0, 0) to find OFFSET

#ifndef WIN_APP_HH
#define WIN_APP_HH

#include "../../gl/image.hh"
#include "util.hh"
#include "window.hh"

// Window I/O handler
struct Application : virtual Window {

  // Whether to draw the frame every update
  bool always_draw;
  // Maximum frames per second
  double max_fps;
  // Time of prior update/draw completion
  clock_t last_update, last_draw;

  virtual void init() = 0;
  virtual void update() = 0;
  virtual bool draw() = 0;
  virtual void resize() = 0;
  virtual void input(const KeyEvent& ke) = 0;
  virtual void run(); };

// Set default member state
// Called by: PROJECT init()
void Application::init(){
  Window::init();
  always_draw = true;
  max_fps = 60.0;
  last_update = last_draw = 0; }

// Send key events to derived application
// Called by: PROJECT update()
void Application::update(){
  key_router.update();
  for(const KeyEvent& ke : key_router.keys)
    input(ke); //* time
  key_router.keys.clear();
  last_update = clock(); }

// Manage draw calls to derived application
// Called by: msg_proc()
bool Application::draw(){
  double sec = (double)(clock() - last_draw) / CPS;
  if(!always_draw || sec < 1.0 / max_fps)
    return false;
  last_draw = clock();
  InvalidateRect(hWnd, NULL, FALSE);
  return true; }

// Display window and run main loop
// Called by: PROJECT
void Application::run(){
  display();
  SetTimer(hWnd, IDT_TIMER1, 1, NULL);
  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg); } }

#endif
