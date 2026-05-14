// CORE INCLUDE

#ifndef CORE_HH
#define CORE_HH

#include "str.hh"

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

ID TypeMgr::add(const str& name, const list<str>& parents){
  ID new_id = next_id++;
  types[new_id] = name;
  //! ancestry
  return new_id; }

#endif
