// EDITOR

#ifndef EDITOR_HH
#define EDITOR_HH

#include "../os/win/util.hh" // load_bmp
#include "../gl/polygon.hh"
#include "window.hh"

const int
    INIT_WIN_W = 1500,
    INIT_WIN_H = 750,
    WIDTH_OFFSET = -16,
    HEIGHT_OFFSET = -38,
    LINE_HEIGHT_SCALE_1 = 20,
    CHAR_WIDTH_SCALE_1 = 10,
    VERTICAL_DIVIDE = 20,
    SCROLL_LINES = 10;

const double
    INIT_TEXT_SCALE = 1.0,
    SCALE_FACTOR = 1.1,
    CURSOR_BLINK = 0.5;

const color
    BKGD_COLOR = BLACK,
    CURSOR_COLOR = CYAN,
    CMD_BAR_COLOR = RED,
    CMD_TEXT_COLOR = BLACK,
    FOCUS_FILE_BAR_COLOR = LTGRAY,
    UNFOCUS_FILE_BAR_COLOR = DKGRAY,
    SELECT_COLOR = BLUE;

const str
    _FONT_LOC = "../symbols.bmp",
    _SYMBOLS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        ".,/?:;'\"+=-_\\|~!@#$%^&*()[]{}<>";

enum Dir { UP, LEFT, DOWN, RIGHT };

// Text editor application
struct Editor : virtual window {

  struct Cursor : virtual polygon {
    bool blink;
    int x, y, xprev, yprev; // text position
    color col; };

  struct Panel : virtual polygon {
    bool hide, saved, refresh_file_bar, refresh_divider, split_ready;
    int width, height, top_line, line_height, char_width, ymark, xmark;
    double text_scale;
    str file;
    color col;
    vec<str> text;
    vec<vec<color> > text_color;
    uset<int> refresh_lines;
    umap<char, image> font;
    viewport view;
    Cursor cursor; };

  bool shift, ctrl, alt;
  int default_line_height, default_char_width;
  vec<str> clipboard;
  umap<char, image> default_font, font_base;
  Panel cmd, info, *focus, *prev_panel;
  vec<Panel> panels;

  void init(const HINSTANCE wp1, const int wp2);
  void run();
  void update(const double ms);
  void draw();

  bool name_or_val(int y, int x) const;

  void load_font();
  void scale_font(double factor);
  void draw_char(const image& img, const point& p, const color& c);
  void insert_text(const vec<str>& text, const int y, const int x);
  void remove_text(const int y0, const int x0, const int yf, const int xf);
  void delete_selection();
  void refresh_panel();
  void scroll(const bool down);
  void move_cursor(const Dir d);
  void clip();
  void split_horizontal();
  void split_vertical();
  void close_panel();

  // Defined outside this file
  void process_key(const str& key, const bool down, const point& mouse);
  void process_cmd(const str& cmd);
  void highlight_text(const int line); };

void Editor::init(const HINSTANCE wp1, const int wp2){
  _win = this;

  // Small members
  win_param_1 = wp1, win_param_2 = wp2;
  width = INIT_WIN_W, height = INIT_WIN_H;
  updated = true;
  last_update = 0;
  shift = ctrl = alt = false;

  // Initial panel
  panels.pb(Panel());
  Panel& p = panels.back();
  p.line_height = LINE_HEIGHT_SCALE_1;
  p.char_width = CHAR_WIDTH_SCALE_1;
  p.text_scale = 1.0;
  p.pos = point(0, 0);
  p.width = width - VERTICAL_DIVIDE + WIDTH_OFFSET;
  p.height = height - p.line_height * 2 + HEIGHT_OFFSET;
  p.set_box(p.width, p.height);
  p.top_line = 0;
  p.file = "";
  p.col = BKGD_COLOR;
  p.text.pb("");
  p.ymark = p.xmark = -1;
  p.saved = true;
  p.refresh_file_bar = true;
  p.refresh_divider = true;
  p.split_ready = false;
  focus = &p;

  // Initial panel cursor
  Cursor& c = focus->cursor;
  c.x = c.y = 0;
  c.xprev = c.yprev = 0;
  c.add(point(0, 0));
  c.add(point(p.char_width-1, 0));
  c.add(point(p.char_width-1, p.line_height-1));
  c.add(point(0, p.line_height-1));
  c.col = CURSOR_COLOR;
  c.blink = true;
  c.last_update = 0;

  // Command bar
  cmd.line_height = LINE_HEIGHT_SCALE_1;
  cmd.char_width = CHAR_WIDTH_SCALE_1;
  cmd.text_scale = 1.0;
  cmd.pos = point(0, p.height + p.line_height);
  cmd.width = width;
  cmd.height = cmd.line_height;
  cmd.set_box(cmd.width, cmd.height);
  cmd.hide = false;
  cmd.top_line = 0;
  cmd.col = CMD_BAR_COLOR;
  cmd.text.pb("");
  cmd.ymark = p.xmark = -1;
  cmd.cursor = c;
  cmd.cursor.blink = false;
  cmd.split_ready = false;

  // Font
  load_font();
  scale_font(INIT_TEXT_SCALE);
  default_line_height = p.line_height;
  default_char_width = p.char_width;
  default_font = p.font;
  focus = &cmd;
  scale_font(INIT_TEXT_SCALE);
  focus = &p;

  // Display frame
  frame.set_size(width, height);
  for(int y = 0; y < height; ++y)
    for(int x = 0; x < width; ++x)
      frame.set_pixel(x, y, BKGD_COLOR); }

