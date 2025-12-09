// FONT INPUT

#ifndef FONT_INPUT_HH
#define FONT_INPUT_HH

#include "../os/win/util.hh"

const str
    // Directory location of character images
    FONT_LOC = "../gl/fonts/",
    // All the characters in each font
    SYMBOLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    //! Not yet supported: .,/?:;'"+=-_\|!@#$%^&*()[]{}<>

// A font that displays label text
struct font : virtual thing {

  // Name on filesystem
  str name;
  // Character images
  umap<char, image> syms;

  font();
  font(const str& fname);

  virtual void validate(const str& func);

  void input(const str& fname); };

// Set default member state
font::font(){}

// Construct with a target on filesystem to load
font::font(const str& fname){
  input(fname); }

// Ensure valid state
void font::validate(const str& func){
  assert(name != "", "font.name is empty");
  assert(!syms.empty(), "font.syms is empty"); }

// Load character images off filesystem
void font::input(const str& fname){
  name = fname;
  for(int i = 0; i < SYMBOLS.size(); ++i){
    char c = SYMBOLS[i];
    str dir = str(FONT_LOC) + name + str("/") + str(1, c) + str(".bmp");
    image img = load_bmp(dir);
    img.fix(WHITE);
    img.replace(WHITE, CLEAR);
    syms[c] = img; }
  validate("font.input"); }

#endif
