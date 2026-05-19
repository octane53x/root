// CORE OPERATIONS

#ifndef CORE_OPS_HH
#define CORE_OPS_HH

#include "../str.hh"

ID TypeMgr::add(const str& name, const list<str>& parents){
  ID new_id = next_id++;
  types[new_id] = name;
  //! ancestry
  return new_id; }

#endif
