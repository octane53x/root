// OBJ

#ifndef OBJ_HH
#define OBJ_HH

#include "thing.hh"
//! #include "num.hh"

struct point : thing {
  int x,y,z;
  point(){ type = "point"; }
  point(int _x, int _y): point() { x = _x, y = _y, z = 0; }
  point(int _x, int _y, int _z): point() { x = _x, y = _y, z = _z; }
  double dist(const point& p){
    int a = abs(x - p.x), b = abs(y - p.y), c = abs(z - p.z);
    return sqrt(a*a + b*b + c*c); } };

struct color : thing {
  uchar r,g,b,a;
  color(){ type = "color"; }
  color(uchar _r, uchar _g, uchar _b):
      color() { r = _r, g = _g, b = _b, a = 0; }
  color(uchar _r, uchar _g, uchar _b, uchar _a):
      color() { r = _r, g = _g, b = _b, a = _a; }
  bool operator==(const color& c){
    return r == c.r && g == c.g && b == c.b && a == c.a; }
  bool operator!=(const color& c){
    return !(*this == c); }
  color avg(const color& c){
    return color((r+c.r)>>1, (g+c.g)>>1, (b+c.b)>>1, (a+c.a)>>1); } };

template <typename T>
struct graph : thing {
  vec<T> nodes;
  umap<int, int> edges;
  graph(){ type = "graph"; } };

struct image {
  point size;
  vec<vec<color> > data;
  image(){}
  image(point _size): size(_size) {
    for(int i = 0; i < size.y; ++i){
      data.pb(vec<color>());
      for(int j = 0; j < size.x; ++j)
        data[i].pb(color(255, 0, 255));
    }
  }
};

struct font {
  str name;
  umap<char, image> syms;
  font(){}
};

#endif
