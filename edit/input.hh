// EDITOR INPUT

#ifndef INPUT_HH
#define INPUT_HH

#include "editor.hh"

void Editor::process_key(const str& key, const bool down, const point& mouse){
  // Modifiers
  if(key == "SHIFT"){
    shift = down;
    return; }
  if(key == "CONTROL"){
    ctrl = down;
    return; }
  if(key == "ALT"){
    alt = down;
    return; }
  if(!down) return;

  Panel& p = *focus;
  if(!ctrl && !alt){
    // Single character
    char c = 0;
    if(key.size() == 1){
      char ci = key[0];
      if(ci >= '0' && ci <= '9'){
        if(shift){
          switch(ci){
          case '0': c = ')'; break;
          case '1': c = '!'; break;
          case '2': c = '@'; break;
          case '3': c = '#'; break;
          case '4': c = '$'; break;
          case '5': c = '%'; break;
          case '6': c = '^'; break;
          case '7': c = '&'; break;
          case '8': c = '*'; break;
          case '9': c = '('; break;
          default: break; }
        }else
          c = ci;
      }else{
        assert(ci >= 'A' && ci <= 'Z', "Editor.process_key", "bad character");
        c = shift ? ci : ci - 'A' + 'a'; }
    }else if(key == "COLON") c = shift ? ':' : ';';
    else if(key == "EQUALS") c = shift ? '+' : '=';
    else if(key == "COMMA") c = shift ? '<' : ',';
    else if(key == "MINUS") c = shift ? '_' : '-';
    else if(key == "PERIOD") c = shift ? '>' : '.';
    else if(key == "SLASH") c = shift ? '?' : '/';
    else if(key == "TILDE") c = '~'; //! add back apostrophe to symbols
    else if(key == "LBRACKET") c = shift ? '{' : '[';
    else if(key == "BACKSLASH") c = shift ? '|' : '\\';
    else if(key == "RBRACKET") c = shift ? '}' : ']';
    else if(key == "QUOTE") c = shift ? '"' : '\'';
    if(c != 0){
      add_char(c);
      return; }

    // Space or two
    if(key == "SPACE"){
      add_char(' ');
      if(shift)
        add_char(' ');
      return; }

    // Backspace
    if(key == "BACKSPACE"){
      if(p.cursor.x > 0){
        p.text[p.cursor.y] = p.text[p.cursor.y].substr(0, p.cursor.x - 1)
            + p.text[p.cursor.y].substr(p.cursor.x);
        --p.cursor.x;
        p.refresh_lines.pb(p.cursor.y);
      }else if(p.cursor.y > 0){
        --p.cursor.y;
        p.cursor.x = (int)p.text[p.cursor.y].size();
        p.text[p.cursor.y] += p.text[p.cursor.y + 1];
        p.text.erase(p.text.begin() + p.cursor.y + 1);
        for(int y = p.cursor.y; y <= p.text.size(); ++y)
          p.refresh_lines.pb(y);
      }else
        assert(p.text.size() == 1 && p.text[0] == "", "process_key",
            "text should be empty");
      return; }

    // Enter
    if(key == "ENTER"){
      str tail = p.text[p.cursor.y].substr(p.cursor.x);
      p.text[p.cursor.y] = p.text[p.cursor.y].substr(0, p.cursor.x);
      ++p.cursor.y;
      p.cursor.x = 0;
      p.text.insert(p.text.begin() + p.cursor.y, tail);
      for(int y = p.cursor.y - 1; y < p.text.size(); ++y)
        p.refresh_lines.pb(y);
      return; } }

  if(!ctrl && alt){
    // Alt+IJKL
    if(key == "I"){
      if(p.cursor.y == 0){
        if(p.cursor.x > 0)
          p.cursor.x = 0;
        return; }
      --p.cursor.y;
      if(p.cursor.x > p.text[p.cursor.y].size())
        p.cursor.x = (int)p.text[p.cursor.y].size();
      return; }
    if(key == "J"){
      if(p.cursor.x == 0){
        if(p.cursor.y == 0) return;
        --p.cursor.y;
        p.cursor.x = (int)p.text[p.cursor.y].size();
      }else
        --p.cursor.x;
      return; }
    if(key == "K"){
      if(p.cursor.y == p.text.size() - 1){
        if(p.cursor.x < p.text[p.cursor.y].size())
          p.cursor.x = (int)p.text[p.cursor.y].size();
        return; }
      ++p.cursor.y;
      if(p.cursor.x > p.text[p.cursor.y].size())
        p.cursor.x = (int)p.text[p.cursor.y].size();
      return; }
    if(key == "L"){
      if(p.cursor.x == p.text[p.cursor.y].size()){
        if(p.cursor.y == p.text.size() - 1) return;
        ++p.cursor.y;
        p.cursor.x = 0;
      }else
        ++p.cursor.x;
      return; } }

  if(ctrl && !alt){
    // Ctrl+Q
    if(key == "Q")
      quit();

    // Ctrl+O
    if(key == "O" && focus != &cmd){
      focus = &cmd;
      cmd.text[0] = "Open: " + current_path().string();
      cmd.cursor.x = cmd.text[0].size();
      cmd.refresh_lines.pb(0); } }

  if(ctrl && alt){} }

#endif
