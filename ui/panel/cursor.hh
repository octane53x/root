// INPUT PANEL CURSOR

#ifndef CURSOR_HH
#define CURSOR_HH

#include "../../gl/polygon.hh"

const double CURSOR_BLINK = 0.5;

enum Dir : uchar { UP, DOWN, LEFT, RIGHT };

struct Cursor : polygon {

  bool updated, blink;
  clock_t last_update;
  color fill, bkgd;
  ipoint pos, size;

  virtual void init();
  virtual void update();
  // Defined in draw.hh
  virtual void draw(image* frame);

  void move(const Dir dir, const int n); };

image* Cursor::frame;

void Cursor::init(){
  object::init();
  updated = blink = true;
  last_update = 0;
  fill = CURSOR_COLOR;
  bkgd = BKGD_COLOR;
  pos = ipoint(0, 0);
  size = ipoint(CHAR_WIDTH_SCALE_1, LINE_HEIGHT_SCALE_1); }

void Cursor::update(){
  object::update();
  double sec = (double)(clock() - last_update) / CPS;
  if(sec >= CURSOR_BLINK){
    blink = !blink;
    fill = blink ? CURSOR_COLOR : bkgd;
    updated = true;
    last_update = clock(); } }

void Cursor::move(const Dir dir, const int n){
  switch(dir){
    case UP: pos.y -= size.y * n; break;
    case DOWN: pos.y += size.y * n; break;
    case LEFT: pos.x -= size.x * n; break;
    case RIGHT: pos.x += size.x * n; break;
    default: err("Cursor.move", "bad direction"); } }

#endif
