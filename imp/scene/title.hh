// TITLE SCENE

#ifndef TITLE_HH
#define TITLE_HH

#include "../../gl/polygon.hh"
#include "../../gl/label.hh"
#include "../../gl/button.hh"
#include "../../gl/scene.hh"

struct Title : scene {

  bool logged_in;
  clock_t last_update_1;
  label title_lbl, start_lbl;
  button* play_btn;
  polygon* rand_color_box;

  Title(): last_update_1(0) {}

  virtual void validate(){
    scene::validate(); }

  void init(int w, int h){
    bkgd_color = RED;
    scene::init(w, h);

    title_lbl.text = "IMPACT";
    title_lbl.text_color = BLACK;
    title_lbl.size = 40;
    title_lbl.pos = point(100, 100);
    title_lbl.font = &fonts["aldo"];

    play_btn->fill = BLACK;
    play_btn->points.pb(point(400, 300));
    play_btn->points.pb(point(400, 400));
    play_btn->points.pb(point(350, 350));

    draw_bkgd();

    polygon* a = new polygon();
    a->points = {point(0,0), point(10,0), point(10,10), point(0,10)};
    a->fill = BLUE;
    a->pos = point(250,150);
    a->mov = new object::movement(object::movement::PATH);
    a->mov->path = {point(100,100), point(-100,100), point(-100,-100),
        point(100,-100)};
    a->mov->vel = 50.0;
    objs.pb(a);

    polygon* b = new polygon();
    b->points = a->points;
    b->fill = YELLOW;
    b->pos = point(250,100);
    b->mov = new object::movement(object::movement::ORBIT);
    b->mov->root = a;
    b->mov->vel = 100.0;
    objs.pb(b);

    polygon* c = new polygon();
    c->points = a->points;
    c->fill = GREEN;
    c->pos = point(250,125);
    c->mov = new object::movement(object::movement::ORBIT);
    c->mov->root = b;
    c->mov->vel = 200.0;
    rand_color_box = c;
    objs.pb(c); }

  void draw_bkgd(){
    title_lbl.draw(&bkgd, view);
    play_btn->draw(&bkgd, view); }

  void update(double ms){
    clock_t now = clock();
    if((double)(now - last_update_1) / CLOCKS_PER_SEC >= 0.1){
      rand_color_box->fill = color().random();
      last_update_1 = now;
    } } };

#endif
