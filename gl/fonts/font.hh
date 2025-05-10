// FONT INPUT

#ifndef FONT_INPUT_HH
#define FONT_INPUT_HH

#include "../../core/os.hh"

#define FONT_LOC "../../gl/fonts/"

image fix_sym(image img){
  int left,right,top,bot;
  for(int i = 0; i < img.size.x; ++i){
    bool found = false;
    for(int j = 0; j < img.size.y; ++j)
      if(img.data[i][j] != WHITE){ found = true; break; }
    if(found){ left = i; break; } }
  for(int i = img.size.x-1; i >= 0; --i){
    bool found = false;
    for(int j = 0; j < img.size.y; ++j)
      if(img.data[i][j] != WHITE){ found = true; break; }
    if(found){ right = i; break; } }
  for(int j = 0; j < img.size.y; ++j){
    bool found = false;
    for(int i = 0; i < img.size.x; ++i)
      if(img.data[i][j] != WHITE){ found = true; break; }
    if(found){ top = j; break; } }
  for(int j = img.size.y; j >= 0; --j){
    bool found = false;
    for(int i = 0; i < img.size.x; ++i)
      if(img.data[i][j] != WHITE){ found = true; break; }
    if(found){ bot = j; break; } }
  image r(point(right-left+1, bot-top+1));
  for(int i = left; i <= right; ++i)
    for(int j = top; j <= bot; ++j)
      r.data[i].pb(img.data[i][j]);
  return r;
}

font input_font(str font_name){
  font m;
  m.name = font_name;
  for(int i = 0; i < SYMBOLS.size(); ++i){
    char c = SYMBOLS[i];
    str dir = str(FONT_LOC) + font_name + str("/") + str(1, c) + str(".bmp");
    m.syms[c] = fix_sym(load_bmp(dir)); }
  return m; }

#endif
