// SYNTAX HIGHLIGHTING

#ifndef HIGHLIGHT_HH
#define HIGHLIGHT_HH

const uset<str> KEYWORDS = {
    "obj", "fn", "if", "for", "return", "break", "continue", "const", "struct",
    "virtual", "block"};

const color
    COLOR_KEYWORD = color(0, 208, 208),
    COLOR_TYPE = color(0, 208, 82),
    COLOR_NAME = color(242, 242, 82),
    COLOR_FUNCTION = color(120, 132, 176),
    COLOR_STRING = color(242, 64, 76),
    COLOR_COMMENT = color(255, 92, 0);

void Editor::highlight_text(){
  Panel& p = *focus;
  p.text_color.clear();
  for(const str& t : p.text){
    p.text_color.pb(vec<color>());
    // Harvest tokens
    vec<pair<int, int> > toks;
    int i = 0, j = 0;
    while(j < t.size()){
      while(i < t.size() && !((t[i] >= '0' && t[i] <= '9')
          || (t[i] >= 'A' && t[i] <= 'Z') || (t[i] >= 'a' && t[i] <= 'z')
          || t[i] == '_')){
        p.text_color.back().pb(WHITE);
        ++i; }
      j = i;
      while(j < t.size() && ((t[j] >= '0' && t[j] <= '9')
          || (t[j] >= 'A' && t[j] <= 'Z') || (t[j] >= 'a' && t[j] <= 'z')
          || t[j] == '_')){
        p.text_color.back().pb(MAGENTA);
        ++j; }
      if(i < j)
        toks.pb(pair<int, int>(i, j));
      i = j; }

    // Process tokens
    while(!toks.empty()){
      str tok = t.substr(toks[0].first, toks[0].second - toks[0].first);
      if(KEYWORDS.find(tok) != KEYWORDS.end()){
        for(int i = toks[0].first; i < toks[0].second; ++i)
          p.text_color.back()[i] = COLOR_KEYWORD;
        toks.erase(toks.begin()); }
      //! remove
      else toks.erase(toks.begin());
    }
  }
}

#endif
