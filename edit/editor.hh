// EDITOR

#ifndef EDITOR_HH
#define EDITOR_HH

#include "../os/win/util.hh" // load_bmp
#include "../gl/polygon.hh"
#include "window.hh"

const int
    INIT_WIN_W = 1500,
    INIT_WIN_H = 750,
    HEIGHT_OFFSET = -38,
    LINE_HEIGHT_SCALE_1 = 20,
    CHAR_WIDTH_SCALE_1 = 10,
    SCROLL_LINES = 10;
const double
    INIT_TEXT_SCALE = 0.95,
    CURSOR_BLINK = 0.5;
const color
    BKGD_COLOR = BLACK,
    CURSOR_COLOR = CYAN,
    CMD_BAR_COLOR = RED,
    FOCUS_FILE_BAR_COLOR = LTGRAY,
    UNFOCUS_FILE_BAR_COLOR = DKGRAY;
const str
    _FONT_LOC = "../symbols.bmp",
    _SYMBOLS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        ".,/?:;'\"+=-_\\|~!@#$%^&*()[]{}<>";

enum Dir { UP, LEFT, DOWN, RIGHT };

struct Editor : virtual window {

  struct Cursor : virtual polygon {
    bool blink;
    int x, y, xprev, yprev; // text position
    color col; };

  struct Panel : virtual polygon {
    bool hide;
    int width, height, top_line;
    str file;
    color col;
    vec<str> text;
    uset<int> refresh_lines;
    viewport view;
    Cursor cursor; };

  bool shift, ctrl, alt;
  int char_width, line_height;
  double text_scale;
  umap<char, image> font, font_base;
  Panel cmd, info, *focus, *prev_panel;
  vec<Panel> panels;

  bool name_or_val(const char c) const;

  void init(const HINSTANCE wp1, const int wp2);
  void run();
  void update(const double ms);
  void draw();

  void load_font();
  void scale_font(double factor);
  void process_key(const str& key, const bool down, const point& mouse);
  void process_cmd(const str& cmd);
  void set_panel(Panel* panel);
  void add_char(const char c);
  void refresh();
  void scroll(const bool down);
  void move_cursor(const Dir d); };

bool Editor::name_or_val(const char c) const {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
      || (c >= 'a' && c <= 'z') || c == '_'; }

void Editor::init(const HINSTANCE wp1, const int wp2){
  _win = this;

  // Small members
  win_param_1 = wp1, win_param_2 = wp2;
  width = INIT_WIN_W, height = INIT_WIN_H;
  updated = true;
  last_update = 0;
  shift = ctrl = alt = false;

  // Font
  line_height = LINE_HEIGHT_SCALE_1;
  char_width = CHAR_WIDTH_SCALE_1;
  load_font();
  text_scale = 1.0;
  scale_font(INIT_TEXT_SCALE);

  // Initial panel
  panels.pb(Panel());
  Panel& p = panels.back();
  p.pos = point(0, 0);
  p.width = width, p.height = height + HEIGHT_OFFSET;
  set_panel(&p);
  p.top_line = 0;
  p.file = "";
  p.col = BKGD_COLOR;
  p.text.pb("");
  focus = &p;

  // Initial panel cursor
  Cursor& c = focus->cursor;
  c.x = c.y = 0;
  c.xprev = c.yprev = 0;
  c.add(point(0, 0));
  c.add(point(char_width-1, 0));
  c.add(point(char_width-1, line_height-1));
  c.add(point(0, line_height-1));
  c.col = CURSOR_COLOR;
  c.blink = true;
  c.last_update = 0;

  // Command bar
  cmd.pos = point(0, height - line_height + HEIGHT_OFFSET);
  cmd.width = width, cmd.height = line_height;
  set_panel(&cmd);
  cmd.hide = false;
  cmd.top_line = 0;
  cmd.col = CMD_BAR_COLOR;
  cmd.text.pb("");
  cmd.cursor = c;
  cmd.cursor.blink = false;

  // Display frame
  frame.set_size(width, height);
  for(int y = 0; y < height; ++y)
    for(int x = 0; x < width; ++x)
      frame.set_pixel(x, y, BKGD_COLOR); }

void Editor::run(){
  _win_init();
  _win_run(); }

void Editor::update(const double ms){
  // Set cursor pos based on text pos
  Cursor& c = focus->cursor;
  c.pos = point(focus->pos.x + c.x * char_width,
      focus->pos.y + (c.y - focus->top_line) * line_height);
  // Blink cursor
  double sec = (double)(clock() - c.last_update) / CLOCKS_PER_SEC;
  if(sec >= CURSOR_BLINK){
    c.blink = !c.blink;
    c.fill = c.blink ? c.col : CLEAR;
    c.last_update = clock(); }
  updated = true; }

