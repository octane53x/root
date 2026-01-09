// PROCESS EDITOR INPUT

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
  Cursor& c = p.cursor;

  // Cancel split
  if(p.split_ready && !(key == "K" || key == "L")){
    p.split_ready = false;
    return; }

  // Key or Shift + Key
  if(!ctrl && !alt){
    if(parse_char(key))
      return;
    if(key == "TAB")
      proc_indent();
    else if(key == "BACKSPACE")
      proc_backspace();
    else if(key == "ENTER")
      proc_enter();
    else if(key == "ESCAPE")
      proc_escape();
    else if(key == "UP")
      move_cursor(UP);
    else if(key == "LEFT")
      move_cursor(LEFT);
    else if(key == "DOWN")
      move_cursor(DOWN);
    else if(key == "RIGHT")
      move_cursor(RIGHT); }

  // Alt + Key
  if(!ctrl && alt && !shift){
    if(key == "I")
      move_cursor(UP);
    else if(key == "J")
      move_cursor(LEFT);
    else if(key == "K")
      move_cursor(DOWN);
    else if(key == "L")
      move_cursor(RIGHT); }

  // Ctrl + Key
  if(ctrl && !alt && !shift){
    if(key == "I")
      proc_ctrl_move(UP);
    else if(key == "J")
      proc_ctrl_move(LEFT);
    else if(key == "K")
      proc_ctrl_move(DOWN);
    else if(key == "L")
      proc_ctrl_move(RIGHT);
    else if(key == "BACKSPACE")
      proc_ctrl_backspace();
    else if(key == "D")
      proc_delete();
    else if(key == "W")
      proc_del_space();
    else if(key == "O")
      proc_open_file();
    else if(key == "EQUALS")
      scale_font(SCALE_FACTOR);
    else if(key == "MINUS")
      scale_font(1.0 / SCALE_FACTOR);
    else if(key == "SPACE")
      proc_set_mark();
    else if(key == "A")
      proc_select_all();
    else if(key == "TAB")
      proc_indent_selection();
    else if(key == "Q")
      proc_unindent_selection();
    else if(key == "X")
      proc_cut();
    else if(key == "C")
      proc_copy();
    else if(key == "V")
      proc_paste(); }

  // Ctrl + Alt + Key
  if(ctrl && alt && !shift){
    if(key == "I")
      proc_move_max(UP);
    else if(key == "J")
      proc_move_max(LEFT);
    else if(key == "K")
      proc_move_max(DOWN);
    else if(key == "L")
      proc_move_max(RIGHT);
    else if(key == "Q")
      quit(); }

  // Ctrl + Shift + Key
  if(ctrl && !alt && shift){
    if(key == "I")
      proc_close_panel();
    if(key == "J")
      proc_left_panel();
    if(key == "K" && focus != &cmd){
      if(p.split_ready){
        p.split_ready = false;
        proc_split_horizontal();
      }else
        p.split_ready = true; }
    if(key == "L" && focus != &cmd){
      if(p.split_ready){
        p.split_ready = false;
        proc_split_vertical();
      }else
        proc_right_panel();
        return; } } } }

// Returns true if action was taken
bool Editor::parse_char(const str& key){
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
  else if(key == "SPACE") co = ' ';

  if(co != 0){
    Panel& p = *focus;
    Cursor& c = p.cursor;
    p.delete_selection();
    p.insert_text(vec<str>({str(1, co)}), c.pos);
    p.move_cursor(RIGHT);
    return true; }
  return false; }

void Editor::proc_indent(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(c.pos.y > 0){
    int spaces = 0;
    for(; spaces < p.text[c.pos.y - 1].size(); ++spaces)
      if(p.text[c.pos.y - 1][spaces] != ' ') break;
    if(c.pos.x < spaces){
      int x = spaces - c.pos.x;
      for(int i = 0; i < x; ++i){
        p.insert_text(vec<str>({" "}), c.pos);
        p.move_cursor(RIGHT); }
      return; } }
  if(c.pos.x & 1){
    p.insert_text(vec<str>({" "}), c.pos);
    p.move_cursor(RIGHT);
  }else{
    p.insert_text(vec<str>({"  "}), c.pos);
    p.move_cursor(RIGHT);
    p.move_cursor(RIGHT); } }

