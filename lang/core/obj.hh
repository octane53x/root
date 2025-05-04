// OBJ

#ifndef OBJ_HH
#define OBJ_HH

#include "util.hh"

struct point {
  int x,y,z;
  point(){}
  point(int _x, int _y): x(_x), y(_y), z(0) {}
  point(int _x, int _y, int _z): x(_x), y(_y), z(_z) {}
  double dist(point& p){
    int a = abs(x - p.x), b = abs(y - p.y), c = abs(z - p.z);
    return sqrt(a*a + b*b + c*c); } };

template <typename T>
struct graph {
  vec<T> nodes;
  map<int, int> edges;
  graph(){} };

#endif
