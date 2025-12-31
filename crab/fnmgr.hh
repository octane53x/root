// FUNCTION MANAGER

#ifndef FN_MGR_HH
#define FN_MGR_HH

#include "../core/util.hh"

struct Fn;

struct FnMgr {

  // Key = full name
  umap<str, Fn> fns; };

#endif
