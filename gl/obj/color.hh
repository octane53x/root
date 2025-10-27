// COLOR

#ifndef COLOR_HH
#define COLOR_HH

#include "../../core/util.hh"

struct color {

  bool clear;
  uchar r, g, b;

  color(): r(0), g(0), b(0), clear(false) {}
  color(bool _clear): r(0), g(0), b(0), clear(_clear) {}
  color(const uchar _r, const uchar _g, const uchar _b): r(_r), g(_g), b(_b),
      clear(false) {}

  bool operator==(const color& c){
    return r == c.r && g == c.g && b == c.b && clear == c.clear; }
  bool operator!=(const color& c){
    return !(*this == c); }

  color avg(const color& c){
    return color(((ui)r+c.r)>>1, ((ui)g+c.g)>>1, ((ui)b+c.b)>>1); } };

const color CLEAR = color(true),
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
