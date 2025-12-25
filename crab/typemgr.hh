// TYPE MANAGER

#ifndef TYPE_MGR_HH
#define TYPE_MGR_HH

#include "../core/util.hh"

struct TypeMgr {

  struct Node {
    str name;
    vec<Node*> parents; };

  umap<str, Node> types;

  void add(const str& name, const vec<str>& parents); };

void TypeMgr::add(const str& name, const vec<str>& parents){
  if(types.find(name) != types.end())
    err("TypeMgr.add", "duplicate type");
  //! Add to types
}

#endif
