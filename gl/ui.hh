// USER INTERFACE

#ifndef UI_HH
#define UI_HH

struct Label {
  point pos;
  str text;
  Label(){}
};

struct Button {
  point pos;
  Label label;
  virtual void fn() = 0;
};

#endif
