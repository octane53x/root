// SYNTAX HIGHLIGHTING

#ifndef HIGHLIGHT_HH
#define HIGHLIGHT_HH

const uset<str> KEYWORDS = {
    "obj", "fn", "if", "else", "for", "while", "return", "break", "continue",
    "const", "virtual", "block", "include", "this", "operator", "template",
    "true", "false",
    "struct", "def", "delete", "in"}; // Other languages

const color
    COLOR_KEYWORD = color(0, 208, 208),
    COLOR_TYPE = color(0, 208, 82),
    COLOR_NAME = color(242, 242, 82),
    COLOR_FUNCTION = color(120, 132, 176),
    COLOR_STRING = color(242, 64, 76),
    COLOR_COMMENT = color(255, 92, 0);

void Editor::highlight_text(const int line){
  Panel& p = *focus;
  while(p.text_color.size() < p.text.size())
    p.text_color.pb(vec<color>());
  p.text_color[line].clear();
  str& t = p.text[line];

  // Harvest tokens
  vec<pair<int, int> > toks;
  int i = 0, j = 0;
  while(j < t.size()){
    while(i < t.size() && !((t[i] >= '0' && t[i] <= '9')
        || (t[i] >= 'A' && t[i] <= 'Z') || (t[i] >= 'a' && t[i] <= 'z')
        || t[i] == '_')){
      p.text_color[line].pb(WHITE);
      ++i; }
    j = i;
    while(j < t.size() && ((t[j] >= '0' && t[j] <= '9')
        || (t[j] >= 'A' && t[j] <= 'Z') || (t[j] >= 'a' && t[j] <= 'z')
        || t[j] == '_')){
      p.text_color[line].pb(WHITE);
      ++j; }
    if(i < j)
      toks.pb(pair<int, int>(i, j));
    i = j; }
  if(p.file.find(".txt") != str::npos)
    return;

  // Process tokens
  while(!toks.empty()){
    str tok = t.substr(toks[0].first, toks[0].second - toks[0].first);
    // Keyword
    if(KEYWORDS.find(tok) != KEYWORDS.end()){
      for(int i = toks[0].first; i < toks[0].second; ++i)
        p.text_color[line][i] = COLOR_KEYWORD;
      toks.erase(toks.begin());
      continue; }

    // Variable declaration
    if(toks.size() >= 2){
      int i = toks[0].second;
      if(t[i] == '&' || t[i] == '*')
        ++i;
      bool spaces = true;
      for(; i < toks[1].first; ++i)
        if(t[i] != ' '){
          spaces = false;
          break; }
      if(!spaces){
        toks.erase(toks.begin());
        continue; }
      for(i = toks[0].first; i < toks[0].second; ++i)
        p.text_color[line][i] = COLOR_TYPE;
      for(i = toks[1].first; i < toks[1].second; ++i)
        p.text_color[line][i] = COLOR_NAME;
      toks.erase(toks.begin());
      toks.erase(toks.begin());
      continue; }

    // Nothing to highlight
    toks.erase(toks.begin()); }

  // String literal
  for(int i = 0; i < t.size(); ++i)
    if(t[i] == '"' && (i == 0 || t[i-1] != '\\'))
      for(int j = i+1; j < t.size(); ++j)
        if(t[j] == '"' && t[j-1] != '\\'){
          for(int k = i; k <= j; ++k)
            p.text_color[line][k] = COLOR_STRING;
          i = j;
          break; }

  // Character literal
  for(int i = 0; i < (int)t.size() - 2; ++i)
    if(t[i] == '\'' && (i == 0 || t[i-1] != '\\') && t[i+1] != '\\'
        && t[i+2] == '\'')
      for(int j = i; j < i+3; ++j)
        p.text_color[line][j] = COLOR_STRING;

  // Comment
  for(int i = 0; i < (int)t.size() - 1; ++i)
    if(t[i] == '/' && t[i+1] == '/')
      for(int j = i; j < t.size(); ++j)
        p.text_color[line][j] = COLOR_COMMENT; }

#endif
