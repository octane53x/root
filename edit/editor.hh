// EDITOR

//! TODO
//!
//! Stop commands for cmd panel
//!
//! Syntax highlighting
//! Find/replace
//! Word wrap
//! Scroll bar
//! Info panel
//! Autocomplete

#ifndef EDITOR_HH
#define EDITOR_HH

#include "window.hh"
#include "panel.hh"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

// Text editor application
struct Editor : virtual window {

  bool shift, ctrl, alt;
  ipoint frame_size;
  vec<str> clipboard;
  font font_base;
  Panel cmd_panel, info_panel, *focus, *prev_panel;
  vec<Panel> panels;

  void init_members(const HINSTANCE wp1, const int wp2);

  virtual void init();
  virtual void run();
  virtual void update(const double ms);
  // Defined in input.hh
  virtual void process_key(const str& key, const bool down,
      const ipoint& mouse);

  // Defined in draw.hh
  void draw();
  // Defined in cmd_panel.hh
  bool process_cmd(const str& cmd);
  void complete_file();

  void resize(const ipoint& npos, const ipoint& nsize);
  void load_font();
  void color_font(const double scale);
  image color_char(const image& img, const color& ctext, const color& cbkgd);
  void scale_font(const double factor);
  void clip();
  void switch_panel(Panel* p);
  bool write_file();

  // Defined in input.hh
  bool parse_char(const str& key);
  void proc_indent();
  void proc_backspace();
  void proc_enter();
  void proc_escape();
  void proc_ctrl_move(const Dir d);
  void proc_ctrl_backspace();
  void proc_delete();
  void proc_del_space();
  void proc_open_file();
  bool proc_save_file();
  void proc_save_new_file();
  void proc_goto_line();
  void proc_set_mark();
  void proc_select_all();
  void proc_indent_selection();
  void proc_unindent_selection();
  void proc_cut();
  void proc_copy();
  void proc_paste();
  void proc_undo();
  void proc_move_max(const Dir d);
  void proc_left_panel();
  void proc_right_panel();
  void proc_split_horizontal();
  void proc_split_vertical();
  void proc_close_panel(); };

void Editor::init_members(const HINSTANCE wp1, const int wp2){
  win_param_1 = wp1, win_param_2 = wp2; }

void Editor::init(){
  system::init();
  _win = this;
  shift = ctrl = alt = false;
  Panel::frame = Cursor::frame = &frame;

  // Window init
  RECT rect;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
  frame_size = ipoint(2 * ((PANEL_CHARS * CHAR_WIDTH_SCALE_1)
      + VERTICAL_DIVIDE), rect.bottom - rect.top + FRAME_HEIGHT_OFFSET);
  size = ipoint(frame_size.x + WIN_WIDTH_OFFSET,
      frame_size.y + WIN_HEIGHT_OFFSET);
  win_pos = ipoint(rect.right - size.x + WIN_XPOS_OFFSET, 0);

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

  // Display frame
  frame.set_size(frame_size);
  draw(); }

void Editor::run(){
  system::run();
  for(Panel& p : panels){
    p.run();
    p.cursor.run(); }
  cmd_panel.run();
  cmd_panel.cursor.run();
  _win_init();
  _win_run(); }

void Editor::update(const double ms){
  system::update(ms);
  for(Panel& p : panels){
    p.update(ms);
    if(p.updated)
      updated = true; }
  cmd_panel.update(ms);
  if(cmd_panel.updated)
    updated = true;
  if(updated)
    last_update = clock(); }

