// EDITOR CURSOR

#ifndef CURSOR_HH
#define CURSOR_HH

#include "util.hh"

struct Cursor : virtual system {

  // Inherits:
  // bool updated
  // clock_t last_update

  bool blink;
  color fill, bkgd;
  ipoint pos, size;
  static image* frame;

  virtual void init();
  virtual void update(const double ms);

  // Defined in draw.hh
  void draw(const ipoint& win_pos); };

image* Cursor::frame;

void Cursor::init(){
  system::init();
  blink = true;
  fill = CURSOR_COLOR;
  bkgd = BKGD_COLOR;
  pos = ipoint(0, 0);
  size = ipoint(CHAR_WIDTH_SCALE_1, LINE_HEIGHT_SCALE_1); }

void Cursor::update(const double ms){
  system::update(ms);
  double sec = (double)(clock() - last_update) / CLOCKS_PER_SEC;
  if(sec >= CURSOR_BLINK){
    blink = !blink;
    fill = blink ? CURSOR_COLOR : CLEAR;
    updated = true;
    last_update = clock(); } }

#endif
