// COLOR

#ifndef COLOR_HH
#define COLOR_HH

#include "../core/thing.hh"

// An RGB color, with a few other options
struct color : virtual thing {

  // Other color options
  enum Custom : uchar {
    // Default, if RGB
    NONE,
    // Draw through this color
    CLEAR,
    // Draw clear onto the next thing
    CLEAR_PEN,
    // Draw the debug color
    DEBUG };

  // Red, green, blue values
  uchar r, g, b;
  // Alternative color
  Custom custom;

  color();
  color(const uchar _r, const uchar _g, const uchar _b);
  color(const Custom c);

  bool operator==(const color& c) const;
  bool operator!=(const color& c) const;

  virtual void validate(const str& func);
  virtual str to_str() const;

  color avg(const color& c) const;
  color random() const; };

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
const color DEFAULT_COLOR = MAGENTA;

// Set default member state
color::color(){
  *this = DEFAULT_COLOR; }

// Set to a specific RGB
color::color(const uchar _r, const uchar _g, const uchar _b):
    r(_r), g(_g), b(_b), custom(NONE) {}

// Set to an alternative color
color::color(const Custom c): r(0), g(0), b(0), custom(c) {}

// Equals comparator
bool color::operator==(const color& c) const {
  return (custom != NONE && custom == c.custom)
      || (custom == NONE && c.custom == NONE
      && r == c.r && g == c.g && b == c.b); }

// Not equals comparator
bool color::operator!=(const color& c) const {
  return !(*this == c); }

// Implemented to remove abstraction
void color::validate(const str& func){}

// Convert to text
str color::to_str() const {
  return "(" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + ")"; }

// Return the average of this color with another
color color::avg(const color& c) const {
  return color(((ui)r+c.r)>>1, ((ui)g+c.g)>>1, ((ui)b+c.b)>>1); }

// Return a random RGB
color color::random() const {
  return color((uchar)(rand() % 256), (uchar)(rand() % 256),
      (uchar)(rand() % 256)); }

#endif
