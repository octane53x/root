// COLOR

#ifndef COLOR_HH
#define COLOR_HH

#include "../core/util.hh"

// An RGB color, with a few other options
struct color {

  // Other color options
  enum Custom : uchar {
    // Default, if RGB
    NONE,
    // Draw through this color
    CLEAR,
    // Draw clear onto the next thing
    CLEAR_PEN,
    // Draw the debug color
    DEBUG
  } custom;

  // Red, green, blue values
  uchar r, g, b;

  color();
  color(const uchar _r, const uchar _g, const uchar _b);
  color(const Custom c);

  bool operator==(const color& c) const;
  bool operator!=(const color& c) const;

  str to_str() const;
  bool approximately(const color& c1) const ;
  color avg(const color& c) const;

  static color random(); };

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
            LTGRAY = color(192, 192, 192),
            DKGRAY = color(64, 64, 64),
            DKDKGRAY = color(32, 32, 32),
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

// Convert to text
str color::to_str() const {
  if(custom == CLEAR) return "CLEAR";
  if(custom == CLEAR_PEN) return "CLEAR_PEN";
  if(custom == DEBUG) return "DEBUG";
  return "(" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + ")"; }

// Whether the color is approximately the other color
bool color::approximately(const color& c) const {
  int off = abs((int)r - c.r) + abs((int)g - c.g) + abs((int)b - c.b);
  return off < 64; }

// Return the average of this color with another
color color::avg(const color& c) const {
  return color(((ui)r+c.r)>>1, ((ui)g+c.g)>>1, ((ui)b+c.b)>>1); }

// Return a random RGB
color color::random(){
  return color((uchar)(rand() % 256), (uchar)(rand() % 256),
      (uchar)(rand() % 256)); }

// Allow color to be hashed as a map key
// Does not regard custom colors
namespace std {
  template <>
  struct hash<color> {
    size_t operator()(const color& c) const {
      size_t h1 = hash<uchar>()(c.r);
      size_t h2 = hash<uchar>()(c.g);
      size_t h3 = hash<uchar>()(c.b);
      return h1 ^ (h2 << 1) ^ (h3 << 2); } }; }

#endif
