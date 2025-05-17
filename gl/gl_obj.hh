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

const color BLACK = color(0, 0, 0),
            RED = color(255, 0, 0),
            LIME = color(0, 255, 0),
            BLUE = color(0, 0, 255),
            YELLOW = color(255, 255, 0),
            MAGENTA = color(255, 0, 255),
            CYAN = color(0, 255, 255),
            WHITE = color(255, 255, 255),
            MAROON = color(128, 0, 0),
            GREEN = color(0, 128, 0),
            NAVY = color(0, 0, 128),
            OLIVE = color(128, 128, 0),
            PURPLE = color(128, 0, 128),
            TEAL = color(0, 128, 128),
            ORANGE = color(255, 165, 0),
            VIOLET = color(127, 0, 255),
            BROWN = color(150, 75, 0),
            GRAY = color(128, 128, 128),
            SILVER = color(192, 192, 192),
            GOLD = color(255, 215, 0),
            INDIGO = color(75, 0, 130),
            PINK = color(255, 192, 203);

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
  color fill_color;
  line(){ type = "line"; }
  line(point _a, point _b): line() { a = _a, b = _b, fill_color = BLACK; }
  void draw(image* img); };

struct triangle : thing {
  point a,b,c;
  color fill_color;
  triangle(){ type = "triangle"; }
  triangle(point _a, point _b, point _c): triangle() {
      a = _a, b = _b, c = _c, fill_color = BLACK; }
  void draw(image* img); };

struct rectangle : thing {
  int width, height;
  point topleft;
  color fill_color;
  rectangle(){ type = "rectangle"; }
  rectangle(point tl, int w, int h): rectangle() {
      topleft = tl, width = w, height = h, fill_color = BLACK; }
  void draw(image* img); };

struct circle : thing {
  int radius;
  point center;
  color fill_color;
  circle(){ type = "circle"; }
  circle(point c, int r): circle() {
      center = c, radius = r, fill_color = BLACK; }
  void draw(image* img); };

#endif
