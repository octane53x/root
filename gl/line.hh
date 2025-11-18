// LINE SEGMENT

#ifndef LINE_HH
#define LINE_HH

#include "image.hh"

struct line : object {

  int thick;
  point a,b;

  line(){}
  line(const point& _a, const point& _b){ a = _a, b = _b, fill = BLACK; }

  virtual void validate(){
    object::validate();
    assert(thick > 0, "line thickness not positive"); }

  double slope() const {
    return deq(a.x, b.x) ? INFD : (b.y - a.y) / (b.x - a.x); }

  // Returns true if the point lies on the line segment
  bool on_seg(const point& p) const {
    if(!deq(line(a, p).slope(), line(b, p).slope())) return false;
    double xmin = min(a.x, b.x), xmax = max(a.x, b.x),
        ymin = min(a.y, b.y), ymax = max(a.y, b.y);
    return dleq(xmin, p.x) && dleq(p.x, xmax)
        && dleq(ymin, p.y) && dleq(p.y, ymax); }

  // Returns zero if the point is on the line extended from the segment
  // Returns 1 or -1 for different sides of the line
  int side(const point& p) const {
    double r = (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
    if(deq(r, 0.0)) return 0;
    else if(r > 0.0) return 1;
    else return -1; }

  // Returns whether the two line segments intersect
  bool intersects(const line& o) const {
    if(o.on_seg(a) || o.on_seg(b) || on_seg(o.a) || on_seg(o.b)) return true;
    int side1 = o.side(a), side2 = o.side(b),
        side3 = side(o.a), side4 = side(o.b);
    if(side1 == 0 || side2 == 0 || side3 == 0 || side4 == 0
        || (side1 > 0 && side2 > 0) || (side1 < 0 && side2 < 0)
        || (side3 > 0 && side4 > 0) || (side3 < 0 && side4 < 0))
      return false;
    return true; }

  virtual point update(double ms){ return point(0, 0); }

  void draw(image* bkgd){
    int xi = (a.x < b.x) ? 1 : -1;
    int yi = (a.y < b.y) ? 1 : -1;
    int w = ((int)floor(b.x - a.x) * xi);
    int h = ((int)floor(b.y - a.y) * yi);
    bool xlong = w > h;
    double di = xlong ? ((double)h / w) : ((double)w / h);
    int j = (int)floor(xlong ? a.y : a.x);
    double d = j;
    for(int i = (int)floor(xlong ? a.x : a.y);
        i != (int)floor(xlong ? b.x : b.y);){
      int ii = i, jj = j, s;
      if(xlong) s = ii, ii = jj, jj = s;
      //! thickness: for(int t =
      bkgd->set_pixel(jj, ii, fill);
      double dp = d;
      d += di;
      if((int)floor(d) != (int)floor(dp)) j += (xlong ? yi : xi);
      i += (xlong ? xi : yi); }
    bkgd->data[(int)floor(b.y)][(int)floor(b.x)] = fill; } };

#endif
