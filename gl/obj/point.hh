// POINT

// Hashable

#ifndef POINT_HH
#define POINT_HH

#include "uvec.hh"

struct point {

  double x, y, z;

  point(): x(0.0), y(0.0), z(0.0) {}
  point(const double _x, const double _y): x(_x), y(_y), z(0.0) {}
  point(const double _x, const double _y, const double _z):
      x(_x), y(_y), z(_z) {}

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
  point& operator*=(const double n){
    x *= n, y *= n, z *= n;
    return *this; }
  point operator*(const double n){
    point r = *this;
    r *= n;
    return r; }
  point& operator/=(const double n){
    x /= n, y /= n, z /= n;
    return *this; }
  point operator/(const double n){
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
    *this += p; }

  void rotate(const point& p, const uvec uv, double deg){
    //!
  }};

namespace std {
  template <>
  struct hash<point> {
    size_t operator()(const point& p) const {
      size_t h1 = hash<double>{}(p.x);
      size_t h2 = hash<double>{}(p.y);
      size_t h3 = hash<double>{}(p.z);
      return h1 ^ (h2 << 1) ^ (h3 << 2); } }; }

#endif
