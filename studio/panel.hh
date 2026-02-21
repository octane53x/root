// STUDIO PANEL

#ifndef PANEL_HH
#define PANEL_HH

#include "cursor.hh"

struct Panel {

  ipoint pos, size;
  color bkgd;

  virtual void update() = 0;
  virtual void draw() = 0; };

struct TextPanel : Panel {

};

#endif
