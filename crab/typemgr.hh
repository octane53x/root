// TYPE MANAGER

#ifndef TYPE_MGR_HH
#define TYPE_MGR_HH

#include "../core/util.hh"

struct Type;
struct Fn;

struct TypeMgr {

  struct Node {
    // False if only forward declared
    bool defined;
    // Total size of member vars
    size_t size;
    // Type name
    str name;
    // Member vars: var name -> type name
    umap<str, str> vars;
    // Member fns: fn name
    uset<str> fns;
    // Direct base classes
    vec<Node*> parents; };

  umap<str, Node> types;

  bool declared(const str& name);
  bool defined(const str& name);
  void declare(const str& name);
  void define(const str& name, const vec<str>& parents,
      const vec<pair<str, str> >& vars, const vec<str>& fns); };

bool TypeMgr::declared(const str& name){
  return types.find(name) != types.end(); }

bool TypeMgr::defined(const str& name){
  return declared(name) && types[name].defined == true; }

void TypeMgr::declare(const str& name){
  assert(!declared(name), "TypeMgr.add", "Type already declared");
  Node node;
  node.defined = false;
  node.name = name;
  types[name] = node; }

// vars[i] = {type name, var name}
void TypeMgr::define(const str& name, const vec<str>& parents,
    const vec<pair<str, str> >& vars, const vec<str>& fns){
  const str _fn = "TypeMgr.add";
  assert(!defined(name), _fn, "Type already defined");
  Node node;
  node.defined = true;
  node.name = name;
  node.size = 0;
  for(const pair<str, str>& p : vars){
    assert(defined(p.first), _fn, "Member type does not exist");
    assert(node.vars.find(p.second) == node.vars.end(),
        _fn, "Member variable name already declared");
    node.vars[p.second] = p.first;
    node.size += types[p.first].size; }
  for(const str& fn : fns){
    assert(node.fns.find(fn) == node.fns.end(),
        _fn, "Member function name already declared");
    node.fns.insert(fn); }
  for(const str& s : parents){
    umap<str, Node>::iterator it = types.find(s);
    assert(it != types.end(), "TypeMgr.add", "base class not yet added");
    node.parents.pb(&it->second); }
  types[name] = node; }

#endif
