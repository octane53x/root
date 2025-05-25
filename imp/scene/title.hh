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
  clock_t last_tri;
  label title_lbl, start_lbl;
  start_button start_btn;
  vec<polygon> tris;

  Title(){
    last_tri = 0;
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
    draw_bkgd();
    clock_t now = clock();
    move_objs((now - last_frame) / (CLOCKS_PER_SEC / 1000));
    last_frame = now;
    draw_objs();

    if((double)(now - last_tri) / CLOCKS_PER_SEC >= 1.0){
      int h = rand() % 150 + 50;
      int y = rand() % (win_size.y - h);
      int x = win_size.x - 1;
      polygon p({point(x,y), point(x,y+h), point(x-(h>>1),y+(h>>1))});
      tris.pb(p);
      last_tri = now; }

    for(int i = 0; i < tris.size(); ++i){
      int t = 0;
      for(int j = 0; j < tris[i].points.size(); ++j){
        tris[i].points[j].x -= 5;
        if(tris[i].points[j].x < 0) ++t; }
      if(t == tris[i].points.size()){
        tris.erase(tris.begin() + i);
        --i; }
      else tris[i].draw(&frame); }

    return &frame; } };

#endif
