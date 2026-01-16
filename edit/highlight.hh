// SYNTAX HIGHLIGHTING

#ifndef HIGHLIGHTER_HH
#define HIGHLIGHTER_HH

#include "panel.hh"

void Panel::highlight_text(){
  text_color.clear();
  while(text_color.size() < text.size())
    text_color.pb(vec<color>());
  color ct = cmd ? BAR_TEXT_COLOR : COLOR_CODE;
  for(int y = 0; y < text.size(); ++y)
    for(int x = 0; x < text[y].size(); ++x)
      text_color[y].pb(ct);
  if(cmd) return;

  bool found;
  int x, y;
  //!

  // Comment
  for(y = 0; y < text.size(); ++y){
    found = false;
    for(x = 0; x < (int)text[y].size() - 1; ++x)
      if(text[y][x] == '/' && text[y][x+1] == '/'){
        found = true;
        break; }
    for(; found && x < text[y].size(); ++x)
      text_color[y][x] = COLOR_COMMENT; }

  // Comment block
  found = false;
  x = y = 0;
  while(y < text.size()){
    if(x < (int)text[y].size() - 1 && text[y][x] == '/' && text[y][x+1] == '*'){
      text_color[y][x] = text_color[y][x+1] = COLOR_COMMENT;
      ++x;
      found = true;
    }else if(x < (int)text[y].size() - 1
        && text[y][x] == '*' && text[y][x+1] == '/'){
      text_color[y][x] = text_color[y][x+1] = COLOR_COMMENT;
      ++x;
      found = false;
    }else if(x < text[y].size() && found)
      text_color[y][x] = COLOR_COMMENT;
    ++x;
    if(x >= text[y].size()){
      ++y;
      x = 0; } } }

#endif