void Editor::proc_backspace(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.ymark == -1){
    p.move_cursor(LEFT);
    p.remove_text(c.pos, c.pos);
  }else
    p.delete_selection(); }

void Editor::proc_enter(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(&p == &cmd){
    focus = prev_panel;
    process_cmd(p.text[0]);
    p.text[0] = "";
    p.hide = true;
  }else{
    p.insert_text(vec<str>({"", ""}), c.pos);
    p.move_cursor(RIGHT);
    int spaces = 0;
    for(; spaces < p.text[c.pos.y - 1].size(); ++spaces)
      if(p.text[c.pos.y - 1][spaces] != ' ') break;
    for(int i = 0; i < spaces; ++i){
      p.insert_text(vec<str>({" "}), c.pos);
      p.move_cursor(RIGHT); } } }

void Editor::proc_escape(){
  Panel& p = *focus;
  if(&p != &cmd) return;
  focus = prev_panel;
  for(int y = p.y; y < p.y + p.size.y; ++y)
    for(int x = p.x; x < p.x + p.size.x; ++x)
      frame.data[y][x] = BKGD_COLOR; }

void Editor::proc_ctrl_move(const Dir d){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(d == UP){
    p.move_cursor(UP);
    while(c.pos.y > 0 && p.text[c.pos.y] != "")
      p.move_cursor(UP);
  }else if(d == LEFT){
    while(!(c.pos.y == 0 && c.pos.x == 0)
        && !name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(LEFT);
    while(!(c.y == 0 && c.x == 0) && name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(LEFT);
  }else if(d == DOWN){
    p.move_cursor(DOWN);
    while(c.pos.y < p.text.size() - 1 && p.text[c.pos.y] != "")
      p.move_cursor(DOWN);
  }else if(d == RIGHT){
    while(!(c.pos.y == p.text.size() - 1 && c.pos.x == p.text[c.pos.y].size())
        && !name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(RIGHT);
    while(!(c.pos.y == p.text.size() - 1 && c.pos.x == p.text[c.pos.y].size())
        && name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(RIGHT); } }

void Editor::proc_ctrl_backspace(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(c.x > 1 && p.text[c.y][c.x - 1] == ' ' && p.text[c.y][c.x - 2] == ' '){
    p.remove_text(c.y, c.x - 2, c.y, c.x - 1);
    p.move_cursor(LEFT);
    p.move_cursor(LEFT);
    return; }
  int y0 = c.y, x0 = c.x;
  while(!(c.y == 0 && c.x == 0) && !name_or_val(c.y, c.x))
    p.move_cursor(LEFT);
  while(!(c.y == 0 && c.x == 0) && name_or_val(c.y, c.x)){
    p.move_cursor(LEFT);
    if(c.x == p.text[c.y].size())
      break; }
  if(!(c.y == 0 && c.x == 0))
    p.move_cursor(RIGHT);
  p.remove_text(c.y, c.x, y0, x0 - 1); }

void Editor::proc_delete(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.ymark == -1)
    p.remove_text(c.pos, c.pos);
  else
    p.delete_selection(); }

void Editor::proc_del_space(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  while(c.pos.x > 0 && p.text[c.pos.y][c.pos.x - 1] == ' '){
    p.move_cursor(LEFT);
    p.remove_text(c.pos, c.pos); }
  while(c.pos.x < p.text[c.pos.y].size() && p.text[c.pos.y][c.pos.x] == ' ')
    p.remove_text(c.pos, c.pos); }

void Editor::proc_open_file(){
  if(focus == &cmd) return;
  prev_panel = focus;
  focus = &cmd;
  Panel& p = *focus;
  Cursor& c = p.cursor;
  str cwd = current_path().string();
  p.insert_text({"Open: " + cwd.substr(0, cwd.find("\\root\\") + 6)}, c.pos);
  c.pos.x = (int)p.text[0].size(); }

void Editor::proc_set_mark(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1){
    p.mark.y = c.pos.y, p.mark.x = c.pos.x;
    return; }
  ipoint p0 = (p.mark.y < c.pos.y
      || (p.mark.y == c.pos.y && p.mark.x < c.pos.x)) ? p.mark : c.pos;
  ipoint pf = (p.mark == p0) ? c.pos : p.mark;
  p.draw_selection(p0, pf);
  p.mark = ipoint(-1, -1); }

void Editor::proc_select_all(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.mark.y = (int)p.text.size() - 1, p.mark.x = (int)p.text.back().size();
  c.pos = ipoint(0, 0);
  while(p.top_line != 0)
    p.scroll(DOWN);
  p.draw(); }

void Editor::proc_indent_selection(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1) return;
  for(int y = min(p.mark.y, c.pos.y); y <= max(p.mark.y, c.pos.y); ++y)
    if(p.text[y] != "")
      p.insert_text(vec<str>({"  "}), ipoint(0, y)); }

void Editor::proc_unindent_selection(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1) return;
  for(int y = min(p.mark.y, c.pos.y); y <= max(p.mark.y, c.pos.y); ++y)
    if(p.text[y].size() > 1 && p.text[y][0] == ' ' && p.text[y][1] == ' ')
      p.remove_text(ipoint(0, y), ipoint(1, y)); }

