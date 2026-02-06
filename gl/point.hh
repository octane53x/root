// POINT

#ifndef POINT_HH
#define POINT_HH

#include "uvec.hh"
#include "ipoint.hh"

// A 2D or 3D coordinate point
struct point {

  // (x,y,z) coordinates
  double x, y, z;

  point();
  point(const double _x, const double _y);
  point(const double _x, const double _y, const double _z);
  point(const ipoint& ipt);

  bool operator==(const point& p) const;
  bool operator!=(const point& p) const;
  point operator+(const point& p) const;
  point operator-(const point& p) const;
  point operator*(const double n) const;
  point operator/(const double n) const;
  point& operator+=(const point& p);
  point& operator-=(const point& p);
  point& operator*=(const double n);
  point& operator/=(const double n);

  virtual void validate(const str& func);
  virtual str to_str() const;

  ipoint to_ipt() const;
  double dist(const point& p) const;

  void rotate(const point& p, const uvec& uv, const double deg); };

// Return this when the function cannot accurately return a point but needs to
const point NULL_POINT = point(DBL_MIN, DBL_MIN, DBL_MIN);

// Set default member state
point::point(): x(0.0), y(0.0), z(0.0) {}

// Construct as a 2D point with (x,y)
point::point(const double _x, const double _y):
    x(_x), y(_y), z(0.0) {}

// Construct as a 3D point with (x,y,z)
point::point(const double _x, const double _y, const double _z):
    x(_x), y(_y), z(_z) {}

// Construct from an integer point
point::point(const ipoint& ipt):
    x(ipt.x), y(ipt.y), z(0.0) {}

// Equals comparator
bool point::operator==(const point& p) const {
  return deq(x, p.x) && deq(y, p.y) && deq(z, p.z); }

// Not equals comparator
bool point::operator!=(const point& p) const {
  return !(*this == p); }

// Add
point point::operator+(const point& p) const {
  point r = *this;
  r += p;
  return r; }

// Subtract
point point::operator-(const point& p) const {
  point r = *this;
  r -= p;
  return r; }

// Multiply
point point::operator*(const double n) const {
  point r = *this;
  r *= n;
  return r; }

// Divide
point point::operator/(const double n) const {
  point r = *this;
  r /= n;
  return r; }

// Add to self
point& point::operator+=(const point& p){
  x += p.x, y += p.y, z += p.z;
  return *this; }

// Subtract from self
point& point::operator-=(const point& p){
  x -= p.x, y -= p.y, z -= p.z;
  return *this; }

// Multiply self
point& point::operator*=(const double n){
  x *= n, y *= n, z *= n;
  return *this; }

// Divide self
point& point::operator/=(const double n){
  x /= n, y /= n, z /= n;
  return *this; }

// Implemented to remove abstraction
void point::validate(const str& func){}

// Convert to string
str point::to_str() const {
  return str("(") + to_string(x) + str(", ") + to_string(y) + str(", ")
      + to_string(z) + str(")"); }

// Get integer point
ipoint point::to_ipt() const {
  return ipoint((int)round(x), (int)round(y)); }

// Distance from this point to another
double point::dist(const point& p) const {
  double a = x - p.x, b = y - p.y, c = z - p.z;
  return sqrt(a*a + b*b + c*c); }

// Rotate the point around another point and a unit vector from that point,
// by some degrees. Use uv=(0,0,-1) for 2D
void point::rotate(const point& p, const uvec& uv, const double deg){
  //! deg in radians, use degrees
  //! this is 2D logic, figure out 3D
  *this -= p;
  point p2 = *this;
  x = p2.x * cos(deg) - p2.y * sin(deg);
  y = p2.x * sin(deg) + p2.y * cos(deg);
  *this += p; }

// Allow point to be hashed as a map key
namespace std {
  template <>
  struct hash<point> {
    size_t operator()(const point& p) const {
      size_t h1 = hash<double>()(p.x);
      size_t h2 = hash<double>()(p.y);
      size_t h3 = hash<double>()(p.z);
      return h1 ^ (h2 << 1) ^ (h3 << 2); } }; }

#endif
