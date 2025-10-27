// IMP SCENES

#ifndef SCENES_HH
#define SCENES_HH

#include "../../gl/obj/polygon.hh"
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

  Title(){}

  void init(){
    bkgd_color = color(255,0,0);
    title_lbl.text = "IMPACT0123456789";
    title_lbl.text_color = color(0,0,0);
    title_lbl.size = 10;
    title_lbl.pos = point(100, 100);
    title_lbl.font = fonts["aldo"];
    draw_bkgd();

    object* a = new object();
    polygon p;
    p.points = {point(0,0), point(10,0), point(10,10), point(0,10)};
    p.fill = BLUE;
    a->img.set_size(11, 11);
    p.draw(&a->img);
    a->img_root = point(5,5);
    a->pos = point(250,150);
    a->mov.type = mov_type::PATH;
    a->mov.path = {point(100,100), point(-100,100), point(-100,-100),
        point(100,-100)};
    a->mov.vel = 50.0;
    objs.pb(a);

    object* b = new object();
    p.fill = YELLOW;
    b->img.set_size(11, 11);
    p.draw(&b->img);
    b->img_root = point(5,5);
    b->pos = point(250,100);
    b->mov.type = mov_type::ORBIT;
    b->mov.root = a;
    b->mov.vel = 100.0;
    objs.pb(b);

    object* c = new object();
    p.fill = GREEN;
    c->img.set_size(11, 11);
    p.draw(&c->img);
    c->img_root = point(5,5);
    c->pos = point(250,125);
    c->mov.type = mov_type::ORBIT;
    c->mov.root = b;
    c->mov.vel = 200.0;
    objs.pb(c); }

  void draw_bkgd(){
    scene::draw_bkgd();
    title_lbl.draw(&bkgd);
    //start_lbl.draw(&frame);
    //start_btn.draw(&frame);
    }

  image* next_frame(){
    frame = bkgd;
    clock_t now = clock();
    move_objs((double)(now - last_frame) * 1000.0 / CLOCKS_PER_SEC);
    last_frame = now;
    draw_objs();
    return &frame; } };

#endif
