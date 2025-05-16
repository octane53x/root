// GL OBJ

#ifndef GL_OBJ_HH
#define GL_OBJ_HH

#include "../core/thing.hh"

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

struct point : thing {
  int x,y,z;
  point(){ type = "point"; }
  point(int _x, int _y): point() { x = _x, y = _y, z = 0; }
  point(int _x, int _y, int _z): point() { x = _x, y = _y, z = _z; }
  double dist(const point& p){
    int a = abs(x - p.x), b = abs(y - p.y), c = abs(z - p.z);
    return sqrt(a*a + b*b + c*c); } };

struct image;

struct line : thing {
  int thick;
  point a,b;
  color col;
  line(){ type = "line"; }
  line(point _a, point _b): line() { a = _a, b = _b; }
  void draw(image* img);
};

#endif
