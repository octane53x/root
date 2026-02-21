// DEVELOPMENT STUDIO

#ifndef STUDIO_HH
#define STUDIO_HH

#include "../os/win/application.hh"
#include "../edit/editor.hh"
#include "../term/terminal.hh"

// Windowed application with code editor and terminal panels
struct Studio : virtual Application {

  // Whether key modifiers are held down
  bool shift, ctrl, alt;

  // Text clipboard for cut/copy/paste
  vec<str> clipboard;

  // Map of character images from disk
  // Modified images (color & scale) are a static member of TextPanel
  font font_base;

  // Panels
  Panel* cmd, *info, *focus, *prev;
  vec<Panel*> panels;

  virtual void init();
  virtual void update();
  virtual void resize();

  // Defined in draw.hh
  virtual bool draw();
  void draw_all();
  void draw_cmd(const bool blt);

  // Defined in font.hh (for now - will be moved to /gl)
  void load_font();
  void color_font(const double scale);
  image color_char(const image& img, const color& ctext, const color& cbkgd);
  void scale_font(const double factor);

  // Defined in layout.hh
  void switch_panel(Panel* p);
  void left_panel();
  void right_panel();
  void split_horizontal();
  void split_vertical();
  void close_panel();

  void clip();
  bool write_file();

  // Defined in cmd.hh
  bool process_cmd(const str& cmd);
  void complete_file();

  // Defined in input.hh
  virtual void input(const KeyEvent& ke);
  bool parse_char(const str& key);
  void indent();
  void backspace();
  void enter();
  void escape();
  void ctrl_move(const Dir d);
  void ctrl_backspace();
  void del();
  void del_space();
  void open_file();
  bool save_file();
  void save_new_file();
  void goto_line();
  void set_mark();
  void select_all();
  void indent_selection();
  void unindent_selection();
  void cut();
  void copy();
  void paste();
  void undo();
  void move_max(const Dir d); };

void Studio::init(){
  // Set default member state
  Application::init();
  start_maximized = always_draw = false;
  updated = true;
  shift = ctrl = alt = false;
  Panel::frame = Cursor::frame = &frame;

  // Find biggest screen
  GetScreens screens;
  int mi = -1, mx = -1;
  for(int i = 0; i < screens.rects.size(); ++i){
    const RECT& r = screens.rects[i];
    int area = (r.right - r.left) * (r.bottom - r.top);
    if(area > mx){
      mx = area;
      mi = i; } }
  RECT rect = screens.rects[mi];

  // Find taskbar height
  RECT work_area;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &work_area, 0);
  int taskbar_height = GetSystemMetrics(SM_CYSCREEN)
      - (work_area.bottom - work_area.top);

  // Set window size and position
  frame_size = ipoint(2 * ((PANEL_CHARS * CHAR_WIDTH_SCALE_1)
      + VERTICAL_DIVIDE),
      rect.bottom - rect.top - taskbar_height + FRAME_HEIGHT_OFFSET);
  size = ipoint(frame_size.x + WIN_WIDTH_OFFSET,
      frame_size.y + WIN_HEIGHT_OFFSET);
  pos = ipoint(rect.right - size.x + WIN_XPOS_OFFSET, rect.top);
  frame.set_size(frame_size);

  // Initial panel
  panels.pb(Panel());
  Panel& p = panels.back();
  p.size = ipoint(frame_size.x - VERTICAL_DIVIDE,
      frame_size.y - 2 * LINE_HEIGHT_SCALE_1);
  p.init();
  focus = &p;

  // Command bar
  cmd_panel.size = ipoint(frame_size.x, LINE_HEIGHT_SCALE_1);
  cmd_panel.pos = ipoint(0, p.size.y + p.line_height);
  cmd_panel.init();
  cmd_panel.cmd = true;
  cmd_panel.bkgd = cmd_panel.cursor.bkgd = CMD_BAR_COLOR;
  cmd_panel.focus = cmd_panel.cursor.blink = false;

  // Font
  load_font();
  color_font(1.0);

  // Preferences
  process_cmd("Open: C:/home/root/edit/editor.hh");
  split_vertical();
  process_cmd("Open: C:/home/root/edit/input.hh");
  left_panel();

  // Display frame
  draw_all(); }

void Studio::update(){
  Application::update();
  for(Panel* p : panels)
    p->update();
  cmd_panel->update(); }

//! probably needs to consider offsets
//! frame_size
void Studio::resize(){
  // if(nsize.x == size.x && nsize.y == size.y){
  //   win_pos = npos;
  //   return; }
  // frame.set_size(npos);
  // for(int y = 0; y < npos.y; ++y)
  //   for(int x = 0; x < npos.x; ++x)
  //     frame.data[y][x] = BKGD_COLOR;
  // double xr = (double)nsize.x / size.x;
  // double yr = (double)nsize.y / size.y;
  // for(Panel& p : panels){
  //   p.pos.x = (int)round(p.pos.x * xr);
  //   p.pos.y = (int)round(p.pos.y * yr);
  //   p.size.x = (int)round(xr * p.size.x);
  //   p.size.y = (int)round(yr * p.size.y); }
  // win_pos = npos, size = nsize;
}

void Studio::clip(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1) return;
  int y0, yf, x0, xf;
  if(c.pos.y < p.mark.y || (c.pos.y == p.mark.y && c.pos.x < p.mark.x))
    y0 = c.pos.y, yf = p.mark.y, x0 = c.pos.x, xf = p.mark.x - 1;
  else if(c.pos.y > p.mark.y || (c.pos.y == p.mark.y && c.pos.x > p.mark.x))
    y0 = p.mark.y, yf = c.pos.y, x0 = p.mark.x, xf = c.pos.x - 1;
  else return;
  clipboard.clear();
  clipboard.pb("");
  while(y0 < yf || (y0 == yf && x0 <= xf)){
    if(x0 < p.text[y0].size()){
      clipboard.back() += str(1, p.text[y0][x0]);
      ++x0;
    }else if(x0 == p.text[y0].size()){
      clipboard.pb("");
      ++y0;
      x0 = 0; } } }

void Studio::switch_panel(Panel* p){
  Panel& p0 = *focus;
  Cursor& c0 = p0.cursor;
  c0.blink = false;
  c0.fill = p0.bkgd;
  char ch = (c0.pos.x == p0.text[c0.pos.y].size())
      ? ' ' : p0.text[c0.pos.y][c0.pos.x];
  color tc = (ch == ' ') ? COLOR_CODE : p0.text_color[c0.pos.y][c0.pos.x];
  p0.draw_char(Panel::fonts[p0.text_scale][c0.fill][tc][ch],
      p0.text_to_frame(c0.pos), true);
  c0.draw(p0.text_to_frame(c0.pos), true);
  p0.focus = false;
  if(&p0 != &cmd_panel && p != &cmd_panel)
    p0.draw_file_bar(true);
  focus = p;
  p->focus = true;
  if(&p0 != &cmd_panel && p != &cmd_panel)
    p->draw_file_bar(true); }

bool Studio::write_file(){
  Panel& p = *focus;
  ofstream fs(p.dir + p.file);
  if(!fs.is_open()) return false;
  for(int y = 0; y < p.text.size(); ++y)
    fs << p.text[y] << endl;
  fs.close();
  return true; }

#endif
