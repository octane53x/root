// IMP SCENES

#ifndef SCENES_HH
#define SCENES_HH

#include "../../gl/shape.hh"
#include "../../gl/scene.hh"

struct start_button : button {
  void fn(){
    //!
  }
};

struct Title : scene {
  label title_lbl, start_lbl;
  start_button start_btn;
  vec<polygon> tris;

  Title(){
    bkgd_color = color(255,0,0);
    title_lbl.text = "IMPACT";
    title_lbl.text_color = color(0,0,0);
    title_lbl.size = 300;
    title_lbl.pos = point(100, 100); }

  void init(){
    title_lbl.font = fonts["aldo"]; }

  void draw_bkgd(){
    scene::draw_bkgd();
    //title_lbl.draw(&frame);
    //start_lbl.draw(&frame);
    //start_btn.draw(&frame);
    }

  image* next_frame(){
    //!
    draw_bkgd();
    polygon({point(100,100),
             point(150,150),
             point(200,100),
             point(200,200),
             point(100,200)}).draw(&frame);
    return &frame; } };

#endif
