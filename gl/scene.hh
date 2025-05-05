// SCENE

#ifndef SCENE_HH
#define SCENE_HH

#include "../lang/core/obj.hh"
#include "ui.hh"

struct Frame {
  vec<vec<color> > data;
  Frame(){}
};

struct Scene {
  vec<Label> labels;
  vec<Button> buttons;
  Frame next_frame();
  Scene(){}
};

#endif
