// IMP SCENES

#ifndef SCENES_HH
#define SCENES_HH

#include "../../gl/shape.hh"
#include "../../gl/move.hh"
#include "../../gl/scene.hh"
#include "../../gl/ui.hh"

struct start_button : button {
  void fn(){
    //!
  }
};

struct Title : scene {
  label title_lbl, start_lbl;
  start_button start_btn;

  Title(){
    bkgd_color = color(255,0,0);
    title_lbl.text = "IMPACT";
    title_lbl.text_color = color(0,0,0);
    title_lbl.size = 300;
    title_lbl.pos = point(100, 100); }

  void init(){
    //title_lbl.font = fonts["aldo"];
    object* a = new object();
    polygon p;
    p.points = {point(0,0), point(20,0), point(20,20), point(0,20)};
    p.fill_color = BLUE;
    a->img.set_size(21, 21);
    p.draw(&a->img);
    a->img_root = point(10,10);
    a->pos = point(200,200);
    a->mov.type = mov_type::PATH;
    a->mov.path = {point(100,0), point(0,100), point(-100,0), point(0,-100)};
    a->mov.vel = 50.0;
    objs.pb(a);

    object* b = new object();
    p.fill_color = YELLOW;
    b->img.set_size(21, 21);
    p.draw(&b->img);
    b->img_root = point(10,10);
    b->pos = point(200,150);
    b->mov.type = mov_type::ORBIT;
    b->mov.root = a;
    b->mov.vel = 200.0;
    objs.pb(b); }

  void draw_bkgd(){
    scene::draw_bkgd();
    //title_lbl.draw(&frame);
    //start_lbl.draw(&frame);
    //start_btn.draw(&frame);
    }

  image* next_frame(){
    draw_bkgd();
    clock_t now = clock();
    move_objs((double)(now - last_frame) * 1000.0 / CLOCKS_PER_SEC);
    last_frame = now;
    draw_objs();
    return &frame; } };

#endif
