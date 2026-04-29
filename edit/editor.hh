// EDITOR PANEL

#ifndef EDITOR_HH
#define EDITOR_HH

#include "panel.hh"

struct Editor : TextPanel {

  // Whether the file is currently saved
  bool saved;
  // Command flags for certain operations
  // True if last pressed and pending another key
  bool ctrl_k, ctrl_h;
  // Directory and filename loaded into the edit panel
  str dir, file;
  // Language of file used for highlighting
  FileType file_type;

  // Defined in this file
  virtual bool init();
  virtual bool update();
  str file_bar_text() const;
  bool set_file_type();

  // Defined in ../draw.hh
  virtual bool draw();
  bool draw_selection(const ipoint& p0, const ipoint& pf);
  bool draw_cursor_pos();
  bool draw_divider();
  bool draw_file_bar();

  // Defined in ../input.hh
  virtual void input(const KeyEvent& ke);

  // Defined in file.hh
  bool load_file();
  bool write_file();
  bool open();
  bool save();
  bool save_new();

  // Defined in highlight.hh
  void highlight_text(const int y0, const int yf); };

ipoint Editor::text_to_frame(const ipoint& p) const {
  return ipoint(p.x * char_width + pos.x,
      (p.y - top_line) * line_height + pos.y); }

str Editor::file_bar_text() const {
  str bar_text = str(saved ? "-----" : "*****") + "     ";
  if(dir.find("/root/") != str::npos)
    bar_text += dir.substr(dir.find("/root/") + 6) + file + "     ";
  return bar_text; }

// Set size and pos prior
void Editor::init(){
  cmd = split_ready = false;
  updated = saved = focus = true;
  line_height = LINE_HEIGHT_SCALE_1;
  char_width = CHAR_WIDTH_SCALE_1;
  scroll_lines = size.y / line_height / 2;
  top_line = 0;
  text_scale = 1.0;
  bkgd = BKGD_COLOR;
  mark = ipoint(-1, -1);
  file = "";
  text.pb("");
  text_color.pb(vec<color>());
  dir = clean_path(current_path().string());
  file_type = NO_FILE_TYPE;
  cursor.init(); }

void Editor::update(){
  Cursor& c = cursor;
  if(!focus) return;
  c.update();
  if(!c.updated) return;
  c.updated = false;
  // Draw character or cursor
  char ch = (c.pos.x == text[c.pos.y].size())
      ? ' ' : text[c.pos.y][c.pos.x];
  color ct = (c.fill == CURSOR_COLOR || cmd) ? BAR_TEXT_COLOR
      : ((ch == ' ') ? COLOR_CODE : text_color[c.pos.y][c.pos.x]);
  color cb = (c.fill == bkgd && mark.y != -1 && (c.pos.y < mark.y
      || (c.pos.y == mark.y && c.pos.x < mark.x))) ? SELECT_COLOR : c.fill;
  draw_char(fonts[text_scale][cb][ct][ch], text_to_frame(c.pos), true);
  updated = true;
  last_update = clock(); }

void Editor::clean(){
  Cursor& c = cursor;
  // Delete trailing whitespace
  for(int y = 0; y < text.size(); ++y){
    int n = 0;
    for(int x = text[y].size() - 1; x >= 0; --x){
      if(text[y][x] == ' ') ++n;
      else break; }
    text[y] = text[y].substr(0, text[y].size() - n); }
  // Reset cursor
  while(c.pos.x > text[c.pos.y].size())
    --c.pos.x; }

void Editor::scroll(const Dir d){
  int lines = min((int)text.size() - top_line - 1, scroll_lines);
  if(d == UP)
    lines = min(lines, top_line);
  top_line += (d == DOWN) ? lines : -lines;
  draw(true); }

void Editor::set_file_type(){
  if(ends_with(file, ".cc") || ends_with(file, ".hh"))
    file_type = CPP;
  else if(ends_with(file, ".py"))
    file_type = PYTH;
  else if(ends_with(file, ".crab"))
    file_type = CRAB;
  else
    file_type = NO_FILE_TYPE; }

#endif
