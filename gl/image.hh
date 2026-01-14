// IMAGE

#ifndef IMAGE_HH
#define IMAGE_HH

#include "color.hh"
#include "object.hh"

// A 2D image of pixel colors
struct image : virtual object {

  // Image size
  ipoint size;
  // Pixel data, size.y by width, (0,0) Top-left
  vec<vec<color> > data;

  image();
  image(const ipoint& sz);

  image& operator=(const image& o);

  virtual void validate(const str& func);
  virtual void draw(image* canvas, const viewport& view);

  bool empty() const;

  void set_size(const ipoint& sz);
  void set_pixel(const ipoint& p, const color& c);
  void fix(const color& c);
  void fill(const color& c);
  void replace(const color& src, const color& dest);
  void replace_except(const color& src, const color& dest);

  image scale(const double s) const;
  image rotate(const double deg) const;
  image flip(const uvec& axis) const; };

// Set default member state
image::image(){
  type = "image";
  size = ipoint(0, 0); }

// Construct at a certain size
image::image(const ipoint& sz){
  set_size(sz); }

// Assignment operator
image& image::operator=(const image& o){
  pos = o.pos, size = o.size, data = o.data;
  validate("image.operator=");
  return *this; }

// Ensure valid state
void image::validate(const str& func){
  object::validate(func);
  assert(!(data.empty() && (size.x > 0 || size.y > 0)), func,
      "image empty but positive size");
  assert(!(!data.empty() && (size.x <= 0 || size.y <= 0)), func,
      "image not empty but size not positive"); }

// Draw onto an image
void image::draw(image* canvas, const viewport& view){
  double ratio = view.size_out / view.size_in;
  image img = scale(ratio);
  img.pos = view.translate_out(pos);
  // If image outside the viewport, don't bother iterating pixels
  if(img.pos.y + img.size.y < 0 || img.pos.y > canvas->size.y
      || img.pos.x + img.size.x < 0 || img.pos.x > canvas->size.x) return;
  // Otherwise draw all valid pixels
  for(int y = max(0, (int)round(pos.y)), yt = max(0, -(int)round(pos.y));
      y < min(canvas->size.y, (int)round(pos.y)+size.y); ++y, ++yt)
    for(int x = max(0, (int)round(pos.x)), xt = max(0, -(int)round(pos.x));
        x < min(canvas->size.x, (int)round(pos.x)+size.x); ++x, ++xt)
      canvas->set_pixel(ipoint(x, y), img.data[yt][xt]);
  validate("image.draw"); }

// True if no pixels stored
bool image::empty() const {
  return data.empty(); }

// Clears contents and sets to a certain size with default pixel color
void image::set_size(const ipoint& sz){
  assert(sz.x > 0 && sz.y > 0, "image.set_size",
      "image.setsize size.x/size.y params must be positive");
  size = sz;
  data.clear();
  for(int i = 0; i < size.y; ++i){
    data.pb(vec<color>());
    data[i].reserve(size.x);
    for(int j = 0; j < size.x; ++j)
      data[i].pb(DEFAULT_COLOR); }
  validate("image.set_size"); }

// Set pixel at (x,y) to color
inline void image::set_pixel(const ipoint& p, const color& c){
  if(c == CLEAR) return;
  if(c != CLEAR_PEN){
    if(p.x >= 0 && p.x < size.x && p.y >= 0 && p.y < size.y)
      data[p.y][p.x] = c;
    return;
  }else if(p.x >= 0 && p.x < size.x && p.y >= 0 && p.y < size.y)
    data[p.y][p.x] = CLEAR; }

