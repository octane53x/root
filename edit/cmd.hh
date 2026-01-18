// COMMAND PROCESSOR

#ifndef CMD_HH
#define CMD_HH

#include "editor.hh"

// Returns whether the cmd was successful
bool Editor::process_cmd(const str& cmd){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  // Open file
  if(cmd.find("Open: ") == 0){
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
      p.text.pb("");
    p.highlight_text();
    p.top_line = 0;
    return true;

  // Save file
  }else if(cmd.find("Save: ") == 0){
    str file = cmd.substr(6);
    if(exists(file))
      return false;
    p.file = file;
    return proc_save_file(); }
  return false; }

void Editor::complete_file(){
  assert(focus == &cmd_panel, "complete_file", "cmd panel not in focus");
  Panel& c = *focus;
  if(c.text[0].find("Open: ") != 0) return;
  str path = c.text[0].substr(6);
  int i;
  for(i = (int)path.size() - 1; i >= 0; --i)
    if(path[i] == '/') break;
  if(i < 0) return;
  str dir = path.substr(0, i + 1);
  str tok = path.substr(i + 1);

  // Get files and dirs
  vec<str> dirs, files, opts;
  for(const directory_entry& e : directory_iterator(dir)){
    str s = e.path().string();
    for(i = (int)s.size() - 1; i >= 0; --i)
      if(s[i] == '/') break;
    if(i < 0) continue;
    if(e.is_directory())
      dirs.pb(s.substr(i + 1));
    else
      files.pb(s.substr(i + 1)); }

  // Find completion candidate
  for(const str& s : dirs)
    if(starts_with(s, tok))
      opts.pb(s + "/");
  for(const str& s : files)
    if(starts_with(s, tok))
      opts.pb(s);
  if(opts.empty()) return;
  tok = "";
  if(opts.size() > 1){
    for(int i = 0; i < opts[0].size(); ++i){
      char c = opts[0][i];
      bool same = true;
      for(int j = 1; j < opts.size(); ++j){
        if(i >= opts[j].size() || opts[j][i] != c){
          same = false;
          break; } }
      if(same)
        tok += str(1, c);
      else break; }
  }else
    tok = opts[0];

  // Display completion
  c.text[0] = "Open: " + dir + tok;
  c.cursor.pos = ipoint(c.text[0].size(), 0);
  c.highlight_text();
  c.draw(); }

#endif
