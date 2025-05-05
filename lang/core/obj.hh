// OBJ

#ifndef OBJ_HH
#define OBJ_HH

#include "util.hh"

struct point {
  int x,y,z;
  point(){}
  point(int _x, int _y): x(_x), y(_y), z(0) {}
  point(int _x, int _y, int _z): x(_x), y(_y), z(_z) {}
  double dist(const point& p){
    int a = abs(x - p.x), b = abs(y - p.y), c = abs(z - p.z);
    return sqrt(a*a + b*b + c*c); } };

struct color {
  uchar r,g,b,a;
  color(){}
  color(uchar _r, uchar _g, uchar _b): r(_r), g(_g), b(_b) {}
  color(uchar _r, uchar _g, uchar _b, uchar _a): r(_r), g(_g), b(_b), a(_a) {}
  color avg(const color& c){
    return color((r+c.r)>>1, (g+c.g)>>1, (b+c.b)>>1, (a+c.a)>>1); } };

template <typename T>
struct graph {
  vec<T> nodes;
  umap<int, int> edges;
  graph(){} };

#endif