void Editor::run(){
  _win_init();
  _win_run(); }

void Editor::update(const double ms){
  assert(cmd.ymark == -1, "update", "mark set in cmd bar");
  // Update text color
  if(focus == &cmd){
    cmd.text_color.clear();
    cmd.text_color.pb(vec<color>());
    for(int i = 0; i < cmd.text[0].size(); ++i)
      cmd.text_color.back().pb(CMD_TEXT_COLOR); }
  // Set cursor pos based on text pos
  Panel& p = *focus;
  Cursor& c = p.cursor;
  c.pos = point(p.pos.x + c.x * p.char_width,
      p.pos.y + (c.y - p.top_line) * p.line_height);
  // Blink cursor
  double sec = (double)(clock() - c.last_update) / CLOCKS_PER_SEC;
  if(sec >= CURSOR_BLINK){
    c.blink = !c.blink;
    c.fill = c.blink ? c.col : CLEAR;
    c.last_update = clock(); }
  updated = true; }

void Editor::draw(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  for(int y : p.refresh_lines){
    if(y < 0 || y > p.height / p.line_height)
      err("draw", "bad refresh line");
    int yl = y + p.top_line;

    // Determine selection
    int xstart = -1, xend = -1;
    if(p.ymark != -1){
      // Cursor before mark
      if(c.y < p.ymark || (c.y == p.ymark && c.x < p.xmark)){
        if(c.y < yl)
          xstart = 0;
        else if(c.y == yl)
          xstart = c.x;
        if(p.ymark > yl)
          xend = (int)p.text[yl].size();
        else if(p.ymark == yl)
          xend = p.xmark - 1;

      // Mark before cursor
      }else if(c.y > p.ymark || (c.y == p.ymark && c.x > p.xmark)){
        if(p.ymark < yl)
          xstart = 0;
        else if(p.ymark == yl)
          xstart = p.xmark;
        if(c.y > yl)
          xend = (int)p.text[yl].size();
        else if(c.y == yl)
          xend = c.x - 1; } }
    if(yl < p.text.size())
      assert(xend <= (int)p.text[yl].size(), "draw", "bad selection end point");

    // Line background
    if(xstart != 0 || xend == -1){
      polygon line;
      line.pos = point(p.pos.x, p.pos.y + y * p.line_height);
      int w = (xstart == -1 || xend == -1) ? p.width : xstart * p.char_width;
      line.set_box(w, p.line_height);
      line.fill = p.col;
      line.draw(&frame, p.view); }

    // Selection background
    if(xstart != -1 && xend != -1){
      polygon select;
      select.pos = point(p.pos.x + xstart * p.char_width,
          p.pos.y + y * p.line_height);
      int w = (xend == p.text[yl].size()) ? p.width - xstart * p.char_width
          : (xend - xstart + 1) * p.char_width;
      select.set_box(w, p.line_height);
      select.fill = SELECT_COLOR;
      select.draw(&frame, p.view); }

    // Line background
    if(xstart != -1 && xend != -1 && xend < p.text[yl].size()){
      polygon line;
      line.pos = point(p.pos.x + (xend + 1) * p.char_width,
          p.pos.y + y * p.line_height);
      line.set_box(p.width - (xend + 1) * p.char_width, p.line_height);
      line.fill = p.col;
      line.draw(&frame, p.view); }

    // Draw text
    if(yl >= p.text.size()) continue;
    if(&p != &cmd)
      highlight_text(yl);
    for(int x = 0; x < p.text[yl].size(); ++x)
      draw_char(p.font[p.text[yl][x]],
          point(p.pos.x + x * p.char_width, p.pos.y + y * p.line_height),
          p.text_color[yl][x]); }
  p.refresh_lines.clear();

  if(c.xprev != c.x || c.yprev != c.y){
    // Clear last character
    color col = c.fill;
    c.fill =
        (p.ymark == -1 || ((c.y < c.yprev || (c.y == c.yprev && c.x < c.xprev))
        && (p.ymark < c.yprev || (p.ymark == c.yprev && p.xmark <= c.xprev)))
        || ((c.y > c.yprev || (c.y == c.yprev && c.x > c.xprev))
        && (p.ymark > c.yprev || (p.ymark == c.yprev && p.xmark > c.xprev))))
        ? p.col : SELECT_COLOR;
    c.pos = point(p.pos.x + c.xprev * p.char_width,
        p.pos.y + (c.yprev - p.top_line) * p.line_height);
    c.draw(&frame, p.view);
    c.pos = point(p.pos.x + c.x * p.char_width,
        p.pos.y + (c.y - p.top_line) * p.line_height);
    c.fill = col;

    // Draw last character
    if(c.yprev < p.text.size() && c.xprev < p.text[c.yprev].size())
      draw_char(p.font[p.text[c.yprev][c.xprev]],
          point(p.pos.x + c.xprev * p.char_width,
          p.pos.y + (c.yprev - p.top_line) * p.line_height),
          p.text_color[c.yprev][c.xprev]);
    c.xprev = c.x, c.yprev = c.y; }

  // Clear cursor
  color col = c.fill;
  c.fill = (p.ymark == -1 || (c.y > p.ymark
      || (c.y == p.ymark && c.x >= p.xmark))) ? p.col : SELECT_COLOR;
  c.draw(&frame, p.view);
  c.fill = col;
  // Draw current character
  if(c.y < p.text.size() && c.x < p.text[c.y].size())
    draw_char(p.font[p.text[c.y][c.x]],
        point(p.pos.x + c.x * p.char_width,
        p.pos.y + (c.y - p.top_line) * p.line_height), p.text_color[c.y][c.x]);
  // Draw cursor
  c.draw(&frame, p.view);

  // Draw vertical divider
  for(Panel& t : panels){
    if(!t.refresh_divider) continue;
    polygon div;
    div.pos = point(t.pos.x + t.width, t.pos.y);
    div.set_box(VERTICAL_DIVIDE, t.height);
    div.fill = UNFOCUS_FILE_BAR_COLOR;
    div.draw(&frame, t.view);
    t.refresh_divider = false; }

  // Draw file bars
  for(Panel& t : panels){
    if(!t.refresh_file_bar) continue;
    polygon bar;
    bar.pos = point(t.pos.x, t.pos.y + t.height);
    bar.set_box(t.width, default_line_height);
    bar.fill = (focus == &t) ? FOCUS_FILE_BAR_COLOR : UNFOCUS_FILE_BAR_COLOR;
    bar.draw(&frame, t.view);
    str bar_text = str(t.saved ? "-----" : "*****") + "     ";
    if(t.file.find("\\root\\") != str::npos)
      bar_text += t.file.substr(t.file.find("\\root\\") + 6) + "     ";
    bar_text += "(" + to_string(t.cursor.y + 1) + ","
        + to_string(t.cursor.x + 1) + ")";
    for(int x = 0; x < bar_text.size(); ++x)
      draw_char(default_font[bar_text[x]],
          point(bar.pos.x + x * default_char_width, bar.pos.y), BLACK);
    t.refresh_file_bar = false; }

  // Hide cmd bar
  if(cmd.hide){
    assert(focus != &cmd, "draw", "cmd marked for refresh but still in focus");
    polygon line;
    line.pos = point(cmd.pos.x, cmd.pos.y);
    line.set_box(cmd.width, cmd.line_height);
    line.fill = BKGD_COLOR;
    line.draw(&frame, cmd.view);
    cmd.hide = false; } }

