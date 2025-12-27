// TYPE MANAGER

#ifndef TYPE_MGR_HH
#define TYPE_MGR_HH

#include "../core/util.hh"

struct Type;

struct TypeMgr {

  struct Node {
    size_t size;
    str name;
    // Key = var name
    umap<str, Type> vars;
    // Key = fn name
    umap<str, Fn*> fns;
    vec<Node*> parents; };

  umap<str, Node> types;

  void add(const str& name, const vec<str>& parents, const size_t size); };

void TypeMgr::add(const str& name, const vec<str>& parents, const size_t size){
  if(types.find(name) != types.end()) return;
  Node node;
  node.name = name;
  node.size = size;
  for(const str& s : parents){
    umap<str, Node>::iterator it = types.find(s);
    assert(it != types.end(), "TypeMgr.add", "base class not yet added");
    node.parents.pb(&it->second); }
  types[name] = node; }

#endif
