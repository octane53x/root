// PROCESS EDITOR INPUT

#ifndef INPUT_HH
#define INPUT_HH

#include "cmd.hh"

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
  Cursor& c = p.cursor;
  if(!ctrl && !alt){
    // Single character
    char co = 0;
    if(key.size() == 1){
      char ci = key[0];
      if(ci >= '0' && ci <= '9'){
        if(shift){
          switch(ci){
          case '0': co = ')'; break;
          case '1': co = '!'; break;
          case '2': co = '@'; break;
          case '3': co = '#'; break;
          case '4': co = '$'; break;
          case '5': co = '%'; break;
          case '6': co = '^'; break;
          case '7': co = '&'; break;
          case '8': co = '*'; break;
          case '9': co = '('; break;
          default: break; }
        }else
          co = ci;
      }else{
        assert(ci >= 'A' && ci <= 'Z', "Editor.process_key", "bad character");
        co = shift ? ci : ci - 'A' + 'a'; }
    }else if(key == "COLON") co = shift ? ':' : ';';
    else if(key == "EQUALS") co = shift ? '+' : '=';
    else if(key == "COMMA") co = shift ? '<' : ',';
    else if(key == "MINUS") co = shift ? '_' : '-';
    else if(key == "PERIOD") co = shift ? '>' : '.';
    else if(key == "SLASH") co = shift ? '?' : '/';
    else if(key == "TILDE") co = '~'; //! add back apostrophe to symbols
    else if(key == "LBRACKET") co = shift ? '{' : '[';
    else if(key == "BACKSLASH") co = shift ? '|' : '\\';
    else if(key == "RBRACKET") co = shift ? '}' : ']';
    else if(key == "QUOTE") co = shift ? '"' : '\'';
    if(co != 0){
      add_char(co);
      return; }

    // Space or two
    if(key == "SPACE"){
      add_char(' ');
      if(shift)
        add_char(' ');
      return; }

    // Backspace
    if(key == "BACKSPACE"){
      if(c.x > 0){
        p.text[c.y] = p.text[c.y].substr(0, c.x - 1) + p.text[c.y].substr(c.x);
        move_cursor(LEFT);
        p.refresh_lines.insert(c.y - p.top_line);
      }else if(c.y > 0){
        move_cursor(LEFT);
        p.text[c.y] += p.text[c.y + 1];
        p.text.erase(p.text.begin() + c.y + 1);
        for(int y = c.y; y <= p.top_line + p.height / p.line_height
            && y <= p.text.size(); ++y)
          p.refresh_lines.insert(y - p.top_line); }
      return; }

    // Enter
    if(key == "ENTER"){
      if(focus == &cmd){
        focus = prev_panel;
        process_cmd(cmd.text[0]);
        cmd.text[0] = "";
        cmd.hide = true;
      }else{
        str tail = p.text[c.y].substr(c.x);
        p.text[c.y] = p.text[c.y].substr(0, c.x);
        p.text.insert(p.text.begin() + c.y + 1, tail);
        move_cursor(RIGHT);
        for(int y = c.y - 1; y <= p.top_line + p.height / p.line_height
            && y <= p.text.size(); ++y)
          p.refresh_lines.insert(y - p.top_line); }
      return; }

    // Escape: Close cmd bar
    if(key == "ESCAPE"){
      if(focus != &cmd) return;
      focus = prev_panel;
      cmd.hide = true; } }

  if(!ctrl && alt){
    // Alt+IJKL: Move cursor one position
    if(key == "I"){
      move_cursor(UP);
      return; }
    if(key == "J"){
      move_cursor(LEFT);
      return; }
    if(key == "K"){
      move_cursor(DOWN);
      return; }
    if(key == "L"){
      move_cursor(RIGHT);
      return; } }

  if(ctrl && !alt){
    // Ctrl+IJKL: Move cursor several positions
    if(key == "I"){
      move_cursor(UP);
      while(c.y > 0 && p.text[c.y] != "")
        move_cursor(UP);
      return; }
    if(key == "J"){
      while(!(c.y == 0 && c.x == 0) && !name_or_val(p.text[c.y][c.x]))
        move_cursor(LEFT);
      while(!(c.y == 0 && c.x == 0) && name_or_val(p.text[c.y][c.x]))
        move_cursor(LEFT);
      return; }
    if(key == "K"){
      move_cursor(DOWN);
      while(c.y < p.text.size() - 1 && p.text[c.y] != "")
        move_cursor(DOWN);
      return; }
    if(key == "L"){
      while(!(c.y == p.text.size() - 1 && c.x == p.text[c.y].size())
          && !name_or_val(p.text[c.y][c.x]))
        move_cursor(RIGHT);
      while(!(c.y == p.text.size() - 1 && c.x == p.text[c.y].size())
          && name_or_val(p.text[c.y][c.x]))
        move_cursor(RIGHT);
      return; }

    // Ctrl+D: Delete a character
    if(key == "D"){
      if(c.x < p.text[c.y].size()){
        p.text[c.y] = p.text[c.y].substr(0, c.x) + p.text[c.y].substr(c.x + 1);
        p.refresh_lines.insert(c.y - p.top_line);
      }else if(c.y < p.text.size() - 1){
        p.text[c.y] += p.text[c.y + 1];
        p.text.erase(p.text.begin() + c.y + 1);
        for(int y = c.y; y <= p.top_line + p.height / p.line_height
            && y <= p.text.size(); ++y)
          p.refresh_lines.insert(y - p.top_line); }
      return; }

    // Ctrl+Q: Close application
    if(key == "Q")
      quit();

    // Ctrl+O: Open file
    if(key == "O" && focus != &cmd){
      prev_panel = focus;
      focus = &cmd;
      str cwd = current_path().string();
      cmd.text[0] = "Open: " + cwd.substr(0, cwd.find("\\root\\") + 6);
      cmd.cursor.x = (int)cmd.text[0].size();
      cmd.refresh_lines.insert(0);
      return; }

    // Ctrl+(+-): Zoom text
    if(key == "EQUALS"){
      scale_font(SCALE_FACTOR);
      refresh_panel();
      return; }
    if(key == "MINUS"){
      scale_font(1.0 / SCALE_FACTOR);
      refresh_panel();
      return; } }

  if(ctrl && alt){
    // Ctrl+Alt+IJKL: Move cursor maximal position
    if(key == "I"){
      while(c.y > 0)
        move_cursor(UP);
      while(c.x > 0)
        move_cursor(LEFT);
      refresh_panel();
      return; }
    if(key == "J"){
      while(c.x > 0)
        move_cursor(LEFT);
      return; }
    if(key == "K"){
      while(c.y < p.text.size() - 1)
        move_cursor(DOWN);
      while(c.x < p.text[c.y].size())
        move_cursor(RIGHT);
      refresh_panel();
      return; }
    if(key == "L"){
      while(c.x < p.text[c.y].size())
        move_cursor(RIGHT);
      return; } } }

#endif