bool Editor::name_or_val(int y, int x) const {
  const Panel& p = *focus;
  if(y >= p.text.size() || x >= p.text[y].size())
    return false;
  const char c = p.text[y][x];
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
      || (c >= 'a' && c <= 'z') || c == '_'; }

void Editor::load_font(){
  image font_img = load_bmp(_FONT_LOC);
  for(int i = 0; i < _SYMBOLS.size(); ++i){
    image c(CHAR_WIDTH_SCALE_1, LINE_HEIGHT_SCALE_1);
    for(int xi = i * c.width, xo = 0; xo < c.width; ++xi, ++xo)
      for(int y = 0; y < c.height; ++y)
        c.set_pixel(xo, y, font_img.data[y][xi]);
    font_base[_SYMBOLS[i]] = c; }
  // Add space manually //! add to symbols
  image space(CHAR_WIDTH_SCALE_1, LINE_HEIGHT_SCALE_1);
  for(int y = 0; y < space.height; ++y)
    for(int x = 0; x < space.width; ++x)
      space.data[y][x] = CLEAR;
  font_base[' '] = space; }

void Editor::scale_font(double factor){
  Panel& p = *focus;
  p.text_scale *= factor;
  p.line_height = (int)ceil(p.text_scale * LINE_HEIGHT_SCALE_1);
  p.char_width = (int)ceil(p.text_scale * CHAR_WIDTH_SCALE_1);
  p.font.clear();
  for(pair<char, image> f : font_base)
    p.font[f.first] = f.second.scale(p.text_scale);
  p.cursor.scale(factor); }

