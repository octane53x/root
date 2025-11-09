// IMP SCENES

#ifndef SCENES_HH
#define SCENES_HH

#include "../../gl/polygon.hh"
#include "../../gl/label.hh"
#include "../../gl/button.hh"
#include "../../gl/scene.hh"

struct Play : button {

  Play(){}

  void fn(){
    //!
  }

  void draw(image* bkgd){
    //!
  } };

struct Title : scene {

  label title_lbl, start_lbl;
  button* play;

  Title(int w, int h): scene(w, h) {}

  void init(){
    bkgd_color = color(255,0,0);
    title_lbl.text = "IMPACT";
    title_lbl.text_color = color(0,0,0);
    title_lbl.size = 40;
    title_lbl.pos = point(100, 100);
    title_lbl.font = fonts["aldo"];
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
    objs.pb(c); }

  void draw_bkgd(){
    scene::draw_bkgd();
    title_lbl.draw(&bkgd); }

  void update(double ms){} };

#endif
