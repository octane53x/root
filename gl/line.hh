// LINE SEGMENT

#ifndef LINE_HH
#define LINE_HH

#include "image.hh"

// Line segment between two points
struct line : virtual object {

  // Pixel thickness of line
  int thick;
  // Endpoints of segment
  point a,b;

  line();
  line(const point& _a, const point& _b);

  line& operator=(const line& o);

  virtual void validate(const str& func);
  virtual void draw(image* canvas, const viewport& view);

  double len() const;
  double slope() const;
  bool on_seg(const point& p) const;
  int side(const point& p) const;
  bool intersects(const line& o) const;
  point intersection(const line& o) const; };

// Set default member state
line::line(){
  type = "line"; }

// Construct with endpoints
line::line(const point& _a, const point& _b): line() {
  a = _a, b = _b; }

// Assignment operator
line& line::operator=(const line& o){
  a = o.a, b = o.b, thick = o.thick;
  return *this; }

// Ensure valid state
void line::validate(const str& func){
  object::validate(func);
  assert(thick > 0, func, "line thickness not positive"); }

// Draw line onto image
void line::draw(image* canvas, const viewport& view){
  point a2 = view.translate(a + pos, canvas->width, canvas->height);
  point b2 = view.translate(b + pos, canvas->width, canvas->height);
  // If line outside the viewport, don't bother iterating pixels
  if((a2.y < 0 && b2.y < 0) || (a2.x < 0 && b2.x < 0)
      || (a2.y > canvas->height && b2.y > canvas->height)
      || (a2.x > canvas->width && b2.x > canvas->width))
    return;
  // Otherwise draw valid pixels
  int xi = (a2.x < b2.x) ? 1 : -1;
  int yi = (a2.y < b2.y) ? 1 : -1;
  int w = ((int)floor(b2.x - a2.x) * xi);
  int h = ((int)floor(b2.y - a2.y) * yi);
  bool xlong = w > h;
  double di = xlong ? ((double)h / w) : ((double)w / h);
  int j = (int)floor(xlong ? a2.y : a2.x);
  double d = j;
  for(int i = (int)floor(xlong ? a2.x : a2.y);
      i != (int)floor(xlong ? b2.x : b2.y);){
    int ii = i, jj = j, s;
    if(xlong) s = ii, ii = jj, jj = s;
    //! thickness
    canvas->set_pixel(jj, ii, fill);
    double dp = d;
    d += di;
    if((int)floor(d) != (int)floor(dp)) j += (xlong ? yi : xi);
    i += (xlong ? xi : yi); }
  canvas->set_pixel((int)floor(b2.x), (int)floor(b2.y), fill); }

// Length of segment
double line::len() const {
  return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)
      + (b.z - a.z) * (b.z - a.z)); }

// Slope of line
double line::slope() const {
  return deq(a.x, b.x) ? INFD : (b.y - a.y) / (b.x - a.x); }

// Returns true if the point lies on the line segment
bool line::on_seg(const point& p) const {
  if(!deq(line(a, p).slope(), line(b, p).slope())) return false;
  double xmin = min(a.x, b.x), xmax = max(a.x, b.x),
      ymin = min(a.y, b.y), ymax = max(a.y, b.y);
  return dleq(xmin, p.x) && dleq(p.x, xmax)
      && dleq(ymin, p.y) && dleq(p.y, ymax); }

// Returns zero if the point is on the line extended from the segment
// Returns 1 or -1 for different sides of the line
int line::side(const point& p) const {
  double r = (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
  if(deq(r, 0.0)) return 0;
  else if(r > 0.0) return 1;
  else return -1; }

// Returns whether the two line segments intersect
bool line::intersects(const line& o) const {
  if(o.on_seg(a) || o.on_seg(b) || on_seg(o.a) || on_seg(o.b)) return true;
  int side1 = o.side(a), side2 = o.side(b),
      side3 = side(o.a), side4 = side(o.b);
  if(side1 == 0 || side2 == 0 || side3 == 0 || side4 == 0
      || (side1 > 0 && side2 > 0) || (side1 < 0 && side2 < 0)
      || (side3 > 0 && side4 > 0) || (side3 < 0 && side4 < 0))
    return false;
  return true; }

// Returns the intersection point on another segment, NULL_POINT if not
point line::intersection(const line& o) const {
  if(o.on_seg(a)) return a;
  if(o.on_seg(b)) return b;
  if(on_seg(o.a)) return o.a;
  if(on_seg(o.b)) return o.b;
  int side1 = o.side(a), side2 = o.side(b),
      side3 = side(o.a), side4 = side(o.b);
  if(side1 == 0 || side2 == 0 || side3 == 0 || side4 == 0
      || (side1 > 0 && side2 > 0) || (side1 < 0 && side2 < 0)
      || (side3 > 0 && side4 > 0) || (side3 < 0 && side4 < 0))
    return NULL_POINT;
  double a1 = b.y - a.y, b1 = a.x - b.x, c1 = b.x*a.y - a.x*b.y;
  double a2 = o.b.y - o.a.y, b2 = o.a.x - o.b.x,
      c2 = o.b.x*o.a.y - o.a.x*o.b.y;
  double x = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);
  double y = (c1*a2 - c2*a1) / (a1*b2 - a2*b1);
  return point(x, y); }

#endif
