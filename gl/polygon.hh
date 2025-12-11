// POLYGON

#ifndef POLYGON_HH
#define POLYGON_HH

#include "line.hh"
#include "image.hh"

// Shape made of line segment edges
struct polygon : virtual object {

  // Points in order of connection by edges
  vec<point> points;

  polygon();

  polygon& operator=(const polygon& o);

  virtual void validate(const str& func);
  virtual void draw(image* canvas, const viewport& view);

  point size() const;
  bool inside(const point& p) const;
  bool intersects(const polygon& o) const;
  double area() const;

  void add(const point& p); };

// Set default member state
polygon::polygon(){
  type = "polygon"; }

// Assignment operator
polygon& polygon::operator=(const polygon& o){
  pos = o.pos;
  points = o.points;
  validate("polygon.operator=");
  return *this; }

// Ensure valid state
void polygon::validate(const str& func){
  object::validate(func);
  assert(points.size() > 2, func, "polygon does not have more than 2 points"); }

// Draw polygon to image
void polygon::draw(image* canvas, const viewport& view){
  double xmin = INFD, xmax = -INFD, ymin = INFD, ymax = -INFD;
  for(int i = 0; i < points.size(); ++i)
    xmin = min(xmin, points[i].x+pos.x), xmax = max(xmax, points[i].x+pos.x),
    ymin = min(ymin, points[i].y+pos.y), ymax = max(ymax, points[i].y+pos.y);
  vec<line> edges;
  for(int i = 0; i < points.size()-1; ++i)
    edges.pb(line(points[i]+pos, points[i+1]+pos));
  edges.pb(line(points.back()+pos, points[0]+pos));
  double yi = view.size_in / view.size_out / 2.0;
  for(double y = ymin; y <= ymax; y += yi){
    line scan(point(xmin, y), point(xmax, y));
    vec<point> inter;
    for(int i = 0; i < edges.size(); ++i){
      point p = scan.intersection(edges[i]);
      if(p != NULL_POINT)
        inter.pb(p); }
    sort(inter.begin(), inter.end(),
        [](const point& a, const point& b){ return a.x < b.x; });
    for(int i = 0; i < inter.size()-1; i += 2){
      if(deq(inter[i].x, inter[i+1].x)){
        ++i;
        continue; }
      line s(inter[i], inter[i+1]);
      s.fill = fill;
      s.draw(canvas, view); } }
  validate("polygon.draw"); }

// Return width and height of box around the polygon
point polygon::size() const {
  double xmin = INFD, xmax = -INFD, ymin = INFD, ymax = -INFD;
  for(int i = 0; i < points.size(); ++i)
    xmin = min(xmin, points[i].x),
    xmax = max(xmax, points[i].x),
    ymin = min(ymin, points[i].y),
    ymax = max(ymax, points[i].y);
  return point(xmax - xmin, ymax - ymin); }

// Return whether the point is inside the polygon
//! broken
bool polygon::inside(const point& p) const {
  line ray(p, point(p.x, p.y + size().y));
  int n = 0;
  for(int i = 0; i < points.size()-1; ++i)
    if(line(points[i], points[i+1]).intersects(ray)) ++n;
  if(line(points.back(), points[0]).intersects(ray)) ++n;
  for(int i = 0; i < points.size(); ++i)
    if(ray.on_seg(points[i])) --n;
  return n & 1; }

// Return whether the polygon intersects another polygon
bool polygon::intersects(const polygon& o) const {
  // True if any points lie inside the other
  for(int i = 0; i < points.size(); ++i)
    if(o.inside(points[i])) return true;
  for(int i = 0; i < o.points.size(); ++i)
    if(inside(o.points[i])) return true;
  // True if any edges intersect other edges
  vec<line> v1, v2;
  for(int i = 0; i < points.size()-1; ++i)
    v1.pb(line(points[i], points[i+1]));
  v1.pb(line(points.back(), points[0]));
  for(int i = 0; i < o.points.size()-1; ++i)
    v2.pb(line(o.points[i], o.points[i+1]));
  v2.pb(line(o.points.back(), o.points[0]));
  for(int i = 0; i < v1.size(); ++i)
    for(int j = 0; j < v2.size(); ++j)
      if(v1[i].intersects(v2[j])) return true;
  // Otherwise false
  return false; }

// Return the area of the polygon
double polygon::area() const {
  double p1 = 0.0, p2 = 0.0;
  for(int i = 0; i < points.size()-1; ++i)
    p1 += points[i].x * points[i+1].y,
    p2 += points[i].y * points[i+1].x;
  p1 += points.back().x * points[0].y;
  p2 += points.back().y * points[0].x;
  return fabs(p1 - p2) / 2.0; }

// Add a point to the polygon
void polygon::add(const point& p){
  points.pb(p); }

#endif
