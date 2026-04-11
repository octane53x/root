// PROCESS STUDIO INPUT

#ifndef INPUT_HH
#define INPUT_HH

#include "studio.hh"

// Processes inputs from Application
// Attempts focus panel inputs first
bool Studio::input(const KeyEvent& ke){
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
    return; } }

void Studio::escape(){
  Panel& p = *focus;
  if(&p != &cmd_panel) return;
  switch_panel(prev_panel);
  p.text[0] = "";
  p.cursor.pos = ipoint(0, 0);
  draw_cmd(true); }

void Studio::goto_line(){
  prev_panel = focus;
  switch_panel(&cmd_panel);
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.draw(true);
  p.insert_text({"Goto: "}, c.pos);
  c.pos.x = 6; }

#endif
