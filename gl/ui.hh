// USER INTERFACE

#ifndef UI_HH
#define UI_HH

#include "gl_incl.hh"

struct Label {
  int size; // Pixel height
  str text, font;
  point pos;
  color text_color;
  Label(){}
  //!
  void draw(Frame* f){} };

struct Button {
  point pos;
  Image image;
  Label label;
  virtual void fn() = 0;
  //!
  void draw(Frame* f){} };

#endif
