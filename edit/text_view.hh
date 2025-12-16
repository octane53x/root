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

  int line_pos, char_pos;
  vec<str> text;
  umap<char, image> font;

  TextView();

  virtual void init();
  virtual void update(const double ms);
  virtual void draw(image* canvas, const viewport& view); };

TextView::TextView(): line_pos(0), char_pos(0) {
  type = "TextView";
  text.pb(""); }

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
  scene::update(ms);
  cursor.pos = point(char_pos * CHAR_WIDTH, line_pos * LINE_HEIGHT); }

void TextView::draw(image* canvas, const viewport& view){
  scene::draw(canvas, view);
  for(int y = 0; y < text.size(); ++y)
    for(int x = 0; x < text[y].size(); ++x){
      image c = font[text[y][x]];
      c.pos = point(x * CHAR_WIDTH, y * LINE_HEIGHT);
      c.draw(canvas, view); } }

TextView::Cursor::Cursor(): blink(true) {}

void TextView::Cursor::update(const double ms){
  double sec = (double)(clock() - last_update) / CLOCKS_PER_SEC;
  if(sec >= CURSOR_BLINK){
    blink = !blink;
    fill = blink ? CYAN : CLEAR;
    last_update = clock(); } }

#endif
