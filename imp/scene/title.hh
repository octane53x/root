// IMP SCENES

#ifndef SCENES_HH
#define SCENES_HH

#include "../../gl/scene.hh"

struct Title : scene {
  label title;
  Title(){
    bkgd_color = color(255,0,0);
    title.text = "IMPACT";
    title.text_color = color(0,0,0);
    title.size = 300;
    title.pos = point(100, 100); }
  void init(){
    title.font = fonts["aldo"]; }
  image next_frame(){
    //!
    return bkgd_frame; }
  void draw_bkgd(){
    scene::draw_bkgd();
    title.draw(&bkgd_frame);
    //!
    } };

#endif