void Editor::draw_char(const image& img, const point& p, const color& c){
  image r = img;
  r.pos = p;
  for(int y = 0; y < img.height; ++y)
    for(int x = 0; x < img.width; ++x){
      color ci = img.data[y][x];
      if(ci.r > 128 || ci.g > 128 || ci.b > 128)
        r.set_pixel(x, y, (c == BLACK) ? BLACK : ci.avg(c));
      else
        r.data[y][x] = CLEAR; }
  r.draw(&frame, focus->view); }

void Editor::insert_text(const vec<str>& text, const int y, const int x){
  Panel& p = *focus;
  str tail = p.text[y].substr(x);
  p.text[y] = p.text[y].substr(0, x) + text[0];
  p.text.insert(p.text.begin() + y + 1, text.begin() + 1, text.end());
  p.text[y + text.size() - 1] += tail;
  for(int yl = y; yl - p.top_line <= p.height / p.line_height
      && yl < p.text.size(); ++yl)
    p.refresh_lines.insert(yl - p.top_line);
  p.refresh_file_bar = true; }

void Editor::remove_text(
    const int y0, const int x0, const int yf, const int xf){
  Panel& p = *focus;
  if(y0 == p.text.size() - 1 && x0 == p.text[y0].size()) return;
  int xf2 = xf, yf2 = yf;
  if(xf == -1){
    --yf2;
    xf2 = (int)p.text[yf2].size(); }
  str line = p.text[y0].substr(0, x0);
  for(int y = y0; y <= p.top_line + p.height / p.line_height
      && y <= p.text.size(); ++y)
    p.refresh_lines.insert(y - p.top_line);
  if(xf2 == p.text[yf2].size()){
    if(yf2 + 1 < p.text.size() && p.text[yf2 + 1] != "")
      line += p.text[yf2 + 1];
    p.text.erase(p.text.begin() + y0 + 1, p.text.begin() + yf2 + 2);
  }else{
    line += p.text[yf2].substr(xf2 + 1);
    p.text.erase(p.text.begin() + y0 + 1, p.text.begin() + yf2 + 1); }
  p.text[y0] = line;
  p.refresh_file_bar = true; }

void Editor::delete_selection(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.ymark == -1) return;
  if(c.y < p.ymark || (c.y == p.ymark && c.x < p.xmark))
    remove_text(c.y, c.x, p.ymark, p.xmark - 1);
  else if(c.y > p.ymark || (c.y == p.ymark && c.x > p.xmark)){
    remove_text(p.ymark, p.xmark, c.y, c.x - 1);
    c.y = p.ymark, c.x = p.xmark; }
  p.ymark = p.xmark = -1; }

void Editor::refresh_panel(){
  Panel& p = *focus;
  for(int y = 0; y <= p.height / p.line_height; ++y)
    p.refresh_lines.insert(y);
  p.refresh_file_bar = cmd.hide = true; }

