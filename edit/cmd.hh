// COMMAND PROCESSOR

#ifndef CMDPROC_HH
#define CMDPROC_HH

#include "editor.hh"

struct CmdProc {

  void process_cmd(const str& cmd);


};

void CmdProc::process_cmd(const str& cmd){
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
