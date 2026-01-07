// EDITOR PANEL

#ifndef PANEL_HH
#define PANEL_HH

#include "cursor.hh"
#include "highlighter.hh"

struct Panel : virtual system {

  // Inherits:
  // bool updated
  // clock_t last_update

  bool hide, saved, focus, refresh, split_ready;
  int line_height, char_width, scroll_lines, top_line;
  double text_scale;
  color bkgd;
  ipoint pos, size, mark;
  str file;
  vec<str> text;
  vec<vec<color> > text_color;
  image buf;
  viewport view;
  Cursor cursor;
  // Key 1: Scale, Key 2: Bkgd color, Key 3: Text color, Key 4: Character
  static umap<double, umap<color, umap<color, umap<char, image> > > > fonts;
  static Highlighter highlighter;

  Panel();

  virtual void init();
  virtual void update(const double ms);

  void draw(image* frame, const viewport& view);
  void draw_char(
      const image& img, const ipoint& buf_pos, const ipoint& frame_pos,
      const color& ctext, const color& cbkgd);

  void resize(const ipoint& _pos, const ipoint& _size);
  void scale_font(const double factor, const umap<char, image>& font_base);
  void insert_text(const vec<str>& ins, const ipoint& p);
  void remove_text(const ipoint& p0, const ipoint& pf);
  void delete_selection();
  void scroll(const bool down);
  void move_cursor(const Dir d); };

Highlighter Panel::highlighter;
umap<double, umap<color, umap<color, umap<char, image> > > > Panel::fonts;

Panel::Panel():
    width(0), height(0) {}

// Set size prior
void Panel::init(){
  system::init();
  hide = split_ready = false;
  saved = focus = refresh = true;
  line_height = LINE_HEIGHT_SCALE_1;
  char_width = CHAR_WIDTH_SCALE_1;
  scroll_lines = size.y / line_height / 2;
  top_line = 0;
  text_scale = 1.0;
  bkgd = BKGD_COLOR;
  pos = ipoint(0, 0);
  mark = ipoint(-1, -1);
  file = "";
  text.pb("");
  text_color.pb(vec<color>());
  buf.set_size(size.x, size.y);
  cursor.init(); }

void Panel::update(const double ms){
  system::update(ms);
  scroll_lines = height / line_height / 2;
  cursor.pos = point(
      pos.x + cursor.x * char_width,
      pos.y + (cursor.y - top_line) * line_height);
  cursor.width = char_width;
  cursor.height = line_height;
  cursor.bkgd = fill;
  updated = true;
  last_update = clock(); }

void Panel::draw(image* frame, const viewport& view){}

void Panel::draw_char(const image& img, const ipoint& buf_pos,
    const ipoint& frame_pos){
  image r = img;
  // Draw to buffer
  r.pos = point(buf_pos);
  r.draw(&buf, view);
  // Draw to frame
  if(frame_pos.y < frame.height && frame_pos.y + line_height >= 0){
    r.pos = point(frame_pos);
    r.draw(&frame, view); } }

void Panel::scale_font(const double factor, const umap<char, image>& font_base){
  text_scale *= factor;
  line_height = (int)ceil(text_scale * LINE_HEIGHT_SCALE_1);
  char_width = (int)ceil(text_scale * CHAR_WIDTH_SCALE_1);
  font.clear();
  for(pair<char, image> f : font_base)
    font[f.first] = f.second.scale(text_scale);
  cursor.scale(factor); }

void Panel::insert_text(const vec<str>& ins, const ipoint& p){
  str tail = text[p.y].substr(p.x);
  text[p.y] = text[p.y].substr(0, p.x) + text[0];
  text.insert(text.begin() + p.y + 1, ins.begin() + 1, ins.end());
  text[p.y + text.size() - 1] += tail;
  highlighter.highlight_text(text, &text_color);

  // Draw single character at end of line
  if(ins.size() == 1 && ins[0].size() == 1 && p.x == text[p.y].size() - 1){
    draw_char(font[ins[0][0]], p,
        ipoint(p.x * char_width + (int)round(pos.x),
        (p.y - top_line) * line_height + (int)round(pos.y)),
        text_color[p.y][p.x], fill);
    return; }

  // Draw single line without adding new lines
  if(ins.size() == 1){
    for(int x = p.x; x < text[p.y].size(); ++x)
      draw_char(font[text[p.y][x]], ipoint(x, p.y);
          ipoint(x * char_width + (int)round(pos.x),
          (p.y - top_line) * line_height + (int)round(pos.y)),
          text_color[p.y][x], fill);
    return; }

  // Split buffer with new lines
  for(int i = 0; i < (ins.size() - 1) * line_height; ++i){
    buf.data.pb(vec<color>());
    buf.data.back().reserve(buf.width); }
  for(int

}

void Panel::remove_text(const ipoint& p0, const ipoint& pf){
  if(p0.y == text.size() - 1 && p0.x == text[p0.y].size()) return;
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
  // draw
  }

void Panel::delete_selection(){
  Cursor& c = cursor;
  if(mark.y == -1) return;
  if(c.y < mark.y || (c.y == mark.y && c.x < mark.x))
    remove_text(c.y, c.x, mark.y, mark.x - 1);
  else if(c.y > mark.y || (c.y == mark.y && c.x > mark.x)){
    remove_text(mark.y, mark.x, c.y, c.x - 1);
    c.y = mark.y, c.x = mark.x; }
  mark.y = mark.x = -1; }

void Panel::scroll(const bool down){
  int lines = min((int)text.size() - top_line - 1, scroll_lines);
  if(!down)
    lines = min(lines, top_line);
  top_line += down ? lines : -lines;
  // draw
  }

void Panel::move_cursor(const Dir d){
  Cursor& c = cursor;
  c.blink = true;
  if(mark.y != -1)
    refresh_lines.insert(c.y - top_line);

  switch(d){
  case UP:
    if(c.y == 0){
      if(c.x > 0)
        c.x = 0;
      return; }
    --c.y;
    if(c.x > text[c.y].size())
      c.x = (int)text[c.y].size();
    if(c.y < top_line)
      scroll(false);
    if(mark.y != -1)
      refresh_lines.insert(c.y - top_line);
    return;

  case LEFT:
    if(c.x == 0){
      if(c.y == 0) return;
      --c.y;
      c.x = (int)text[c.y].size();
      if(c.y < top_line)
        scroll(false);
      if(mark.y != -1)
        refresh_lines.insert(c.y - top_line);
    }else
      --c.x;
    return;

  case DOWN:
    if(c.y == text.size() - 1){
      if(c.x < text[c.y].size())
        c.x = (int)text[c.y].size();
      return; }
    ++c.y;
    if(c.x > text[c.y].size())
      c.x = (int)text[c.y].size();
    if(c.y - top_line >= height / line_height - 1)
      scroll(true);
    if(mark.y != -1)
      refresh_lines.insert(c.y - top_line);
    return;

  case RIGHT:
    if(c.x == text[c.y].size()){
      if(c.y == text.size() - 1) return;
      ++c.y;
      c.x = 0;
      if(c.y - top_line >= height / line_height - 1)
        scroll(true);
      if(mark.y != -1)
        refresh_lines.insert(c.y - top_line);
    }else
      ++c.x;
    return;
  default:
    err("move_cursor", "bad direction"); } }

#endif
