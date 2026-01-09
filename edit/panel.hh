// EDITOR PANEL

#ifndef PANEL_HH
#define PANEL_HH

#include "cursor.hh"

struct Panel : virtual system {

  // Inherits:
  // bool updated
  // clock_t last_update

  bool cmd, saved, focus, split_ready;
  int line_height, char_width, scroll_lines, top_line;
  double text_scale;
  color bkgd;
  ipoint pos, size, mark;
  str file;
  vec<str> text;
  vec<vec<color> > text_color;
  Cursor cursor;
  uset<Panel*> siblings;

  static image* frame;
  // Key 1: Scale, Key 2: Bkgd color, Key 3: Text color, Key 4: Character
  static umap<double, umap<color, umap<color, font> > > fonts;

  Panel();

  virtual void init();
  virtual void update(const double ms);

  // Defined in draw.hh
  void draw();
  void draw_char(const image& img, const ipoint& p);
  void draw_selection(const ipoint& p0, const ipoint& pf);
  void draw_divider();
  void draw_file_bar();

  void resize(const ipoint& _pos, const ipoint& _size);
  void insert_text(const vec<str>& ins, const ipoint& p);
  void remove_text(const ipoint& p0, const ipoint& pf);
  void delete_selection();
  void scroll(const Dir d);
  void move_cursor(const Dir d);
  // Defined in highlight.hh
  void highlight_text(); };

image* Panel::frame;
umap<double, umap<color, umap<color, umap<char, image> > > > Panel::fonts;

Panel::Panel():
    size(0, 0), pos(0, 0) {}

// Set size and pos prior
void Panel::init(){
  system::init();
  cmd = split_ready = false;
  saved = focus = true;
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
  cursor.init(); }

void Panel::update(const double ms){
  system::update(ms);
  Cursor& c = cursor;
  ipoint cpos = ipoint(c.pos.x * char_width + pos.x,
      (c.pos.y - top_line) * line_height + pos.y);
  if(focus){
    c.update(ms);
    if(c.updated){
      c.updated = false;
      char ch = (c.pos.x == text[c.pos.y].size())
          ? ' ' : text[c.pos.y][c.pos.x];
      color tc = (c.fill == CURSOR_COLOR) ? BAR_TEXT_COLOR
          : ((ch == ' ') ? COLOR_CODE : text_color[c.pos.y][c.pos.x]);
      draw_char(fonts[text_scale][c.fill][tc][ch], cpos);
      updated = true;
      last_update = clock(); }
  }else if(!cmd)
    c.draw(cpos); }

void Panel::insert_text(const vec<str>& ins, const ipoint& p){
  // Modify text
  str tail = text[p.y].substr(p.x);
  text[p.y] = text[p.y].substr(0, p.x) + ins[0];
  text.insert(text.begin() + p.y + 1, ins.begin() + 1, ins.end());
  text[p.y + ins.size() - 1] += tail;
  highlight_text();

  // Draw single character at end of line
  if(ins.size() == 1 && ins[0].size() == 1 && p.x == text[p.y].size() - 1){
    draw_char(fonts[text_scale][bkgd][text_color[p.y][p.x]][ins[0][0]],
        ipoint(p.x * char_width + pos.x,
        (p.y - top_line) * line_height + pos.y));
    return; }

  // Draw single line without adding new lines
  if(ins.size() == 1){
    for(int x = p.x; x < text[p.y].size(); ++x)
      draw_char(fonts[text_scale][bkgd][text_color[p.y][x]][text[p.y][x]],
          ipoint(x * char_width + pos.x,
          (p.y - top_line) * line_height + pos.y));
    return; }

  // Redraw panel if lines moved
  draw(); }

