// IMP SCENES

#include "../gl/scene.hh"

struct TitleScreen : Scene {
  Label title;
  TitleScreen(){
    title.text = "IMPACT";
    title.text_color = color(0,0,0);
    title.size = 300;
    title.font = "aldo";
    title.pos = point(100, 100);
  }
  Frame init_frame(){

  }
  Frame next_frame(){
    return init_frame();
  }
};
