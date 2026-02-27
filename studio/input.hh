// PROCESS EDITOR INPUT

#ifndef INPUT_HH
#define INPUT_HH

#include "studio.hh"

// Returns whether the editor has updated
void Studio::input(const KeyEvent& ke){
  // Modifiers
  if(ke.key == "SHIFT"){
    shift = ke.down;
    return; }
  if(ke.key == "CONTROL"){
    ctrl = ke.down;
    return; }
  if(ke.key == "ALT"){
    alt = ke.down;
    return; }
  if(!ke.down)
    return;

  Panel& p = *focus;
  Cursor& c = p.cursor;

  // Cancel split
  if(p.split_ready && !(ke.key == "K" || ke.key == "L")){
    p.split_ready = false;
    return; }

  // Key or Shift + Key
  if(!ctrl && !alt){
    if(parse_char(ke.key)){
      updated = true;
      return; }
    if(ke.key == "TAB"){
      if(p.cmd)
        complete_file();
      else
        indent();
    }else if(ke.key == "BACKSPACE")
      backspace();
    else if(ke.key == "ENTER")
      enter();
    else if(ke.key == "ESCAPE")
      escape();
    else if(ke.key == "UP")
      p.move_cursor(UP);
    else if(ke.key == "LEFT")
      p.move_cursor(LEFT);
    else if(ke.key == "DOWN")
      p.move_cursor(DOWN);
    else if(ke.key == "RIGHT")
      p.move_cursor(RIGHT);
    else return;

  // Alt + Key
  }else if(!ctrl && alt && !shift){
    if(ke.key == "I")
      p.move_cursor(UP);
    else if(ke.key == "J")
      p.move_cursor(LEFT);
    else if(ke.key == "K")
      p.move_cursor(DOWN);
    else if(ke.key == "L")
      p.move_cursor(RIGHT);
    else return;

  // Ctrl + Key
  }else if(ctrl && !alt && !shift){
    if(ke.key == "I")
      ctrl_move(UP);
    else if(ke.key == "J")
      ctrl_move(LEFT);
    else if(ke.key == "K")
      ctrl_move(DOWN);
    else if(ke.key == "L")
      ctrl_move(RIGHT);
    else if(ke.key == "UP")
      move_max(UP);
    else if(ke.key == "LEFT")
      move_max(LEFT);
    else if(ke.key == "DOWN")
      move_max(DOWN);
    else if(ke.key == "RIGHT")
      move_max(RIGHT);
    else if(ke.key == "BACKSPACE")
      ctrl_backspace();
    else if(ke.key == "D")
      del();
    else if(ke.key == "W")
      del_space();
    else if(ke.key == "O")
      open_file();
    else if(ke.key == "S")
      save_file();
    else if(ke.key == "EQUALS")
      scale_font(SCALE_FACTOR);
    else if(ke.key == "MINUS")
      scale_font(1.0 / SCALE_FACTOR);
    else if(ke.key == "G")
      goto_line();
    else if(ke.key == "SPACE")
      set_mark();
    else if(ke.key == "A")
      select_all();
    else if(ke.key == "TAB")
      indent_selection();
    else if(ke.key == "Q")
      unindent_selection();
    else if(ke.key == "X")
      cut();
    else if(ke.key == "C")
      copy();
    else if(ke.key == "V")
      paste();
    else if(ke.key == "Z")
      undo();
    else return;

  // Ctrl + Alt + Key
  }else if(ctrl && alt && !shift){
    if(ke.key == "I")
      move_max(UP);
    else if(ke.key == "J")
      move_max(LEFT);
    else if(ke.key == "K")
      move_max(DOWN);
    else if(ke.key == "L")
      move_max(RIGHT);
    else if(ke.key == "Q")
      quit();
    else return;

  // Ctrl + Shift + Key
  }else if(ctrl && !alt && shift){
    if(ke.key == "I")
      close_panel();
    else if(ke.key == "J")
      left_panel();
    else if(ke.key == "K" && focus != &cmd_panel){
      if(p.split_ready){
        p.split_ready = false;
        split_horizontal();
      }else
        p.split_ready = true;
    }else if(ke.key == "L" && focus != &cmd_panel){
      if(p.split_ready){
        p.split_ready = false;
        split_vertical();
      }else
        right_panel();
    }else if(ke.key == "S")
      save_new_file();
    else return;

  // Unhandled modifier combo
  }else return;
  // Fall-through from successful command
  updated = true;
  return; }

