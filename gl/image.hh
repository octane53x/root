// IMAGE

#ifndef IMAGE_HH
#define IMAGE_HH

#include "gl_const.hh"

struct image : thing {
  point pos, size;
  vec<vec<color> > data;
  image(){ type = "image"; }
  image(point _size): image() {
    size = _size;
    for(int i = 0; i < size.y; ++i){
      data.pb(vec<color>());
      for(int j = 0; j < size.x; ++j)
        data[i].pb(WHITE); } }
  void fix();
  void scale(double s);
  void rotate(double r);
  void flip(line axis); };

void image::fix(){
  int left,right,top,bot;
  for(int i = 0; i < size.x; ++i){
    bool found = false;
    for(int j = 0; j < size.y; ++j)
      if(data[j][i] != WHITE){ found = true; break; }
    if(found){ left = i; break; } }
  for(int i = size.x-1; i >= 0; --i){
    bool found = false;
    for(int j = 0; j < size.y; ++j)
      if(data[j][i] != WHITE){ found = true; break; }
    if(found){ right = i; break; } }
  for(int j = 0; j < size.y; ++j){
    bool found = false;
    for(int i = 0; i < size.x; ++i)
      if(data[j][i] != WHITE){ found = true; break; }
    if(found){ top = j; break; } }
  for(int j = size.y-1; j >= 0; --j){
    bool found = false;
    for(int i = 0; i < size.x; ++i)
      if(data[j][i] != WHITE){ found = true; break; }
    if(found){ bot = j; break; } }
  image r(point(right-left+1, bot-top+1));
  for(int i = top; i <= bot; ++i)
    for(int j = left; j <= right; ++j)
      r.data[i-top][j-left] = data[i][j];
  *this = r; }

void image::scale(double s){
  image r(point((int)ceil(s*size.x), (int)ceil(s*size.y)));
  //!
  *this = r; }

void image::rotate(double r){
  //!
}

void image::flip(line axis){
  //!
}

#endif
