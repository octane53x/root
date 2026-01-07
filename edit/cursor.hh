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

  Cursor();

  virtual void init();
  virtual void update(const double ms);

  void draw(image* frame, const viewport& view); };

Cursor::Cursor():
    fill(CURSOR_COLOR), blink(true), focus(true),
    x(0), y(0), xprev(0), yprev(0) {}

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

void Cursor::draw(image* frame, const viewport& view){
  if(focus)
    polygon::draw(frame, view);
  else{
    line(pos, point(pos.x + width, pos.y)).draw(frame, view);
    line(point(pos.x + width, pos.y),
        point(pos.x + width, pos.y + height)).draw(frame, view);
    line(point(pos.x + width, pos.y + height),
        point(pos.x, pos.y + height)).draw(frame, view);
    line(point(pos.x, pos.y + height), pos).draw(frame, view); } }

#endif
