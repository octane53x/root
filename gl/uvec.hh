// UNIT VECTOR

#ifndef UVEC_HH
#define UVEC_HH

// Unit vector whose coordinates are of distance 1.0 from origin
struct uvec {

  // Coordinates of the tip of the unit vector
  double x, y, z;

  uvec();
  uvec(const double _x, const double _y, const double _z);

  virtual void validate(const str& func);

  void rotate(const uvec& uv, const double deg); };

// Set default member state
uvec::uvec(): x(0.0), y(0.0), z(1.0) {}

// Construct with angle
uvec::uvec(const double _x, const double _y, const double _z):
    x(_x), y(_y), z(_z) {
  validate("uvec(x,y,z)"); }

// Ensure valid state
void uvec::validate(const str& func){
  double len = sqrt(x * x + y * y + z * z);
  if(deq(len, 1.0)) return;
  x /= len, y /= len, z /= len; }

// Rotate unit vector around another unit vector by some degrees
void uvec::rotate(const uvec& uv, const double deg){
  //! rotate
}

#endif
