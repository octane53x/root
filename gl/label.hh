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

  virtual void draw(image* bkgd){
    int x = (int)floor(pos.x);
    for(int k = 0; k < text.size(); ++k){
      image orig = font->syms[text[k]];
      image img = orig.scale((double)size / orig.height);
      img.pos = point(x, pos.y);
      img.replace_except(CLEAR, text_color);
      img.draw(bkgd);
      x += img.width + ((size >= 100) ? LARGE_SPACING : SMALL_SPACING); } } };

#endif
