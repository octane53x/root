// IMP SCENES

#ifndef SCENES_HH
#define SCENES_HH

#include "../../gl/scene.hh"

struct Title : scene {
  label title_lbl;
  Title(){
    bkgd_color = color(255,0,0);
    title_lbl.text = "IMPACT";
    title_lbl.text_color = color(0,0,0);
    title_lbl.size = 300;
    title_lbl.pos = point(100, 100); }
  void init(){
    title_lbl.font = fonts["aldo"]; }
  image next_frame(){
    //!
    return bkgd_frame; }
  void draw_bkgd(){
    scene::draw_bkgd();
    title_lbl.draw(&bkgd_frame);
    //!
    } };

#endif
