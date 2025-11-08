// COLOR

#ifndef COLOR_HH
#define COLOR_HH

#include "../core/thing.hh"

struct color : thing {

  enum Custom : uchar {
    NONE,
    CLEAR,
    CLEAR_PEN,
    DEBUG };

  uchar r, g, b;
  Custom custom;

  color(){}
  color(Custom c): custom(c) {}
  color(const uchar _r, const uchar _g, const uchar _b):
      r(_r), g(_g), b(_b), custom(NONE) {}

  bool operator==(const color& c){
    return (custom != NONE && custom == c.custom)
        || (r == c.r && g == c.g && b == c.b); }
  bool operator!=(const color& c){
    return !(*this == c); }

  virtual void validate(){}

  color avg(const color& c){
    return color(((ui)r+c.r)>>1, ((ui)g+c.g)>>1, ((ui)b+c.b)>>1); } };

const color CLEAR = color(color::CLEAR),
            CLEAR_PEN = color(color::CLEAR_PEN),
            BLACK = color(0, 0, 0),
            RED = color(255, 0, 0),
            LIME = color(0, 255, 0),
            BLUE = color(0, 0, 255),
            YELLOW = color(255, 255, 0),
            MAGENTA = color(255, 0, 255),
            CYAN = color(0, 255, 255),
            WHITE = color(255, 255, 255),
            MAROON = color(128, 0, 0),
            GREEN = color(0, 128, 0),
            NAVY = color(0, 0, 128),
            OLIVE = color(128, 128, 0),
            PURPLE = color(128, 0, 128),
            TEAL = color(0, 128, 128),
            ORANGE = color(255, 165, 0),
            VIOLET = color(127, 0, 255),
            BROWN = color(150, 75, 0),
            GRAY = color(128, 128, 128),
            SILVER = color(192, 192, 192),
            GOLD = color(255, 215, 0),
            INDIGO = color(75, 0, 130),
            PINK = color(255, 192, 203);

#endif
