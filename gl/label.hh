// LABEL

#ifndef LABEL_HH
#define LABEL_HH

#include "image.hh"
#include "font.hh"

const int SMALL_SPACING = 2,
          LARGE_SPACING = 5;

struct label : object {

  int size; // Pixel height
  str text;
  color text_color;
  font* font;

  label(){}

  virtual void validate(){
    object::validate();
    assert(size > 0, "label text size not positive"); }

  virtual point update(double ms){ return point(0, 0); }

  void draw(image* f){
    int x = (int)floor(pos.x);
    for(int k = 0; k < text.size(); ++k){
      image img = font->syms[text[k]];
      img.scale((double)size / img.height);
      //img.fix();
      for(int i = 0; i < img.height; ++i)
        for(int j = 0; j < img.width; ++j)
          if(img.data[i][j] != CLEAR)
            f->data[(int)floor(pos.y)+i][x+j] = text_color;
      x += img.width + ((size >= 100) ? LARGE_SPACING : SMALL_SPACING); } } };

#endif
