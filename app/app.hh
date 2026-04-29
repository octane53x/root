// APPLICATION TEMPLATE

#ifndef APP_HH
#define APP_HH

#include "../os/win/application.hh"

struct App : Application {

  void map_fns();
  void init();
  void update();
  void draw();
  void run();

  static bool fn(const KeyEvent& ke); };

void App::map_fns(){
  keymap["CTRL+A"] = fn;
  // etc
}

void App::init(){
  Application::init();
  // Initialization
}

void App::update(){
  Application::update();
  // Update
}

void App::draw(){
  // Draw
}

void App::run(){
  // Run

  Application::run(); }

bool App::fn(const KeyEvent& ke){
  // Respond to CTRL+A

  return false;
}

#endif
