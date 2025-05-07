// USER INTERFACE

#ifndef UI_HH
#define UI_HH

struct Label {
  int size; // Pixel height
  str text, font;
  point pos;
  color text_color;
  Label(){} };

struct Button {
  point pos;
  Label label;
  virtual void fn() = 0; };

#endif
