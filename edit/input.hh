// PROCESS EDITOR INPUT

#ifndef INPUT_HH
#define INPUT_HH

#include "cmd.hh"
#include "highlight.hh" // Not used here

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

  // Cancel split
  if(p.split_ready && !(key == "K" || key == "L")){
    p.split_ready = false;
    return; }

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
      insert_text(vec<str>({str(1, co)}), c.y, c.x);
      move_cursor(RIGHT);
      return; }

    // Space or two
    if(key == "SPACE"){
      insert_text(vec<str>({" "}), c.y, c.x);
      move_cursor(RIGHT);
      if(shift){
        insert_text(vec<str>({" "}), c.y, c.x);
        move_cursor(RIGHT); }
      return; }

    // Backspace
    if(key == "BACKSPACE"){
      move_cursor(LEFT);
      remove_text(c.y, c.y, c.x, c.x);
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

  if(!ctrl && alt && !shift){
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

  if(ctrl && !alt && !shift){
    // Ctrl+IJKL: Move cursor several positions
    if(key == "I"){
      move_cursor(UP);
      while(c.y > 0 && p.text[c.y] != "")
        move_cursor(UP);
      return; }
    if(key == "J"){
      while(!(c.y == 0 && c.x == 0) && !name_or_val(c.y, c.x))
        move_cursor(LEFT);
      while(!(c.y == 0 && c.x == 0) && name_or_val(c.y, c.x))
        move_cursor(LEFT);
      return; }
    if(key == "K"){
      move_cursor(DOWN);
      while(c.y < p.text.size() - 1 && p.text[c.y] != "")
        move_cursor(DOWN);
      return; }
    if(key == "L"){
      while(!(c.y == p.text.size() - 1 && c.x == p.text[c.y].size())
          && !name_or_val(c.y, c.x))
        move_cursor(RIGHT);
      while(!(c.y == p.text.size() - 1 && c.x == p.text[c.y].size())
          && name_or_val(c.y, c.x))
        move_cursor(RIGHT);
      return; }

    // Ctrl+Backspace: Delete back several characters
    if(key == "BACKSPACE"){
      int y0 = c.y, x0 = c.x;
      while(!(c.y == 0 && c.x == 0) && !name_or_val(c.y, c.x))
        move_cursor(LEFT);
      while(!(c.y == 0 && c.x == 0) && name_or_val(c.y, c.x))
        move_cursor(LEFT);
      move_cursor(RIGHT);
      p.text[c.y] = p.text[c.y].substr(0, c.x) + p.text[y0].substr(x0);
      for(int y = y0; y > c.y; --y)
        p.text.erase(p.text.begin() + y);
      for(int y = c.y; y <= p.top_line + p.height / p.line_height
          && y <= p.text.size(); ++y)
        p.refresh_lines.insert(y - p.top_line); }

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
    if(key == "EQUALS" && focus != &cmd){
      scale_font(SCALE_FACTOR);
      refresh_panel();
      return; }
    if(key == "MINUS" && focus != &cmd){
      scale_font(1.0 / SCALE_FACTOR);
      refresh_panel();
      return; }

    // Ctrl+Space: Set/unset mark
    if(key == "SPACE" && focus != &cmd){
      if(p.ymark == -1)
        p.ymark = c.y, p.xmark = c.x;
      else{
        for(int i = min(p.ymark, c.y); i <= max(p.ymark, c.y); ++i)
          p.refresh_lines.insert(i - p.top_line);
        p.ymark = p.xmark = -1; } }

    // Ctrl+A: Select all
    if(key == "A"){
      p.ymark = (int)p.text.size() - 1, p.xmark = (int)p.text.back().size();
      c.y = c.x = 0;
      while(p.top_line != 0)
        scroll(false);
      return; }

    // Ctrl+X: Cut selection
    if(key == "X"){
      if(p.ymark == -1) return;
      clip();
      if(c.y < p.ymark || (c.y == p.ymark && c.x < p.xmark))
        remove_text(c.y, p.ymark, c.x, p.xmark - 1);
      else if(c.y > p.ymark || (c.y == p.ymark && c.x > p.xmark))
        remove_text(p.ymark, c.y, p.xmark, c.x - 1);
      else return;
      if(c.y > p.ymark || (c.y == p.ymark && c.x > p.xmark))
        c.y = p.ymark, c.x = p.xmark;
      p.ymark = p.xmark = -1;
      return; }

    // Ctrl+C: Copy selection
    if(key == "C"){
      if(p.ymark == -1) return;
      clip();
      for(int y = min(p.ymark, c.y); y <= max(p.ymark, c.y); ++y)
        p.refresh_lines.insert(y - p.top_line);
      p.ymark = p.xmark = -1;
      return; }

    // Ctrl+V: Paste seletion
    if(key == "V"){
      if(clipboard.empty()) return;
      insert_text(clipboard, c.y, c.x);
      c.y += (int)clipboard.size() - 1;
      if(clipboard.size() == 1)
        c.x += (int)clipboard.back().size();
      else
        c.x = (int)clipboard.back().size();
      return; } }

  if(ctrl && alt && !shift){
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
      return; } }

  if(ctrl && !alt && shift){
    // Ctrl+Shift+I
    if(key == "I" && focus != &cmd){
      close_panel();
      return; }

    // Ctrl+Shift+J
    if(key == "J" && focus != &cmd){
      int i;
      for(i = 0; i < panels.size(); ++i)
        if(focus == &panels[i]) break;
      p.refresh_file_bar = true;
      focus = &panels[(i == 0) ? panels.size() - 1 : i - 1];
      focus->refresh_file_bar = true;
      return; }

    // Ctrl+Shift+K
    if(key == "K" && focus != &cmd){
      if(!p.split_ready){
        p.split_ready = true;
        return;
      }else{
        p.split_ready = false;
        split_horizontal();
        return; } }

    // Ctrl+Shift+L
    if(key == "L" && focus != &cmd){
      if(p.split_ready){
        p.split_ready = false;
        split_vertical();
        return;
      }else{
        int i;
        for(i = 0; i < panels.size(); ++i)
          if(focus == &panels[i]) break;
        p.refresh_file_bar = true;
        focus = &panels[(i == panels.size() - 1) ? 0 : i + 1];
        focus->refresh_file_bar = true;
        return; } } } }

#endif
