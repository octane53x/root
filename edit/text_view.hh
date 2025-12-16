// TEXT VIEW SCENE

#ifndef TEXT_VIEW_HH
#define TEXT_VIEW_HH

#include "../gl/scene.hh"

int LINE_HEIGHT = 20,
    CHAR_WIDTH = 10;
double
    CURSOR_BLINK = 0.5;

struct TextView : virtual scene {

  struct Cursor : virtual polygon {
    bool blink;
    Cursor();
    virtual void update(const double ms);
  } cursor;

  TextView();

  virtual void init();
  virtual void update(const double ms); };

TextView::TextView(){
  type = "TextView"; }

void TextView::init(){
  width = win_w, height = win_h;
  bkgd_color = BLACK;

  cursor.pos = point(0, 0);
  cursor.add(point(0, 0));
  cursor.add(point(CHAR_WIDTH, 0));
  cursor.add(point(CHAR_WIDTH, LINE_HEIGHT));
  cursor.add(point(0, LINE_HEIGHT));
  cursor.fill = CYAN;
  objs[cursor.id] = &cursor;

  scene::init(); }

void TextView::update(const double ms){
  scene::update(ms); }

TextView::Cursor::Cursor(): blink(true) {}

void TextView::Cursor::update(const double ms){
  debug("Cursor.update");
  double sec = (double)(clock() - last_update) / CLOCKS_PER_SEC;
  if(sec >= CURSOR_BLINK){
    blink = !blink;
    fill = blink ? CYAN : CLEAR;
    last_update = clock(); } }

#endif