void Editor::scroll(const bool down){
  Panel& p = *focus;
  assert(&p != &cmd, "scroll", "cmd bar is in focus");
  int lines = min((int)p.text.size() - p.top_line - 1, SCROLL_LINES);
  if(!down)
    lines = min(lines, p.top_line);
  p.top_line += down ? lines : -lines;
  p.refresh_divider = true;
  refresh_panel(); }

void Editor::move_cursor(const Dir d){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.refresh_file_bar = true;
  if(p.ymark != -1)
    p.refresh_lines.insert(c.y - p.top_line);

  switch(d){
  case UP:
    if(c.y == 0){
      if(c.x > 0)
        c.x = 0;
      return; }
    --c.y;
    if(c.x > p.text[c.y].size())
      c.x = (int)p.text[c.y].size();
    if(c.y < p.top_line)
      scroll(false);
    if(p.ymark != -1)
      p.refresh_lines.insert(c.y - p.top_line);
    return;

  case LEFT:
    if(c.x == 0){
      if(c.y == 0) return;
      --c.y;
      c.x = (int)p.text[c.y].size();
      if(c.y < p.top_line)
        scroll(false);
      if(p.ymark != -1)
        p.refresh_lines.insert(c.y - p.top_line);
    }else
      --c.x;
    return;

  case DOWN:
    if(c.y == p.text.size() - 1){
      if(c.x < p.text[c.y].size())
        c.x = (int)p.text[c.y].size();
      return; }
    ++c.y;
    if(c.x > p.text[c.y].size())
      c.x = (int)p.text[c.y].size();
    if(c.y - p.top_line >= p.height / p.line_height - 1)
      scroll(true);
    if(p.ymark != -1)
      p.refresh_lines.insert(c.y - p.top_line);
    return;

  case RIGHT:
    if(c.x == p.text[c.y].size()){
      if(c.y == p.text.size() - 1) return;
      ++c.y;
      c.x = 0;
      if(c.y - p.top_line >= p.height / p.line_height - 1)
        scroll(true);
      if(p.ymark != -1)
        p.refresh_lines.insert(c.y - p.top_line);
    }else
      ++c.x;
    return;
  default:
    err("move_cursor", "bad direction"); } }

void Editor::clip(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.ymark == -1) return;
  int y0, yf, x0, xf;
  if(c.y < p.ymark || (c.y == p.ymark && c.x < p.xmark))
    y0 = c.y, yf = p.ymark, x0 = c.x, xf = p.xmark - 1;
  else if(c.y > p.ymark || (c.y == p.ymark && c.x > p.xmark))
    y0 = p.ymark, yf = c.y, x0 = p.xmark, xf = c.x - 1;
  else return;
  clipboard.clear();
  clipboard.pb("");
  while(y0 < yf || (y0 == yf && x0 <= xf)){
    if(x0 < p.text[y0].size()){
      clipboard.back() += str(1, p.text[y0][x0]);
      ++x0; }
    if(x0 == p.text[y0].size()){
      clipboard.pb("");
      ++y0;
      x0 = 0; } } }

void Editor::split_horizontal(){
  Panel& p = *focus;
  p.height = (p.height + default_line_height) / 2 - default_line_height;
  p.refresh_file_bar = true;
  p.refresh_divider = true;
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  if(i + 1 == panels.size())
    panels.pb(p);
  else
    panels.insert(panels.begin() + i + 1, p);
  Panel& p2 = panels[i];
  focus = &panels[i + 1];
  focus->pos = point(p2.pos.x, p2.pos.y + p2.height + default_line_height);
  refresh_panel(); }

void Editor::split_vertical(){
  Panel& p = *focus;
  p.width = (p.width + VERTICAL_DIVIDE) / 2 - VERTICAL_DIVIDE;
  p.refresh_file_bar = true;
  p.refresh_divider = true;
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  if(i + 1 == panels.size())
    panels.pb(p);
  else
    panels.insert(panels.begin() + i + 1, p);
  Panel& p2 = panels[i];
  focus = &panels[i + 1];
  focus->pos = point(p2.pos.x + p2.width + VERTICAL_DIVIDE, p2.pos.y);
  refresh_panel(); }

void Editor::close_panel(){
  Panel& p = *focus;
  //!

}

#endif
