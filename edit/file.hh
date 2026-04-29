// STUDIO FILE OPS

#ifndef FILE_HH
#define FILE_HH

#include "studio.hh"

bool Studio::write_file(){
  Panel& p = *focus;
  ofstream fs(p.dir + p.file);
  if(!fs.is_open()) return false;
  for(int y = 0; y < p.text.size(); ++y)
    fs << p.text[y] << endl;
  fs.close();
  return true; }

void Studio::open_file(){
  if(focus == &cmd_panel) return;
  prev_panel = focus;
  switch_panel(&cmd_panel);
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.draw(true);
  p.insert_text({"Open: " + prev_panel->dir}, c.pos);
  c.pos.x = (int)p.text[0].size(); }

bool Studio::save_file(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.file == ""){
    save_new_file();
    return false; }
  p.clean();
  bool success = write_file();
  if(success)
    p.saved = true;
  p.draw(true);
  return success; }

void Studio::save_new_file(){
  prev_panel = focus;
  switch_panel(&cmd_panel);
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.draw(true);
  p.insert_text({"Save: " + prev_panel->dir}, c.pos);
  c.pos.x = (int)p.text[0].size(); }

#endif
