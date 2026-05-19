// CORE UTILITY OPERATIONS

#ifndef CORE_OPS_UTIL_HH
#define CORE_OPS_UTIL_HH

#include "../str.hh"

void print(const str& s){
#ifdef _WIN32
  ofstream fs(DEBUG_FILE.c_str(), ios::app);
  fs << s.data;
  fs.close();
#else
  printf("%s", s.c_str());
#endif
}

void assert(const bool b, const str& func, const str& msg){
  if(!b) err(func, msg); }

#endif
