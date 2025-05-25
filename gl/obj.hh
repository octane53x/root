// OBJ

#ifndef OBJ_HH
#define OBJ_HH

#include "util.hh"

template <typename T>
struct graph {
  vec<T> nodes;
  umap<int, vec<int> > edges;
  graph(){} };

struct color {
  uchar r,g,b;
  color(): r(0), g(0), b(0) {}
  color(uchar _r, uchar _g, uchar _b): r(_r), g(_g), b(_b) {}
  bool operator==(const color& c){
    return r == c.r && g == c.g && b == c.b; }
  bool operator!=(const color& c){
    return !(*this == c); }
  color avg(const color& c){
    return color(((ui)r+c.r)>>1, ((ui)g+c.g)>>1, ((ui)b+c.b)>>1); } };

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

struct point {
  int x,y,z;
  point(){}
  point(int _x, int _y){ x = _x, y = _y, z = 0; }
  point(int _x, int _y, int _z){ x = _x, y = _y, z = _z; }
  double dist(const point& p){
    int a = abs(x - p.x), b = abs(y - p.y), c = abs(z - p.z);
    return sqrt(a*a + b*b + c*c); } };

struct image;

struct line {
  int thick;
  point a,b;
  color fill_color;
  line(){}
  line(point _a, point _b): line() { a = _a, b = _b, fill_color = BLACK; }
  void draw(image* bkgd); };

struct polygon {
  vec<point> points;
  color fill_color;
  polygon(){}
  polygon(const vec<point>& _p): polygon() { points = _p; }
  void draw(image* bkgd); };

struct circle {
  int radius;
  point center;
  color fill_color;
  circle(){}
  circle(point c, int r): circle() {
      center = c, radius = r, fill_color = BLACK; }
  void draw(image* bkgd); };

struct image {
  point size;
  vec<vec<color> > data;
  image(){}
  image(point _size): image() {
    size = _size;
    for(int i = 0; i < size.y; ++i){
      data.pb(vec<color>());
      for(int j = 0; j < size.x; ++j)
        data[i].pb(WHITE); } }
  void fix();
  void scale(double s);
  void rotate(double r);
  void flip(line axis); };

enum mov_type {
  ROOT,
  PATH,
  ORBIT };

struct object;

struct movement {
  mov_type type;
  object* root;
  vec<point> path;
  movement(){} };

struct object {
  double vel;
  point pos, img_root;
  image img;
  movement mov;
  object(){}
  void move(int ms);
  void draw(image* bkgd){
    for(int i = 0; i < img.size.y; ++i)
      for(int j = 0; j < img.size.x; ++j){
        int y = pos.y - img_root.y + i;
        int x = pos.x - img_root.x + j;
        if(img.data[i][j] != WHITE
            && y >= 0 && y < bkgd->size.y && x >= 0 && x < bkgd->size.x)
          bkgd->data[y][x] = img.data[i][j]; } } };

#endif
