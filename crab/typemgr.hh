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
  if(types.find(name) != types.end()) return;
  Node node;
  node.name = name;
  for(const str& s : parents){
    umap<str, Node>::iterator it = types.find(s);
    assert(it != types.end(), "TypeMgr.add", "base class not yet added");
    node.parents.pb(&it->second); }
  types[name] = node; }

#endif
