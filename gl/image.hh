// IMAGE

#ifndef IMAGE_HH
#define IMAGE_HH

#include "obj.hh"

void image::fix(){
  int left,right,top,bot;
  for(int i = 0; i < width; ++i){
    bool found = false;
    for(int j = 0; j < height; ++j)
      if(data[j][i] != WHITE){ found = true; break; }
    if(found){ left = i; break; } }
  for(int i = width-1; i >= 0; --i){
    bool found = false;
    for(int j = 0; j < height; ++j)
      if(data[j][i] != WHITE){ found = true; break; }
    if(found){ right = i; break; } }
  for(int j = 0; j < height; ++j){
    bool found = false;
    for(int i = 0; i < width; ++i)
      if(data[j][i] != WHITE){ found = true; break; }
    if(found){ top = j; break; } }
  for(int j = height-1; j >= 0; --j){
    bool found = false;
    for(int i = 0; i < width; ++i)
      if(data[j][i] != WHITE){ found = true; break; }
    if(found){ bot = j; break; } }
  image r(right-left+1, bot-top+1);
  for(int i = top; i <= bot; ++i)
    for(int j = left; j <= right; ++j)
      r.data[i-top][j-left] = data[i][j];
  *this = r; }

void image::scale(double s){
  image r((int)ceil(s * width), (int)ceil(s * height));
  //!
  *this = r; }

void image::rotate(double r){
  //!
}

void image::flip(line axis){
  //!
}

#endif
