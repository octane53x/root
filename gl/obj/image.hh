// IMAGE

#ifndef IMAGE_HH
#define IMAGE_HH

#include "color.hh"
#include "uvec.hh"

struct image {

  bool is_flp;
  int width, height, width_orig, height_orig;
  double rot, scl;
  uvec flp;
  // Height by width, (0,0) Top-left
  // orig only set if modified
  vec<vec<color> > data, orig;

  image(){}
  image(int _w, int _h): rot(0.0), scl(1.0), is_flp(false) {
    set_size(_w, _h); }

  void set_size(int _w, int _h){
    if(!orig.empty()) err("image.set_size after modification");
    width = _w, height = _h;
    data.clear();
    for(int i = 0; i < height; ++i){
      data.pb(vec<color>());
      for(int j = 0; j < width; ++j)
        data[i].pb(CLEAR); } }

  void set_pixel(int i, int j, color c){
    if(orig.empty()) data[j][i] = c;
    else{
      orig[j][i] = c;
      adjust(); } }

  void scale(double s){
    scl *= s;
    adjust(); }

  void rotate(double deg){
    rot += deg;
    while(rot >= 360.0)
      rot -= 360.0;
    adjust(); }

  void flip(uvec axis){
    //!
    adjust();
  }

  void adjust(){
    if(scl == 1.0 && rot == 0.0 && !is_flp){
      if(!orig.empty()) data = orig;
      orig.clear();
      return; }
    if(orig.empty()) orig = data;
    else data = orig;
    if(scl != 1.0) scale_pix();
    if(rot != 0.0) rotate_pix();
    if(is_flp) flip_pix(); }

  // Replace color with CLEAR and delete margins
  void fix(color c){
    if(!orig.empty()) err("image.fix after modification");
    int left,right,top,bot;
    for(int i = 0; i < width; ++i){
      bool found = false;
      for(int j = 0; j < height; ++j)
        if(data[j][i] != c){ found = true; break; }
      if(found){ left = i; break; } }
    for(int i = width-1; i >= 0; --i){
      bool found = false;
      for(int j = 0; j < height; ++j)
        if(data[j][i] != c){ found = true; break; }
      if(found){ right = i; break; } }
    for(int j = 0; j < height; ++j){
      bool found = false;
      for(int i = 0; i < width; ++i)
        if(data[j][i] != c){ found = true; break; }
      if(found){ top = j; break; } }
    for(int j = height-1; j >= 0; --j){
      bool found = false;
      for(int i = 0; i < width; ++i)
        if(data[j][i] != c){ found = true; break; }
      if(found){ bot = j; break; } }
    image r(right-left+1, bot-top+1);
    for(int i = top; i <= bot; ++i)
      for(int j = left; j <= right; ++j)
        r.data[i-top][j-left] = data[i][j];
    for(int i = 0; i < r.height; ++i)
      for(int j = 0; j < r.width; ++j)
        if(r.data[i][j] == c) r.data[i][j] = CLEAR;
    *this = r; }

  void scale_pix(){
    double s = scl;
    image r((int)ceil(s * width), (int)ceil(s * height));
    if(s < 1.0){ // Scale down
      s = 1.0 / s;
      double sx = (double)width / r.width, sy = (double)height / r.height;
      int ny = height, ry = r.height, dy = 0;
      for(int y = 0; y < r.height; ++y){
        double sy2 = (double)ny / ry;
        int py = (sy2 > sy) ? (int)ceil(sy) : (int)floor(sy);
        int nx = width, rx = r.width, dx = 0;
        for(int x = 0; x < r.width; ++x){
          double sx2 = (double)nx / rx;
          int px = (sx2 > sx) ? (int)ceil(sx) : (int)floor(sx);
          r.data[y][x] = data[dy][dx];
          dx += px; }
        dy += py; }

    }else if(s > 1.0){ // Scale up
      double sx = (double)r.width / width, sy = (double)r.height / height;
      int ny = height, ry = r.height, dy = 0;
      for(int y = 0; y < height; ++y){
        double sy2 = (double)ry / ny;
        int py = (sy2 > sy) ? (int)ceil(sy) : (int)floor(sy);
        int nx = width, rx = r.width, dx = 0;
        for(int x = 0; x < width; ++x){
          double sx2 = (double)rx / nx;
          int px = (sx2 > sx) ? (int)ceil(sx) : (int)floor(sx);
          for(int i = 0; i < py; ++i)
            for(int j = 0; j < px; ++j)
              r.data[dy+i][dx+j] = data[y][x];
          dx += px; }
        dy += py; } }
    width = r.width;
    height = r.height;
    data = r.data; }

  void rotate_pix(){
    //!
  }

  void flip_pix(){
    //!
  } };

#endif
