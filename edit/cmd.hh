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
    // Return if directory does not exist
    str path = cmd.substr(6);
    int i;
    for(i = path.size() - 1; i >= 0; --i)
      if(path[i] == '/') break;
    if(i < 0) return false;
    str dir = path.substr(0, i + 1);
    if(!exists(dir)) return false;
    p.dir = dir;
    p.file = path.substr(i + 1);
    p.set_file_type();

    // Clear panel
    p.text.clear();
    c.pos = ipoint(0, 0);
    // Load file text
    if(exists(path)){
      ifstream fs(path);
      str line;
      while(getline(fs, line))
        p.text.pb(line);
    }else
      p.text.pb("");
    p.highlight_text();
    p.top_line = 0;
    return true;

  // Save new file
  }else if(cmd.find("Save: ") == 0){
    str path = cmd.substr(6);
    if(exists(path))
      return false;
    int i;
    for(i = path.size() - 1; i >= 0; --i)
      if(path[i] == '/') break;
    if(i < 0) return false;
    str dir = path.substr(0, i + 1);
    if(!exists(dir)) return false;
    p.dir = dir;
    p.file = path.substr(i + 1);
    p.set_file_type();
    return proc_save_file();

  // Go to line number
  }else if(cmd.find("Goto: ") == 0){
    str line = cmd.substr(6);
    for(int i = 0; i < line.size(); ++i)
      if(!is_digit(line[i]))
        return false;
    int n = stoi(line) - 1;
    if(n >= p.text.size()) return false;
    while(c.pos.y > n)
      p.move_cursor(UP);
    while(c.pos.y < n)
      p.move_cursor(DOWN);
    return true; }

  // Bad command
  return false; }

void Editor::complete_file(){
  assert(focus == &cmd_panel, "complete_file", "cmd panel not in focus");
  Panel& c = *focus;
  if(c.text[0].find("Open: ") != 0 && c.text[0].find("Save: ") != 0) return;
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
