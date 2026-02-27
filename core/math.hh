// MATH

#ifndef MATH_HH
#define MATH_HH

#include "const.hh"

// Greatest common denominator
int gcd(const int a, const int b){
  return b ? gcd(b, a % b) : a; }

// Random number generator, but up to a 64 bit value (default RAND_MAX is less)
llu lrand(){
  int bits = 1;
  llu t = 1;
  while(t < RAND_MAX)
    ++bits, t <<= 1;
  llu r = 0;
  for(int i = 0; i < 64/bits; ++i)
    r = r * rand() + rand();
  return r; }

// Curved random number generator, favoring smaller numbers
//! Curve param
int crand(){
  const int RMAX = 1000000;
  int r = 1;
  while(r == 1)
    r = (int)floor((double)RMAX / (rand() % RMAX + 1));
  return r - 1; }

#endif
