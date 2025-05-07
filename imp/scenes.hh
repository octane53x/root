// IMP SCENES

#include "imp_incl.hh"

const int WIN_W = 1024,
          WIN_H = 768;

struct TitleScreen : Scene {
  Label title;
  TitleScreen(){
    bkgd_color = color(255,0,0);
    title.text = "IMPACT";
    title.text_color = color(0,0,0);
    title.size = 300;
    title.font = "aldo";
    title.pos = point(100, 100);
    bkgd = Scene::draw_bkgd();
    bkgd = draw_bkgd(bkgd);
  }
  Frame next_frame(){
    return bkgd;
  }
  Frame draw_bkgd(Frame f){
    title.draw(f);
  }
};
