// FONT INPUT

#ifndef FONT_INPUT_HH
#define FONT_INPUT_HH

#include "image.hh"
#include "os.hh"

#define FONT_LOC "../../gl/fonts/"

const str SYMBOLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
/* {.,/?:;'"+=-_\|!@#$%^&8()[]{}<> */

struct font {
  str name;
  umap<char, image> syms;
  font(){}
  font(str fname): font() { input(fname); }
  void input(str fname); };

void font::input(str fname){
  name = fname;
  for(int i = 0; i < SYMBOLS.size(); ++i){
    char c = SYMBOLS[i];
    str dir = str(FONT_LOC) + name + str("/") + str(1, c) + str(".bmp");
    image img = load_bmp(dir);
    img.fix();
    syms[c] = img; } }

#endif
