// COMMAND PROCESSOR

#ifndef CMD_HH
#define CMD_HH

#include "editor.hh"

void Editor::process_cmd(const str& cmd){
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
      p.text.pb(""); } }

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
  if(opts.size() != 1) return;

  // Display completion
  c.text[0] = "Open: " + dir + opts[0];
  c.cursor.pos = ipoint(c.text[0].size(), 0);
  c.highlight_text();
  c.draw(); }

#endif