//! probably needs to consider offsets
//! frame_size
void Editor::resize(const ipoint& npos, const ipoint& nsize){
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

// Split characters from font image
void Editor::load_font(){
  image font_img = load_bmp(_FONT_LOC);
  for(int i = 0; i < _SYMBOLS.size(); ++i){
    image c(ipoint(CHAR_WIDTH_SCALE_1, LINE_HEIGHT_SCALE_1));
    for(int xi = i * c.size.x, xo = 0; xo < c.size.x; ++xi, ++xo)
      for(int y = 0; y < c.size.y; ++y)
        c.set_pixel(ipoint(xo, y), font_img.data[y][xi]);
    font_base[_SYMBOLS[i]] = c; } }

void Editor::color_font(const double scale){
  if(Panel::fonts.find(scale) != Panel::fonts.end()) return;
  umap<color, umap<color, font> > cmbkgd;
  // Code colors
  vec<color> bkgds = {BKGD_COLOR, SELECT_COLOR};
  for(const color& cb : bkgds){
    umap<color, font> cmtext;
    for(const color& ct : TEXT_COLORS){
      font f;
      for(const char c : _SYMBOLS)
        f[c] = color_char(font_base[c], ct, cb).scale(scale);
      cmtext[ct] = f; }
    cmbkgd[cb] = cmtext; }
  // File and command bar colors
  bkgds = {CMD_BAR_COLOR, FOCUS_FILE_BAR_COLOR, UNFOCUS_FILE_BAR_COLOR,
      CURSOR_COLOR};
  for(const color& cb : bkgds){
    umap<color, font> cmtext;
    font f;
    for(const char c : _SYMBOLS)
      f[c] = color_char(font_base[c], BAR_TEXT_COLOR, cb).scale(scale);
    cmtext[BAR_TEXT_COLOR] = f;
    cmbkgd[cb] = cmtext; }
  Panel::fonts[scale] = cmbkgd; }

image Editor::color_char(const image& img, const color& ctext,
    const color& cbkgd){
  image r = img;
  for(int y = 0; y < img.size.y; ++y)
    for(int x = 0; x < img.size.x; ++x){
      color ci = img.data[y][x];
      double brt = (double)((int)ci.r + ci.g + ci.b) / (255 * 3);
      color co;
      if(brt > 0.2){
        brt = min(1.0, brt * 1.2);
        co = color((uchar)floor(brt * ctext.r),
                   (uchar)floor(brt * ctext.g),
                   (uchar)floor(brt * ctext.b));
      }else
        co = color((uchar)floor((1.0 - brt) * cbkgd.r),
                   (uchar)floor((1.0 - brt) * cbkgd.g),
                   (uchar)floor((1.0 - brt) * cbkgd.b));
      r.data[y][x] = co; }
  return r; }

void Editor::scale_font(const double factor){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.text_scale *= factor;
  c.size.y = p.line_height = (int)ceil(p.text_scale * LINE_HEIGHT_SCALE_1);
  c.size.x = p.char_width = (int)ceil(p.text_scale * CHAR_WIDTH_SCALE_1);
  color_font(p.text_scale); }

void Editor::clip(){
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

void Editor::switch_panel(Panel* p){
  Panel& p0 = *focus;
  Cursor& c0 = p0.cursor;
  c0.blink = false;
  c0.fill = p0.bkgd;
  char ch = (c0.pos.x == p0.text[c0.pos.y].size())
      ? ' ' : p0.text[c0.pos.y][c0.pos.x];
  color tc = (ch == ' ') ? COLOR_CODE : p0.text_color[c0.pos.y][c0.pos.x];
  p0.draw_char(Panel::fonts[p0.text_scale][c0.fill][tc][ch],
      p0.text_to_frame(c0.pos));
  c0.draw(p0.text_to_frame(c0.pos));
  p0.focus = false;
  if(&p0 != &cmd_panel && p != &cmd_panel)
    p0.draw_file_bar();
  focus = p;
  p->focus = true;
  if(&p0 != &cmd_panel && p != &cmd_panel)
    p->draw_file_bar(); }

bool Editor::write_file(){
  Panel& p = *focus;
  ofstream fs(p.dir + p.file);
  if(!fs.is_open()) return false;
  for(int y = 0; y < p.text.size(); ++y)
    fs << p.text[y] << endl;
  fs.close();
  return true; }

#endif