// Returns true if action was taken
bool Studio::parse_char(const str& key){
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

void Studio::indent(){
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

void Studio::backspace(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1){
    if(&p == &cmd_panel){
      if(p.text[0].find("Open: ") == 0 && c.pos.x == 6) return;
      if(p.text[0].find("Save: ") == 0 && c.pos.x == 6) return;
      if(p.text[0].find("Goto: ") == 0 && c.pos.x == 6) return; }
    p.move_cursor(LEFT);
    p.remove_text(c.pos, c.pos);
  }else
    p.delete_selection(); }

void Studio::enter(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  // Execute command
  if(&p == &cmd_panel){
    switch_panel(prev_panel);
    bool success = process_cmd(p.text[0]);
    if(!success){
      switch_panel(&cmd_panel);
      return; }
    p.text[0] = "";
    c.pos = ipoint(0, 0);
    draw_all();
    return; }

  // Newline
  p.delete_selection();
  p.insert_text(vec<str>({"", ""}), c.pos);
  p.move_cursor(RIGHT);
  int spaces = 0;
  for(; spaces < p.text[c.pos.y - 1].size(); ++spaces)
    if(p.text[c.pos.y - 1][spaces] != ' ') break;
  for(int i = 0; i < spaces; ++i){
    p.insert_text(vec<str>({" "}), c.pos);
    p.move_cursor(RIGHT); } }

void Studio::escape(){
  Panel& p = *focus;
  if(&p != &cmd_panel) return;
  switch_panel(prev_panel);
  p.text[0] = "";
  p.cursor.pos = ipoint(0, 0);
  draw_cmd(true); }

void Studio::ctrl_move(const Dir d){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(d == UP){
    p.move_cursor(UP);
    while(c.pos.y > 0 && p.text[c.pos.y] != "")
      p.move_cursor(UP);
    while(c.pos.x > 0)
      p.move_cursor(LEFT);
  }else if(d == LEFT){
    p.move_cursor(LEFT);
    while(!(c.pos.y == 0 && c.pos.x == 0)
        && !name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(LEFT);
    while(!(c.pos.y == 0 && c.pos.x == 0)
        && name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(LEFT);
    if(!(c.pos.y == 0 && c.pos.x == 0))
      p.move_cursor(RIGHT);
  }else if(d == DOWN){
    p.move_cursor(DOWN);
    while(c.pos.y < p.text.size() - 1 && p.text[c.pos.y] != "")
      p.move_cursor(DOWN);
    while(c.pos.x < p.text[c.pos.y].size())
      p.move_cursor(RIGHT);
  }else if(d == RIGHT){
    while(!(c.pos.y == p.text.size() - 1 && c.pos.x == p.text[c.pos.y].size())
        && !name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(RIGHT);
    while(!(c.pos.y == p.text.size() - 1 && c.pos.x == p.text[c.pos.y].size())
        && name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(RIGHT); } }

void Studio::ctrl_backspace(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(c.pos.x > 1 && p.text[c.pos.y][c.pos.x - 1] == ' '
      && p.text[c.pos.y][c.pos.x - 2] == ' '){
    p.remove_text(ipoint(c.pos.x - 2, c.pos.y), ipoint(c.pos.x - 1, c.pos.y));
    p.move_cursor(LEFT);
    p.move_cursor(LEFT);
    return; }
  int y0 = c.pos.y, x0 = c.pos.x;
  while(!(c.pos.y == 0 && c.pos.x == 0)
      && !name_or_val(p.text[c.pos.y][c.pos.x]))
    p.move_cursor(LEFT);
  while(!(c.pos.y == 0 && c.pos.x == 0)
      && name_or_val(p.text[c.pos.y][c.pos.x])){
    p.move_cursor(LEFT);
    if(c.pos.x == p.text[c.pos.y].size())
      break; }
  if(!(c.pos.y == 0 && c.pos.x == 0))
    p.move_cursor(RIGHT);
  p.remove_text(c.pos, ipoint(x0 - 1, y0)); }

void Studio::del(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1)
    p.remove_text(c.pos, c.pos);
  else
    p.delete_selection(); }

void Studio::del_space(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  while(c.pos.x > 0 && p.text[c.pos.y][c.pos.x - 1] == ' '){
    p.move_cursor(LEFT);
    p.remove_text(c.pos, c.pos); }
  while(c.pos.x < p.text[c.pos.y].size() && p.text[c.pos.y][c.pos.x] == ' ')
    p.remove_text(c.pos, c.pos); }

void Studio::goto_line(){
  prev_panel = focus;
  switch_panel(&cmd_panel);
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.draw(true);
  p.insert_text({"Goto: "}, c.pos);
  c.pos.x = 6; }

void Studio::set_mark(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1){
    p.mark.y = c.pos.y, p.mark.x = c.pos.x;
    return; }
  ipoint p0 = (p.mark.y < c.pos.y
      || (p.mark.y == c.pos.y && p.mark.x < c.pos.x)) ? p.mark : c.pos;
  ipoint pf = (p.mark == p0) ? c.pos : p.mark;
  p.mark = ipoint(-1, -1);
  p.draw_selection(p0, ipoint(pf.x - 1, pf.y), true); }

void Studio::select_all(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.mark = ipoint((int)p.text.back().size(), (int)p.text.size() - 1);
  c.pos = ipoint(0, 0);
  p.top_line = 0;
  p.draw(true); }

void Studio::indent_selection(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1) return;
  for(int y = min(p.mark.y, c.pos.y); y <= max(p.mark.y, c.pos.y); ++y)
    if(p.text[y] != "")
      p.insert_text(vec<str>({"  "}), ipoint(0, y)); }

void Studio::unindent_selection(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1) return;
  for(int y = min(p.mark.y, c.pos.y); y <= max(p.mark.y, c.pos.y); ++y)
    if(p.text[y].size() > 1 && p.text[y][0] == ' ' && p.text[y][1] == ' ')
      p.remove_text(ipoint(0, y), ipoint(1, y)); }

void Studio::undo(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.opstack.empty()) return;
  Panel::Op op = p.opstack.back();
  p.opstack.popb();
  if(op.ins){
    ipoint pf = op.pos;
    if(op.text.size() == 1){
      pf.x += (int)op.text[0].size() - 1;
    }else{
      pf.y += (int)op.text.size() - 1;
      pf.x = (int)op.text.back().size() - 1; }
    p.remove_text(op.pos, pf);
    c.pos = op.pos;
  }else{
    p.insert_text(op.text, op.pos);
    c.pos = op.pos;
    if(op.text.size() == 1){
      c.pos.x += (int)op.text[0].size();
    }else{
      c.pos.x = (int)op.text.back().size();
      c.pos.y += (int)op.text.size() - 1; } }
  p.opstack.popb(); }

void Studio::move_max(const Dir d){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(d == UP){
    c.pos = ipoint(0, 0);
    p.top_line = 0;
    p.draw(true);
  }else if(d == LEFT){
    while(c.pos.x > 0)
      p.move_cursor(LEFT);
  }else if(d == DOWN){
    c.pos = ipoint((int)p.text.back().size(), (int)p.text.size() - 1);
    p.top_line = max(0, (int)p.text.size() - (p.size.y / p.line_height));
    p.draw(true);
  }else if(d == RIGHT){
    while(c.pos.x < p.text[c.pos.y].size())
      p.move_cursor(RIGHT); } }

#endif
