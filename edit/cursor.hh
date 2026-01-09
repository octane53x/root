// EDITOR CURSOR

#ifndef CURSOR_HH
#define CURSOR_HH

#include "../gl/polygon.hh"

struct Cursor : virtual system {

  // Inherits:
  // bool updated
  // clock_t last_update

  bool blink, focus;
  color fill, bkgd;
  ipoint pos, ppos, size;

  virtual void init();
  virtual void update(const double ms);

  // Defined in draw.hh
  void draw(image* frame, const viewport& view); };

void Cursor::init(){
  system::init();
  blink = focus = true;
  fill = CURSOR_COLOR;
  bkgd = BKGD_COLOR;
  pos = ppos = ipoint(0, 0);
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
