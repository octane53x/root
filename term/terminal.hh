// TERMINAL PANEL

#ifndef TERMINAL_HH
#define TERMINAL_HH

#include "panel.hh"

struct Terminal : TextPanel {

  void init();
  virtual void update();
  virtual void draw(); };

void Terminal::init(){

}

#endif