void Editor::draw(){
  // Hide cmd bar
  if(cmd.hide){
    assert(focus != &cmd, "draw", "cmd marked for refresh but still in focus");
    polygon line;
    line.pos = point(cmd.pos.x, cmd.pos.y);
    line.add(point(0, 0));
    line.add(point(cmd.width - 1, 0));
    line.add(point(cmd.width - 1, line_height - 1));
    line.add(point(0, line_height - 1));
    line.fill = BKGD_COLOR;
    line.draw(&frame, cmd.view);
    cmd.hide = false; }

  Panel& p = *focus;
  for(int y : p.refresh_lines){
    // Clear line
    polygon line;
    line.pos = point(p.pos.x, p.pos.y + y * line_height);
    line.add(point(0, 0));
    line.add(point(p.width - 1, 0));
    line.add(point(p.width - 1, line_height - 1));
    line.add(point(0, line_height - 1));
    line.fill = p.col;
    line.draw(&frame, p.view);
    if(y + p.top_line >= p.text.size()) continue;
    // Draw text
    for(int x = 0; x < p.text[y + p.top_line].size(); ++x){
      image img = font[p.text[y + p.top_line][x]];
      img.pos = point(p.pos.x + x * char_width, p.pos.y + y * line_height);
      img.draw(&frame, p.view); } }
  p.refresh_lines.clear();

  Cursor& c = p.cursor;
  if(c.xprev != c.x || c.yprev != c.y){
    // Clear last character
    color col = c.fill;
    c.fill = p.col;
    c.pos = point(p.pos.x + c.xprev * char_width,
        p.pos.y + (c.yprev - p.top_line) * line_height);
    c.draw(&frame, p.view);
    c.pos = point(p.pos.x + c.x * char_width,
        p.pos.y + (c.y - p.top_line) * line_height);
    c.fill = col;
    // Draw last character
    if(c.yprev < p.text.size() && c.xprev < p.text[c.yprev].size()){
      image img = font[p.text[c.yprev][c.xprev]];
      img.pos = point(p.pos.x + c.xprev * char_width,
          p.pos.y + (c.yprev - p.top_line) * line_height);
      img.draw(&frame, p.view); }
    c.xprev = c.x, c.yprev = c.y; }

  // Clear cursor
  color col = c.fill;
  c.fill = p.col;
  c.draw(&frame, p.view);
  c.fill = col;
  // Draw current character
  image img = font[p.text[c.y][c.x]];
  img.pos = point(p.pos.x + c.x * char_width,
      p.pos.y + (c.y - p.top_line) * line_height);
  img.draw(&frame, p.view);
  // Draw cursor
  c.draw(&frame, p.view); }

void Editor::load_font(){
  image font_img = load_bmp(_FONT_LOC);
  for(int i = 0; i < _SYMBOLS.size(); ++i){
    image c(char_width, line_height);
    for(int xi = i * char_width, xo = 0; xo < char_width; ++xi, ++xo)
      for(int y = 0; y < line_height; ++y)
        c.set_pixel(xo, y, font_img.data[y][xi]);
    font_base[_SYMBOLS[i]] = c; }
  // Add space manually //! add to symbols
  image space(char_width, line_height);
  for(int y = 0; y < space.height; ++y)
    for(int x = 0; x < space.width; ++x)
      space.set_pixel(x, y, CLEAR_PEN);
  font_base[' '] = space;
  font = font_base; }

void Editor::scale_font(double factor){
  text_scale *= factor;
  line_height = (int)ceil(text_scale * line_height);
  char_width = (int)ceil(text_scale * char_width);
  font.clear();
  for(pair<char, image> p : font_base)
    font[p.first] = p.second.scale(text_scale); }

void Editor::set_panel(Panel* panel){
  assert(panel->width > 0 && panel->height > 0, "set_panel",
      "panel size not positive");
  panel->points.clear();
  panel->add(point(0, 0));
  panel->add(point(panel->width - 1, 0));
  panel->add(point(panel->width - 1, panel->height - 1));
  panel->add(point(0, panel->height - 1)); }

void Editor::add_char(const char c){
  Panel& p = *focus;
  p.text[p.cursor.y] = p.text[p.cursor.y].substr(0, p.cursor.x) + str(1, c)
      + p.text[p.cursor.y].substr(p.cursor.x);
  ++p.cursor.x;
  p.refresh_lines.insert(p.cursor.y - p.top_line); }

void Editor::refresh(){
  for(int y = 0; y <= focus->height / line_height; ++y)
    focus->refresh_lines.insert(y); }

void Editor::scroll(const bool down){
  assert(focus != &cmd, "scroll", "cmd bar is in focus");
  int lines = min((int)focus->text.size() - focus->top_line - 1, SCROLL_LINES);
  focus->top_line += down ? lines : -lines;
  refresh(); }

void Editor::move_cursor(const Dir d){
  Cursor& c = focus->cursor;
  switch(d){
  case UP:
    if(c.y == 0){
      if(c.x > 0)
        c.x = 0;
      return; }
    --c.y;
    if(c.x > focus->text[c.y].size())
      c.x = (int)focus->text[c.y].size();
    if(c.y < focus->top_line)
      scroll(false);
    return;

  case LEFT:
    if(c.x == 0){
      if(c.y == 0) return;
      --c.y;
      c.x = (int)focus->text[c.y].size();
      if(c.y < focus->top_line)
        scroll(false);
    }else
      --c.x;
    return;

  case DOWN:
    if(c.y == focus->text.size() - 1){
      if(c.x < focus->text[c.y].size())
        c.x = (int)focus->text[c.y].size();
      return; }
    ++c.y;
    if(c.x > focus->text[c.y].size())
      c.x = (int)focus->text[c.y].size();
    if(c.y - focus->top_line >= focus->height / line_height)
      scroll(true);
    return;

  case RIGHT:
    if(c.x == focus->text[c.y].size()){
      if(c.y == focus->text.size() - 1) return;
      ++c.y;
      c.x = 0;
      if(c.y - focus->top_line >= focus->height / line_height)
        scroll(true);
    }else
      ++c.x;
    return;
  default:
    err("move_cursor", "bad direction"); } }

#endif
