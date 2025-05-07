// FONT INPUT

#ifndef FONT_INPUT_HH
#define FONT_INPUT_HH

#include "../../lang/core/obj.hh"

vec<vec<color> > bmp_parse(FILE* f){
  vec<vec<color> > v;
  //!
  return v;
}

void font_input(str font){
  umap<char, vec<vec<color> > > m;
  for(char c = '0'; c <= 'Z'; ++c){
    FILE* f = fopen(font + "/" + c + ".bmp");
    vec<vec<color> > image = bmp_parse(f);
    m[c] = image;
  }
}

#endif