// Delete margins of color
void image::fix(const color& c){
  int left,right,top,bot;
  for(int i = 0; i < size.x; ++i){
    bool found = false;
    for(int j = 0; j < size.y; ++j)
      if(data[j][i] != c){ found = true; break; }
    if(found){ left = i; break; } }
  for(int i = size.x-1; i >= 0; --i){
    bool found = false;
    for(int j = 0; j < size.y; ++j)
      if(data[j][i] != c){ found = true; break; }
    if(found){ right = i; break; } }
  for(int j = 0; j < size.y; ++j){
    bool found = false;
    for(int i = 0; i < size.x; ++i)
      if(data[j][i] != c){ found = true; break; }
    if(found){ top = j; break; } }
  for(int j = size.y-1; j >= 0; --j){
    bool found = false;
    for(int i = 0; i < size.x; ++i)
      if(data[j][i] != c){ found = true; break; }
    if(found){ bot = j; break; } }
  image r(ipoint(right-left+1, bot-top+1));
  for(int i = top; i <= bot; ++i)
    for(int j = left; j <= right; ++j)
      r.data[i-top][j-left] = data[i][j];
  *this = r;
  validate("image.fix"); }

// Fill the image with one color
void image::fill(const color& c){
  for(int y = 0; y < size.y; ++y)
    for(int x = 0; x < size.x; ++x)
      data[y][x] = c; }

// Replace all pixels of src color with dest color
void image::replace(const color& src, const color& dest){
  for(int y = 0; y < size.y; ++y)
    for(int x = 0; x < size.x; ++x)
      if(data[y][x] == src)
        data[y][x] = dest;
  validate("image.replace"); }

// Replace all pixels that are not src color with dest color
void image::replace_except(const color& src, const color& dest){
  for(int y = 0; y < size.y; ++y)
    for(int x = 0; x < size.x; ++x)
      if(data[y][x] != src)
        data[y][x] = dest;
  validate("image.replace_except"); }

// Return a scaled image. Does not modify current image
image image::scale(const double s) const {
  if(deq(s, 1.0)) return *this;
  image ret(ipoint((int)ceil(s * size.x), (int)ceil(s * size.y)));

  // Enlarge image
  if(s > 1.0){
    double ys = 0.0;
    for(int yo = 0, yi = 0; yo < ret.size.y; ++yo){
      double xs = 0.0;
      for(int xo = 0, xi = 0; xo < ret.size.x; ++xo){
        if(dgeq(xs, s)){
          xs -= s;
          ++xi; }
        ret.data[yo][xo] = data[yi][xi];
        xs += 1.0; }
      if(dgeq(ys, s)){
        ys -= s;
        ++yi; }
      ys += 1.0; }

  // Shrink image
  }else{
    double ys = 0.0;
    int yo = 0;
    for(int yi = 0; yo < ret.size.y && yi < data.size(); ++yo){
      double xs = 0.0;
      int xo = 0;
      for(int xi = 0; xo < ret.size.x && xi < data[yi].size(); ++xo){
        // Average the input pixel colors
        int r = 0, g = 0, b = 0, custom = 0, c = 0;
        color special;
        double yst = ys;
        for(int yt = yi; yt < data.size() && dlt(yst, 1.0); yst += s, ++yt){
          double xst = xs;
          for(int xt = xi; xt < data[yt].size() && dlt(xst, 1.0);
              xst += s, ++xt, ++c){
            if(data[yt][xt].custom != color::NONE){
              ++custom;
              special = data[yt][xt];
            }else
              r += data[yt][xt].r, g += data[yt][xt].g, b += data[yt][xt].b; } }
        int n = c - custom;
        color col = (custom > c / 2) ? special : color(r / n, g / n, b / n);

        // Set output pixel
        ret.data[yo][xo] = col;
        while(dlt(xs, 1.0)){
          xs += s;
          ++xi; }
        xs -= 1.0; }
      if(xo < ret.size.x)
        ret.data[yo][xo] = ret.data[yo][xo - 1];
      while(dlt(ys, 1.0)){
        ys += s;
        ++yi; }
      ys -= 1.0; }
    if(yo < ret.size.y)
      for(int x = 0; x < ret.size.x; ++x)
        ret.data[yo][x] = ret.data[yo - 1][x]; }

  ret.pos = pos;
  ret.validate("image.scale");
  return ret; }

// Rotate an image by a given degree < 360
image image::rotate(const double deg) const {
  image r(size);
  //! rotate
  r.validate("image.rotate");
  return r; }

// Flip an image over an axis
image image::flip(const uvec& axis) const {
  image r(size);
  //! flip
  r.validate("image.flip");
  return r; }

#endif
