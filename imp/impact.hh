// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../gl/fonts/font.hh"
#include "../gl/env.hh"
#include "game.hh"
#include "scene/title.hh"

struct Impact : env {
  Game* g;
  Impact(point _win_size): env(_win_size) {}
  void init(){
    fonts["aldo"] = input_font("aldo");
    Title* title = new Title();
    active_scene = title;
    env::init();
    title->init();
    //title->draw_bkgd();

    image* f = &title->bkgd_frame;
    f->size = win_size;
    for(int i = 0; i < f->size.y; ++i){
      f->data.pb(vec<color>());
      for(int j = 0; j < f->size.x; ++j)
        f->data[i].pb(BLUE);
    }

    for(int ii = 0; ii < 500; ii += 20)
      for(int jj = 0; jj < 800; jj += 20)
        for(int i = ii; i < ii+10; ++i)
          for(int j = jj; j < jj+10; ++j)
            f->data[i][j] = YELLOW;

    //!
    } };

#endif
