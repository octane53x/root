// COMMAND PROCESSOR

#ifndef CMD_HH
#define CMD_HH

#include "editor.hh"

void Editor::process_cmd(const str& cmd){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  // Open file
  if(cmd.find("Open") == 0){
    p.file = cmd.substr(6);
    // Clear panel
    p.text.clear();
    c.pos = ipoint(0, 0);
    // Load file text
    if(exists(p.file)){
      ifstream fs(p.file);
      str line;
      while(getline(fs, line))
        p.text.pb(line);
    }else
      p.text.pb(""); } }

#endif
