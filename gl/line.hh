// LINE

#ifndef LINE_HH
#define LINE_HH

#include "image.hh"

struct line : object {

  int thick;
  point a,b;
  color fill;

  line(){}
  line(const point _a, const point _b){ a = _a, b = _b, fill = BLACK; }

  virtual void validate(){
    object::validate();
    assert(thick > 0, "line thickness not positive"); }

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
      bkgd->data[ii][jj] = fill;
      double dp = d;
      d += di;
      if((int)floor(d) != (int)floor(dp)) j += (xlong ? yi : xi);
      i += (xlong ? xi : yi); }
    bkgd->data[(int)floor(b.y)][(int)floor(b.x)] = fill; } };

#endif
