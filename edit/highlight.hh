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
  int x, y, f1, f2;

  //! Type/var

  // Keyword
  for(y = 0; y < text.size(); ++y){
    str line = text[y];
    x = 0;
    while(line != ""){
      str tok = next_tok(line);
      if(KEYWORDS.find(tok) != KEYWORDS.end())
        for(int i = x; i < x + tok.size(); ++i)
          text_color[y][i] = COLOR_KEYWORD;
      int n = line.size();
      line = delete_tok(line);
      x += n - line.size(); } }

  // String and char
  for(y = 0; y < text.size(); ++y){
    f1 = f2 = -1;
    for(x = 0; x < text[y].size(); ++x){
      if(text[y][x] == '"'){
        if(f1 != -1){
          for(int i = x; i >= f1; --i)
            text_color[y][i] = COLOR_STRING;
          f1 = -1;
        }else if(f2 == -1)
          f1 = x;
      }else if(text[y][x] == '\''){
        if(f2 != -1){
          for(int i = x; i >= f2; --i)
            text_color[y][i] = COLOR_STRING;
          f2 = -1;
        }else if(f1 == -1)
          f2 = x; } } }

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
