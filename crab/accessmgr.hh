// MEMORY ACCESS MANAGER

#ifndef ACCESS_MGR_HH
#define ACCESS_MGR_HH

#include "../core/util.hh"

struct AccessMgr {

  // Key 1: scope, Key 2: var name
  umap<llu, umap<str, Var> > table; };

#endif
