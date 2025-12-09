// IMAGE

#ifndef IMAGE_HH
#define IMAGE_HH

#include "color.hh"
#include "object.hh"

// A 2D image of pixel colors
struct image : virtual object {

  // Image size
  int width, height;
  // Pixel data, height by width, (0,0) Top-left
  vec<vec<color> > data;

  image();
  image(const int w, const int h);

  image& operator=(const image& o);

  virtual void validate(const str& func);
  virtual void draw(image* canvas, const viewport& view);

  bool empty() const;

  void set_size(const int w, const int h);
  void set_pixel(const int x, const int y, const color& c);
  void fix(const color& c);
  void replace(const color& src, const color& dest);
  void replace_except(const color& src, const color& dest);

  image scale(const double s) const;
  image rotate(const double deg) const;
  image flip(const uvec& axis) const; };

// Set default member state
image::image(): width(0), height(0) {
  type = "image"; }

// Construct at a certain size
image::image(const int w, const int h){
  set_size(w, h); }

// Assignment operator
image& image::operator=(const image& o){
  pos = o.pos;
  width = o.width, height = o.height, data = o.data;
  validate("image.operator=");
  return *this; }

// Ensure valid state
void image::validate(const str& func){
  object::validate(func);
  assert(!(data.empty() && (width > 0 || height > 0)), func,
      "image empty but positive size");
  assert(!(!data.empty() && (width <= 0 || height <= 0)), func,
      "image not empty but size not positive"); }

// Draw onto an image
void image::draw(image* canvas, const viewport& view){
  double ratio = min(canvas->width, canvas->height) / view.size;
  image img = scale(ratio);
  img.pos = view.translate(pos, canvas->width, canvas->height);
  // If image outside the viewport, don't bother iterating pixels
  if(img.pos.y + img.height < 0 || img.pos.y > canvas->height
      || img.pos.x + img.width < 0 || img.pos.x > canvas->width) return;
  // Otherwise draw all valid pixels
  for(int y = max(0, (int)round(pos.y)), yt = max(0, -(int)round(pos.y));
      y < min(canvas->height, (int)round(pos.y)+height); ++y, ++yt)
    for(int x = max(0, (int)round(pos.x)), xt = max(0, -(int)round(pos.x));
        x < min(canvas->width, (int)round(pos.x)+width); ++x, ++xt)
      if(data[yt][xt] != CLEAR)
        canvas->set_pixel(x, y, img.data[yt][xt]);
  validate("image.draw"); }

// True if no pixels stored
bool image::empty() const {
  return data.empty(); }

// Clears contents and sets to a certain size with default pixel color
void image::set_size(const int w, const int h){
  assert(w > 0 && h > 0, "image.set_size",
      "image.setsize width/height params must be positive");
  width = w, height = h;
  data.clear();
  for(int i = 0; i < height; ++i){
    data.pb(vec<color>());
    for(int j = 0; j < width; ++j)
      data[i].pb(DEFAULT_COLOR); }
  validate("image.set_size"); }

// Set pixel at (x,y) to color
void image::set_pixel(const int x, const int y, const color& c){
  if(x >= 0 && x < width && y >= 0 && y < height)
    data[y][x] = c;
  validate("image.set_pixel"); }

// Delete margins of color
void image::fix(const color& c){
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
  *this = r;
  validate("image.fix"); }

// Replace all pixels of src color with dest color
void image::replace(const color& src, const color& dest){
  for(int y = 0; y < height; ++y)
    for(int x = 0; x < width; ++x)
      if(data[y][x] == src)
        data[y][x] = dest;
  validate("image.replace"); }

// Replace all pixels that are not src color with dest color
void image::replace_except(const color& src, const color& dest){
  for(int y = 0; y < height; ++y)
    for(int x = 0; x < width; ++x)
      if(data[y][x] != src)
        data[y][x] = dest;
  validate("image.replace_except"); }

// Return a scaled image. Does not modify current image
//! Not working
image image::scale(const double s) const {
  if(deq(s, 1.0)) return *this;
  image r((int)ceil(s * width), (int)ceil(s * height));
  if(s < 1.0){ // Scale down
    //s = 1.0 / s;
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
  r.validate("image.scale");
  return r; }

// Rotate an image by a given degree < 360
image image::rotate(const double deg) const {
  image r(width, height);
  //! rotate
  r.validate("image.rotate");
  return r; }

// Flip an image over an axis
image image::flip(const uvec& axis) const {
  image r(width, height);
  //! flip
  r.validate("image.flip");
  return r; }

#endif
