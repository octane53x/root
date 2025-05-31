// USER INTERFACE

#ifndef UI_HH
#define UI_HH

#include "obj.hh"

const int SYMBOL_SPACING = 5;

struct label : object {
  int size; // Pixel height
  str text;
  color text_color;
  font* font;
  label(){}
  void draw(image* f){
    int x = (int)floor(pos.x);
    for(int k = 0; k < text.size(); ++k){
      image img = font->syms[text[k]];
      img.scale((double)size / img.height);
      img.fix();
      for(int i = 0; i < img.height; ++i)
        for(int j = 0; j < img.width; ++j)
          if(img.data[i][j] != WHITE)
            f->data[(int)floor(pos.y)+i][x+j] = text_color;
      x += img.width + SYMBOL_SPACING; } } };

struct button : object {
  image img;
  label label;
  button(){}
  virtual void fn() = 0;
  void draw(image* f){} };

#endif
