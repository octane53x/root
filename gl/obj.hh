// OBJ

#ifndef OBJ_HH
#define OBJ_HH

//! #include "num.hh"
#include "graph.hh"

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
  double x,y,z;
  point(){}
  point(double _x, double _y){ x = _x, y = _y, z = 0; }
  point(double _x, double _y, double _z){ x = _x, y = _y, z = _z; }

  point& operator+=(const point& p){
    x += p.x, y += p.y, z += p.z;
    return *this; }
  point operator+(const point& p){
    point r = *this;
    r += p;
    return r; }
  point& operator-=(const point& p){
    x -= p.x, y -= p.y, z -= p.z;
    return *this; }
  point operator-(const point& p){
    point r = *this;
    r -= p;
    return r; }
  point& operator*=(double n){
    x *= n, y *= n, z *= n;
    return *this; }
  point operator*(double n){
    point r = *this;
    r *= n;
    return r; }
  point& operator/=(double n){
    x /= n, y /= n, z /= n;
    return *this; }
  point operator/(double n){
    point r = *this;
    r /= n;
    return r; }

  double dist(const point& p){
    double a = x - p.x, b = y - p.y, c = z - p.z;
    return sqrt(a*a + b*b + c*c); }
  void rotate(const point& p, double deg){ // deg in radians
    *this -= p;
    point p2 = *this;
    x = p2.x * cos(deg) - p2.y * sin(deg);
    y = p2.x * sin(deg) + p2.y * cos(deg);
    *this += p; } };

struct uvec {
  double deg;
  uvec(){ deg = 0.0; }
  uvec(double d){ deg = d; }
  void rotate(double d){
    deg += d;
    while(deg >= PI * 2.0)
      deg -= PI * 2.0; } };

struct image;

struct line {
  int thick;
  point a,b;
  color fill_color;
  line(){}
  line(point _a, point _b){ a = _a, b = _b, fill_color = BLACK; }
  void draw(image* bkgd); };

struct polygon {
  vec<point> points;
  color fill_color;
  polygon(){}
  polygon(const vec<point>& _p){ points = _p; }
  void draw(image* bkgd); };

struct circle {
  double radius;
  point center;
  color fill_color;
  circle(){}
  circle(point c, int r){
      center = c, radius = r, fill_color = BLACK; }
  void draw(image* bkgd); };

struct image {
  int width, height;
  vec<vec<color> > data;
  image(){}
  image(int _w, int _h){ set_size(_w, _h); }
  void set_size(int _w, int _h){
    width = _w, height = _h;
    data.clear();
    for(int i = 0; i < height; ++i){
      data.pb(vec<color>());
      for(int j = 0; j < width; ++j)
        data[i].pb(WHITE); } }
  // image.hh
  void fix();
  void scale(double s);
  void rotate(double r);
  void flip(line axis); };

enum mov_type {
  NONE,
  ROOT,
  PATH,
  CURVED_PATH,
  ORBIT };

struct object;

struct movement {
  mov_type type;
  int path_pos;
  double vel, path_prog; // vel = pixels/sec
  vec<point> path;
  object* root;
  movement(): type(mov_type::NONE), vel(0.0), path_pos(0), path_prog(0.0),
      root(NULL) {} };

struct object {
  llu id;
  static llu next_id;
  point pos, img_root;
  image img;
  movement mov;

  object(){ id = next_id++; }
  point move(double ms); // move.hh

  void draw(image* bkgd){
    for(int i = 0; i < img.height; ++i)
      for(int j = 0; j < img.width; ++j){
        int y = (int)floor(pos.y - img_root.y) + i;
        int x = (int)floor(pos.x - img_root.x) + j;
        if(img.data[i][j] != WHITE
            && y >= 0 && y < bkgd->height && x >= 0 && x < bkgd->width)
          bkgd->data[y][x] = img.data[i][j]; } } };

llu object::next_id = 1;

#endif
