// DOUBLE UTILITY

#ifndef DOUBLE_HH
#define DOUBLE_HH

#include "const.hh"

// Double equals comparator
bool deq(const double a, const double b){
  return fabs(a - b) < SAFE_ZERO; }

// Double less than or equals comparator
bool dleq(const double a, const double b){
  return a < b || deq(a, b); }

// Double greater than or equals comparator
bool dgeq(const double a, const double b){
  return a > b || deq(a, b); }

// Double less than comparator
bool dlt(const double a, const double b){
  return a < b && !deq(a, b); }

// Double greater than comparator
bool dgt(const double a, const double b){
  return a > b && !deq(a, b); }

#endif
