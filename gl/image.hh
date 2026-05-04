// IMAGE

#ifndef IMAGE_HH
#define IMAGE_HH

#include "object.hh"

// A 2D image of pixel colors
struct image : virtual object {

  // Pointer to color buffer
  ui* buf;
  // Pixel data, height by width, (0,0) Top-left
  // Used if buf is NULL
  vec<vec<color> > data;

  // Constructors
  image();
  image(const point& _size);

  // Operators
  image& operator=(const image& o);

  // Convert to string
  virtual str to_str() const;
  // Ensure valid state
  virtual void _validate(const str& fn);
  // Draw onto an image
  virtual void _draw(image* canvas, const viewport& view);

  // Clears contents and sets to a certain size with default pixel color
  void set_size(const point& _size);
  // Set pixel at (x,y) to color
  void set_pixel(const point& p, const color& c);
  // Fill the image with one color
  void fill(const color& c);
  // Replace all pixels of src color with dest color
  void replace(const color& src, const color& dest);
  // Replace all pixels that are not src color with dest color
  void replace_except(const color& src, const color& dest);

  // Return a scaled image. Does not modify current image
  image scale(const double s) const;
  // Rotate an image by a given degree < 360
  image rotate(const double deg) const;
  // Flip an image over an axis
  image flip(const uvec& axis) const; };

image::image():
  buf(NULL) {}

image::image(const point& _size): image() {
  set_size(_size); }

image& image::operator=(const image& o){
  pos = o.pos, size = o.size, data = o.data, buf = o.buf;
  validate("image.operator=");
  return *this; }

str image::to_str() const {
  return str("image ") + str(id) + str(" of size ") + size.to_str(); }

void image::_validate(const str& func){
  object::validate(func);
  assert(!(buf != NULL && !data.empty()), func, "data and buf both used");
  assert(!(data.empty() && (size.x > 0 || size.y > 0)), func,
      "image empty but positive size");
  assert(!(!data.empty() && (size.x <= 0 || size.y <= 0)), func,
      "image not empty but size not positive"); }

void image::_draw(image* canvas, const viewport& view){
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
      canvas->set_pixel(point(x, y), img.data[yt][xt]);
  validate("image.draw"); }

void image::set_size(const point& _size){
  assert(_size.xi() > 0 && _size.yi() > 0, "image.set_size",
      "image.setsize size.x/size.y params must be positive");
  size = _size;
  data.clear();
  for(int i = 0; i < size.yi(); ++i){
    data.pb(vec<color>());
    data[i].reserve(size.xi());
    for(int j = 0; j < size.xi(); ++j)
      data[i].pb(DEFAULT_COLOR); }
  validate("image.set_size"); }

void image::set_pixel(const point& p, const color& c){
  assert(p.xi() >= 0 && p.xi() < size.xi()
      && p.yi() >= 0 && p.yi() < size.yi(), "image.set_pixel", "OOB");
  if(c == CLEAR)
    return;
  if(buf != NULL)
    buf[p.yi() * size.xi() + p.xi()] = ((ui)c.r << 16) | ((ui)c.g << 8) | c.b;
  else
    data[p.y][p.x] = (c == CLEAR_PEN) ? CLEAR : c; }

void image::fill(const color& c){
  for(int y = 0; y < size.y; ++y)
    for(int x = 0; x < size.x; ++x)
      data[y][x] = c;
  validate("image.fill"); }

void image::replace(const color& src, const color& dest){
  for(int y = 0; y < size.y; ++y)
    for(int x = 0; x < size.x; ++x)
      if(data[y][x] == src)
        data[y][x] = dest;
  validate("image.replace"); }

void image::replace_except(const color& src, const color& dest){
  for(int y = 0; y < size.y; ++y)
    for(int x = 0; x < size.x; ++x)
      if(data[y][x] != src)
        data[y][x] = dest;
  validate("image.replace_except"); }

image image::scale(const double s) const {
  if(deq(s, 1.0)) return *this;
  image ret(point((int)ceil(s * size.x), (int)ceil(s * size.y)));

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

image image::rotate(const double deg) const {
  image r(size);
  //! rotate
  r.validate("image.rotate");
  return r; }

image image::flip(const uvec& axis) const {
  image r(size);
  //! flip
  r.validate("image.flip");
  return r; }

#endif
