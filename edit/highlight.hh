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

  // str& t = text[line];

  // // Harvest tokens
  // vec<pair<int, int> > toks;
  // int i = 0, j = 0;
  // while(j < t.size()){
  //   while(i < t.size() && !((t[i] >= '0' && t[i] <= '9')
  //       || (t[i] >= 'A' && t[i] <= 'Z') || (t[i] >= 'a' && t[i] <= 'z')
  //       || t[i] == '_')){
  //     text_color[line].pb(COLOR_CODE);
  //     ++i; }
  //   j = i;
  //   while(j < t.size() && ((t[j] >= '0' && t[j] <= '9')
  //       || (t[j] >= 'A' && t[j] <= 'Z') || (t[j] >= 'a' && t[j] <= 'z')
  //       || t[j] == '_')){
  //     text_color[line].pb(COLOR_CODE);
  //     ++j; }
  //   if(i < j)
  //     toks.pb(pair<int, int>(i, j));
  //   i = j; }
  // if(file.find(".txt") != str::npos)
  //   return;

  // // Process tokens
  // while(!toks.empty()){
  //   str tok = t.substr(toks[0].first, toks[0].second - toks[0].first);
  //   // Keyword
  //   if(KEYWORDS.find(tok) != KEYWORDS.end()){
  //     for(int i = toks[0].first; i < toks[0].second; ++i)
  //       text_color[line][i] = COLOR_KEYWORD;
  //     toks.erase(toks.begin());
  //     continue; }

  //   // Variable declaration
  //   if(toks.size() >= 2){
  //     int i = toks[0].second;
  //     if(t[i] == '&' || t[i] == '*')
  //       ++i;
  //     bool spaces = true;
  //     for(; i < toks[1].first; ++i)
  //       if(t[i] != ' '){
  //         spaces = false;
  //         break; }
  //     if(!spaces){
  //       toks.erase(toks.begin());
  //       continue; }
  //     for(i = toks[0].first; i < toks[0].second; ++i)
  //       text_color[line][i] = COLOR_TYPE;
  //     for(i = toks[1].first; i < toks[1].second; ++i)
  //       text_color[line][i] = COLOR_NAME;
  //     toks.erase(toks.begin());
  //     toks.erase(toks.begin());
  //     continue; }

  //   // Nothing to highlight
  //   toks.erase(toks.begin()); }

  // // String literal
  // for(int i = 0; i < t.size(); ++i)
  //   if(t[i] == '"' && (i == 0 || t[i-1] != '\\'))
  //     for(int j = i+1; j < t.size(); ++j)
  //       if(t[j] == '"' && t[j-1] != '\\'){
  //         for(int k = i; k <= j; ++k)
  //           text_color[line][k] = COLOR_STRING;
  //         i = j;
  //         break; }

  // // Character literal
  // for(int i = 0; i < (int)t.size() - 2; ++i)
  //   if(t[i] == '\'' && (i == 0 || t[i-1] != '\\') && t[i+1] != '\\'
  //       && t[i+2] == '\'')
  //     for(int j = i; j < i+3; ++j)
  //       text_color[line][j] = COLOR_STRING;

  // // Comment
  // for(int i = 0; i < (int)t.size() - 1; ++i)
  //   if(t[i] == '/' && t[i+1] == '/')
  //     for(int j = i; j < t.size(); ++j)
  //       text_color[line][j] = COLOR_COMMENT; }
}

#endif
