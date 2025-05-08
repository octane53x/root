// IMP SCENES

#ifndef SCENES_HH
#define SCENES_HH

#include "imp_incl.hh"

struct TitleScreen : Scene {
  Label title;
  TitleScreen(){
    bkgd_color = color(255,0,0);
    title.text = "IMPACT";
    title.text_color = color(0,0,0);
    title.size = 300;
    title.font = "aldo";
    title.pos = point(100, 100); }
  Frame next_frame(){
    return bkgd; }
  void draw_bkgd(){
    Scene::draw_bkgd();
    title.draw(&bkgd); } };

#endif
