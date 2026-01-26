// EDITOR CURSOR

#ifndef CURSOR_HH
#define CURSOR_HH

#include "util.hh"

struct Cursor {

  bool updated, blink;
  clock_t last_update;
  color fill, bkgd;
  ipoint pos, size;

  static image* frame;

  void init();
  void update(const double ms);

  // Defined in draw.hh
  void draw(const ipoint& win_pos, const bool blt); };

image* Cursor::frame;

void Cursor::init(){
  updated = blink = true;
  last_update = 0;
  fill = CURSOR_COLOR;
  bkgd = BKGD_COLOR;
  pos = ipoint(0, 0);
  size = ipoint(CHAR_WIDTH_SCALE_1, LINE_HEIGHT_SCALE_1); }

void Cursor::update(const double ms){
  double sec = (double)(clock() - last_update) / CPS;
  if(sec >= CURSOR_BLINK){
    blink = !blink;
    fill = blink ? CURSOR_COLOR : bkgd;
    updated = true;
    last_update = clock(); } }

#endif