void Editor::proc_cut(){
  Panel& p = *focus;
  if(p.mark.y == -1) return;
  clip();
  p.delete_selection(); }

void Editor::proc_copy(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1) return;
  clip();
  ipoint p0 = (p.mark.y < c.pos.y
      || (p.mark.y == c.pos.y && p.mark.x < c.pos.x)) ? p.mark : c.pos;
  ipoint pf = (p.mark == p0) ? c.pos : p.mark;
  p.ymark = p.xmark = -1;
  p.draw_selection(p0, pf); }

void Editor::proc_paste(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(clipboard.empty()) return;
  p.insert_text(clipboard, c.pos);
  c.pos.y += (int)clipboard.size() - 1;
  if(clipboard.size() == 1)
    c.pos.x += (int)clipboard.back().size();
  else
    c.pos.x = (int)clipboard.back().size(); }

void Editor::proc_move_max(const Dir d){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(d == UP){
    c.pos = ipoint(0, 0);
    p.top_line = 0;
    p.draw();
  }else if(d == LEFT){
    while(c.pos.x > 0)
      move_cursor(LEFT);
  }else if(d == DOWN){
    c.pos = ipoint(p.text.back.size(), p.text.size() - 1);
    p.top_line = max(0, p.text.size() - (p.size.y / p.line_height));
    p.draw();
  }else if(d == RIGHT){
    while(c.pos.x < p.text[c.pos.y].size())
      move_cursor(RIGHT); } }

void Editor::proc_left_panel(){
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  focus->draw_file_bar();
  focus = &panels[(i == 0) ? panels.size() - 1 : i - 1];
  focus->draw_file_bar(); }

void Editor::proc_right_panel(){
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  focus->draw_file_bar();
  focus = &panels[(i == panels.size() - 1) ? 0 : i + 1];
  focus->draw_file_bar(); }

void Editor::proc_split_horizontal(){
  Panel& p = *focus;
  p.size.y = (p.size.y + LINE_HEIGHT_SCALE_1) / 2 - LINE_HEIGHT_SCALE_1;
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  if(i + 1 == panels.size())
    panels.pb(p);
  else
    panels.insert(panels.begin() + i + 1, p);
  Panel& p2 = panels[i];
  Panel& p3 = panels[i + 1];
  p3.pos = ipoint(p2.pos.x, p2.pos.y + p2.size.y + LINE_HEIGHT_SCALE_1);
  focus = &p3;
  p2.draw();
  p3.draw(); }

void Editor::proc_split_vertical(){
  Panel& p = *focus;
  p.size.x = (p.size.x + VERTICAL_DIVIDE) / 2 - VERTICAL_DIVIDE;
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  if(i + 1 == panels.size())
    panels.pb(p);
  else
    panels.insert(panels.begin() + i + 1, p);
  Panel& p2 = panels[i];
  Panel& p3 = panels[i + 1];
  p3.pos = ipoint(p2.pos.x + p2.size.x + VERTICAL_DIVIDE, p2.pos.y);
  focus = &p3;
  p2.draw();
  p3.draw(); }

void Editor::proc_close_panel(){
  //! difficult ???
}

#endif
