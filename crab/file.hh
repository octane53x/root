// FILE

#ifndef FILE_HH
#define FILE_HH

#include "../core/util.hh"

struct File {

  bool main;
  str name, dir;
  vec<str> includes, code; };

#endif
