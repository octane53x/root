// EDITOR

#ifndef EDITOR_HH
#define EDITOR_HH

#include "../os/win/util.hh" // load_bmp
#include "../gl/polygon.hh"
#include "window.hh"

const int
    INIT_WIN_W = 1500,
    INIT_WIN_H = 750,
    HEIGHT_OFFSET = -55,
    LINE_HEIGHT = 20,
    CHAR_WIDTH = 10;
const double
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

struct Editor : virtual window {

  struct Cursor : virtual polygon {
    bool blink;
    int x, y, xprev, yprev; // text position
    color col; };

  struct Panel : virtual polygon {
    int width, height, top_line;
    str file;
    color col;
    vec<int> refresh_lines;
    vec<str> text;
    viewport view;
    Cursor cursor; };

  bool shift, ctrl, alt;
  umap<char, image> font;
  Panel* focus;
  Panel cmd, info;
  vec<Panel> panels;

  void init(const HINSTANCE wp1, const int wp2);
  void run();
  void update(const double ms);
  void draw();
  void load_font();
  void process_key(const str& key, const bool down, const point& mouse);
  void set_panel(Panel* panel);
  void add_char(const char c); };

void Editor::init(const HINSTANCE wp1, const int wp2){
  _win = this;

  win_param_1 = wp1, win_param_2 = wp2;
  width = INIT_WIN_W, height = INIT_WIN_H;
  updated = true;
  last_update = 0;
  shift = ctrl = alt = false;

  panels.pb(Panel());
  Panel& p = panels.back();
  p.pos = point(0, 0);
  p.width = width, p.height = height;
  set_panel(&p);
  p.file = "";
  p.col = BKGD_COLOR;
  p.text.pb("");
  focus = &p;

  Cursor& c = focus->cursor;
  c.x = c.y = 0;
  c.xprev = c.yprev = 0;
  c.add(point(0, 0));
  c.add(point(CHAR_WIDTH-1, 0));
  c.add(point(CHAR_WIDTH-1, LINE_HEIGHT-1));
  c.add(point(0, LINE_HEIGHT-1));
  c.col = CURSOR_COLOR;
  c.blink = true;
  c.last_update = 0;

  cmd.pos = point(0, height - LINE_HEIGHT + HEIGHT_OFFSET);
  cmd.width = width, cmd.height = LINE_HEIGHT;
  set_panel(&cmd);
  cmd.col = CMD_BAR_COLOR;
  cmd.text.pb("");
  cmd.cursor = c;
  cmd.cursor.blink = false;

  load_font();
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
  c.pos = point(focus->pos.x + c.x * CHAR_WIDTH,
      focus->pos.y + (c.y - focus->top_line) * LINE_HEIGHT);
  // Blink cursor
  double sec = (double)(clock() - c.last_update) / CLOCKS_PER_SEC;
  if(sec >= CURSOR_BLINK){
    c.blink = !c.blink;
    c.fill = c.blink ? c.col : CLEAR;
    c.last_update = clock(); }
  updated = true; }

void Editor::draw(){
  Panel& p = *focus;
  for(int y : p.refresh_lines){
    // Clear line
    polygon line;
    line.pos = point(p.pos.x, p.pos.y + (y - p.top_line) * LINE_HEIGHT);
    line.add(point(0, 0));
    line.add(point(p.width - 1, 0));
    line.add(point(p.width - 1, LINE_HEIGHT - 1));
    line.add(point(0, LINE_HEIGHT - 1));
    line.fill = p.col;
    line.draw(&frame, p.view);
    // Draw text
    for(int x = 0; x < p.text[y].size(); ++x){
      image img = font[p.text[y][x]];
      img.pos = point(p.pos.x + x * CHAR_WIDTH,
          p.pos.y + (y - p.top_line) * LINE_HEIGHT);
      img.draw(&frame, p.view); } }
  p.refresh_lines.clear();

  Cursor& c = p.cursor;
  if(c.xprev != c.x || c.yprev != c.y){
    // Clear last character
    color col = c.fill;
    c.fill = p.col;
    c.pos = point(p.pos.x + c.xprev * CHAR_WIDTH,
        p.pos.y + (c.yprev - p.top_line) * LINE_HEIGHT);
    c.draw(&frame, p.view);
    c.pos = point(p.pos.x + c.x * CHAR_WIDTH,
        p.pos.y + (c.y - p.top_line) * LINE_HEIGHT);
    c.fill = col;
    // Draw last character
    if(c.yprev < p.text.size() && c.xprev < p.text[c.yprev].size()){
      image img = font[p.text[c.yprev][c.xprev]];
      img.pos = point(p.pos.x + c.xprev * CHAR_WIDTH,
          p.pos.y + (c.yprev - p.top_line) * LINE_HEIGHT);
      img.draw(&frame, p.view); }
    c.xprev = c.x, c.yprev = c.y; }

  // Draw current character
  image img = font[p.text[c.y][c.x]];
  img.pos = point(p.pos.x + c.x * CHAR_WIDTH,
      p.pos.y + (c.y - p.top_line) * LINE_HEIGHT);
  img.draw(&frame, p.view);

  // Draw cursor
  c.draw(&frame, p.view); }

void Editor::load_font(){
  image font_img = load_bmp(_FONT_LOC);
  for(int i = 0; i < _SYMBOLS.size(); ++i){
    image c(CHAR_WIDTH, LINE_HEIGHT);
    for(int xi = i * CHAR_WIDTH, xo = 0; xo < CHAR_WIDTH; ++xi, ++xo)
      for(int y = 0; y < LINE_HEIGHT; ++y)
        c.set_pixel(xo, y, font_img.data[y][xi]);
    font[_SYMBOLS[i]] = c; }
  // Add space manually //! add to symbols
  image space(CHAR_WIDTH, LINE_HEIGHT);
  for(int y = 0; y < space.height; ++y)
    for(int x = 0; x < space.width; ++x)
      space.set_pixel(x, y, CLEAR_PEN);
  font[' '] = space; }

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
  p.refresh_lines.pb(p.cursor.y); }

#endif
