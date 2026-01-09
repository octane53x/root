// INTEGER POINT

#ifndef IPOINT_HH
#define IPOINT_HH

#include "../core/thing.hh"

struct ipoint : virtual thing {

  int x, y;

  ipoint();
  ipoint(const int _x, const int _y);

  bool operator==(const ipoint& o) const;

  virtual void validate(const str& func);
  virtual str to_str() const; };

ipoint::ipoint():
    x(0), y(0) {}

ipoint::ipoint(const int _x, const int _y):
    x(_x), y(_y) {}

bool ipoint::operator==(const ipoint& o) const {
  return x == o.x && y == o.y; }

void ipoint::validate(const str& func){}

str ipoint::to_str(){
  return "(" + to_string(x) + ", " + to_string(y) + ")"; }

// Allow ipoint to be hashed as a map key
namespace std {
  template <>
  struct hash<ipoint> {
    size_t operator()(const ipoint& p) const {
      size_t h1 = hash<int>()(p.x);
      size_t h2 = hash<int>()(p.y);
      return h1 ^ (h2 << 1); } }; }

#endif
