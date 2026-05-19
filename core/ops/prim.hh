// PRIMITIVE OPERATIONS

// Primitives should not be seen elsewhere

#ifndef PRIM_HH
#define PRIM_HH

#include "str.hh"

// Construct from primitive bool
Flag::Flag(const bool b){
  data = b ? TRUE : FALSE; }

// Construct from primitive char array
Str::Str(const char* s){
  int i = 0;
  while(s[i] != '\0')
    push(s[i++]); }

#endif
