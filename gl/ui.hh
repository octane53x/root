// USER INTERFACE

#ifndef UI_HH
#define UI_HH

#include "gl_const.hh"

struct label {
  int size; // Pixel height
  str text;
  point pos;
  color text_color;
  font* font;
  label(){}
  void draw(image* f){
    int x = pos.x;
    for(int k = 0; k < text.size(); ++k){
      image img = font->syms[text[k]];
      for(int i = 0; i < img.size.y; ++i)
        for(int j = 0; j < img.size.x; ++j)
          if(img.data[i][j] != WHITE)
            f->data[pos.y+i][x+j] = text_color;
      x += img.size.x; } } };

struct button {
  point pos;
  image img;
  label label;
  button(){}
  virtual void fn() = 0;
  //!
  void draw(image* f){} };

#endif
