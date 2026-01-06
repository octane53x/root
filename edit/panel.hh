// EDITOR PANEL

#ifndef PANEL_HH
#define PANEL_HH

#include "cursor.hh"

struct Panel : virtual polygon {

  // Inherits:
  // clock_t last_update
  // color fill
  // point pos
  // vec<point> points

  bool hide, saved, refresh, split_ready;
  int width, height, top_line, line_height, char_width, ymark, xmark;
  double text_scale;
  str file;
  vec<str> text;
  vec<vec<color> > text_color;
  umap<char, image> font;
  viewport view;
  Cursor cursor;

  Panel();

  virtual void update(const double ms);
  virtual void draw(image* frame, const viewport& view); };

Panel::Panel():
    text_scale(1.0), pos(0, 0), top_line(0), file(""), fill(BKGD_COLOR),
    text(""), ymark(-1), xmark(-1), saved(true), refresh(true),
    split_ready(false), hide(false) {}

void Panel::update(const double ms){
  system::update(ms);
  cursor.pos = point(
      pos.x + cursor.x * char_width,
      pos.y + (cursor.y - top_line) * line_height);
  cursor.width = char_width;
  cursor.height = line_height;
  cursor.bkgd = fill;
  updated = true;
  last_update = clock(); }

void Panel::draw(image* frame, const viewport& view){}

#endif
