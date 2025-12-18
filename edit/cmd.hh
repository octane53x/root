// PROCESS EDITOR COMMAND

#ifndef CMD_HH
#define CMD_HH

#include "editor.hh"

void Editor::process_cmd(const str& cmd){
  // Open file
  if(cmd.find("Open") == 0){
    focus->file = cmd.substr(6);
    // Clear panel
    if(!exists(focus->file))
      for(int i = 0; i < focus->height / LINE_HEIGHT + 1
          && i + focus->top_line < focus->text.size(); ++i)
        focus->refresh_lines.pb(i + focus->top_line);
    focus->text.clear();
    focus->cursor.x = focus->cursor.y = 0;
    // Load file text
    if(exists(focus->file)){
      ifstream fs(focus->file);
      str line;
      while(getline(fs, line))
        focus->text.pb(line);
      for(int i = 0; i < focus->height / LINE_HEIGHT + 1
          && i + focus->top_line < focus->text.size(); ++i)
        focus->refresh_lines.pb(i + focus->top_line);
    }else{
      focus->text.pb("");
    } } }

#endif