void Panel::remove_text(const ipoint& p0, const ipoint& pf){
  if(p0.y == text.size() - 1 && p0.x == text[p0.y].size()) return;
  // Draw over end of line
  if(p0.y == pf.y)
    for(int x = (int)text[p0.y].size() - (pf.x - p0.x);
        x < text[p0.y].size(); ++x)
      draw_char(fonts[text_scale][bkgd][COLOR_CODE][' '],
          ipoint(x * char_width + pos.x,
          (p0.y - top_line) * line_height + pos.y));

  // Remove from text
  ipoint pf2 = pf;
  if(pf.x == -1){
    --pf2.y;
    pf2.x = (int)text[pf2.y].size(); }
  str line = text[p0.y].substr(0, p0.x);
  if(pf2.x == text[pf2.y].size()){
    if(pf2.y + 1 < text.size() && text[pf2.y + 1] != "")
      line += text[pf2.y + 1];
    text.erase(text.begin() + p0.y + 1, text.begin() + pf2.y + 2);
  }else{
    line += text[pf2.y].substr(pf2.x + 1);
    text.erase(text.begin() + p0.y + 1, text.begin() + pf2.y + 1); }
  text[p0.y] = line;
  highlight_text();

  // Draw chars after deletion
  if(p0.y == pf.y){
    for(int x = p0.x; x < text[p0.y].size(); ++x)
      draw_char(fonts[text_scale][bkgd][text_color[p0.y][x]][text[p0.y][x]],
          ipoint(x * char_width + pos.x,
          (p0.y - top_line) * line_height + pos.y));
    return;
  }

  // Redraw panel if lines moved
  draw(); }

void Panel::delete_selection(){
  if(mark.y == -1) return;
  Cursor& c = cursor;
  if(c.pos.y < mark.y || (c.pos.y == mark.y && c.pos.x < mark.x))
    remove_text(c.pos, ipoint(mark.x - 1, mark.y));
  else if(c.pos.y > mark.y || (c.pos.y == mark.y && c.pos.x > mark.x)){
    remove_text(mark, ipoint(c.pos.x - 1, c.pos.y));
    c.pos.y = mark.y, c.pos.x = mark.x; }
  mark.y = mark.x = -1; }

void Panel::scroll(const Dir d){
  int lines = min((int)text.size() - top_line - 1, scroll_lines);
  if(d == UP)
    lines = min(lines, top_line);
  top_line += (d == DOWN) ? lines : -lines;
  draw(); }

void Panel::move_cursor(const Dir d){
  Cursor& c = cursor;
  c.blink = true;

  // Draw char at previous position
  color cb = (mark.y != -1 && (c.pos.y < mark.y
      || (c.pos.y == mark.y && c.pos.x < mark.x))) ? SELECT_COLOR : bkgd;
  char ch = (c.pos.x == text[c.pos.y].size()) ? ' ' : text[c.pos.y][c.pos.x];
  color ct = (ch == ' ') ? COLOR_CODE : text_color[c.pos.y][c.pos.x];
  draw_char(fonts[text_scale][cb][ct][ch], ipoint(c.pos.x * char_width + pos.x,
      (c.pos.y - top_line) * line_height + pos.y));

  switch(d){
  case UP:
    if(c.pos.y == 0){
      if(c.pos.x > 0)
        c.pos.x = 0;
      return; }
    --c.pos.y;
    if(c.pos.x > text[c.pos.y].size())
      c.pos.x = (int)text[c.pos.y].size();
    if(c.pos.y < top_line)
      scroll(UP);
    return;

  case LEFT:
    if(c.pos.x == 0){
      if(c.pos.y == 0) return;
      --c.pos.y;
      c.pos.x = (int)text[c.pos.y].size();
      if(c.pos.y < top_line)
        scroll(UP);
    }else
      --c.pos.x;
    return;

  case DOWN:
    if(c.pos.y == text.size() - 1){
      if(c.pos.x < text[c.pos.y].size())
        c.pos.x = (int)text[c.pos.y].size();
      return; }
    ++c.pos.y;
    if(c.pos.x > text[c.pos.y].size())
      c.pos.x = (int)text[c.pos.y].size();
    if(c.pos.y - top_line >= size.y / line_height - 1)
      scroll(DOWN);
    return;

  case RIGHT:
    if(c.pos.x == text[c.pos.y].size()){
      if(c.pos.y == text.size() - 1) return;
      ++c.pos.y;
      c.pos.x = 0;
      if(c.pos.y - top_line >= size.y / line_height - 1)
        scroll(DOWN);
    }else
      ++c.pos.x;
    return;
  default:
    err("move_cursor", "impossible direction"); } }

#endif
