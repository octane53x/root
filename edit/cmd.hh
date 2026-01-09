// COMMAND PROCESSOR

#ifndef CMD_HH
#define CMD_HH

#include "editor.hh"

void Editor::process_cmd(const str& cmd){
  // Open file
  if(cmd.find("Open") == 0){
    focus->file = cmd.substr(6);
    // Clear panel
    focus->text.clear();
    focus->cursor.x = focus->cursor.y = 0;
    focus->top_line = 0;
    refresh_panel();
    // Load file text
    if(exists(focus->file)){
      ifstream fs(focus->file);
      str line;
      while(getline(fs, line))
        focus->text.pb(line);
    }else
      focus->text.pb(""); } }

#endif
