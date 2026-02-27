// Sample Application

#ifndef SAMPLE_APP_HH
#define SAMPLE_APP_HH

#include "application.hh"

struct Sample : Application {

  virtual void init() = 0;
  virtual bool update() = 0;
  virtual bool draw() = 0;
  virtual void resize() = 0;
  virtual void input(const KeyEvent& ke) = 0;
  virtual void run() = 0; };

void Sample::init(){
  Application::init();
  //! ...
}

bool Sample::update(){
  // Route key events
  Application::update();
  //! ...
  last_update = clock();
  //! return true to draw
}

bool Sample::draw(){
  if(Application::draw...
  //! return
}

#endif
