// IMAGE

#ifndef IMAGE_HH
#define IMAGE_HH

#include "color.hh"
#include "object.hh"

// Declared virtual so button is only one object
struct image : virtual object {

  int width, height;
  // Height by width, (0,0) Top-left
  vec<vec<color> > data;

  image(): width(0), height(0) {}
  image(int w, int h){ set_size(w, h); }

  virtual void validate(){
    object::validate();
    assert((data.empty() && width == 0 && height == 0)
        || (!data.empty() && width > 0 && height > 0), "image broken"); }

  bool empty() const {
    return data.empty(); }

  // Clears contents
  void set_size(int w, int h){
    assert(w > 0 && h > 0, "image.setsize width/height must be > 0");
    width = w, height = h;
    data.clear();
    for(int i = 0; i < height; ++i){
      data.pb(vec<color>());
      for(int j = 0; j < width; ++j)
        data[i].pb(CLEAR); } }

  // Set pixel to color
  void set_pixel(int x, int y, color c){
    if(x >= 0 && x < width && y >= 0 && y < height)
      data[y][x] = c; }

  // Delete margins of color
  void fix(color c){
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
    *this = r; }

  void replace(color src, color dest){
    for(int y = 0; y < height; ++y)
      for(int x = 0; x < width; ++x)
        if(data[y][x] == src)
          data[y][x] = dest; }

  void replace_except(color src, color dest){
    for(int y = 0; y < height; ++y)
      for(int x = 0; x < width; ++x)
        if(data[y][x] != src)
          data[y][x] = dest; }

  //! Not working for all scales
  image scale(double s){
    if(deq(s, 1.0)) return *this;
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
          r.set_pixel(x, y, data[dy][dx]);
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
              r.set_pixel(dx+j, dy+i, data[y][x]);
          dx += px; }
        dy += py; } }
    r.pos = pos;
    return r; }

  image rotate(double deg){
    image r(width, height);
    //!
    return r; }

  image flip(uvec axis){
    image r(width, height);
    //!
    return r; }

  virtual point update(double ms){ return point(0, 0); }

  void draw(image* bkgd, viewport view){
    double ratio = min(bkgd->width, bkgd->height) / view.size;
    image img = scale(ratio);
    img.pos = view.translate(pos, bkgd->width, bkgd->height);
    // If image outside the viewport, don't bother iterating pixels
    if(img.pos.y + img.height < 0 || img.pos.y > bkgd->height
        || img.pos.x + img.width < 0 || img.pos.x > bkgd->width) return;
    // Otherwise draw all valid pixels
    for(int y = max(0, (int)round(pos.y)), yt = max(0, -(int)round(pos.y));
        y < min(bkgd->height, (int)round(pos.y)+height); ++y, ++yt)
      for(int x = max(0, (int)round(pos.x)), xt = max(0, -(int)round(pos.x));
          x < min(bkgd->width, (int)round(pos.x)+width); ++x, ++xt)
        if(data[yt][xt] != CLEAR)
          bkgd->set_pixel(x, y, img.data[yt][xt]); } };

#endif
