// 8-BIT MEMORY UNIT

#ifndef BYTE_HH
#define BYTE_HH

#include "thing.hh"

struct Str;
enum class PrintMode;

// 8 bits of data
struct Byte : virtual Thing {

  // Number of bits
  static const int BITS = 8;

  // Primitive C++ storage
  uchar data;

  // Constructors
  Byte();
  Byte(const uchar b);

  // Convert to string
  // Defined in conv.hh
  virtual Str to_str(const PrintMode mode) const;

  // Sets to zero
  void reset(); };

Byte::Byte(){
  reset(); }

Byte::Byte(const uchar b){
  data = b; }

void Byte::reset(){
  data = 0; }

#endif
