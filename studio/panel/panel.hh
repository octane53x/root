// STUDIO PANEL

#ifndef PANEL_HH
#define PANEL_HH

#include "cursor.hh"

// Abstract panel
struct Panel {

  ipoint pos, size;
  color bkgd;

  virtual void update() = 0;
  virtual void draw() = 0; };

// Panel that displays text in lines
struct TextPanel : Panel {

  virtual void draw() = 0;
  void draw_char(const image& img, const ipoint& p);
};

// Panel that allows text input
struct InputPanel : TextPanel {

};

#endif
