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
    // Containing class
    str container;
    // Member vars: var name -> type name
    umap<str, str> vars;
    // Member fns: fn name
    uset<str> fns;
    // Direct base classes
    vec<Node*> parents; };

  umap<str, Node> types;

  bool contains(const str& full_name);
  str search(const str& name, const str& container);
  bool declared(const str& name, const str& container);
  bool defined(const str& name, const str& container);
  void declare(const str& name, const str& container);
  void define(const str& name, const str& container, const vec<str>& parents,
      const vec<pair<str, str> >& vars, const vec<str>& fns); };

bool TypeMgr::contains(const str& full_name){
  return types.find(full_name) != types.end(); }

// Return full type name by searching all containing classes
// Returns empty string if not found
str TypeMgr::search(const str& name, const str& container){
  if(contains(name)) return name;
  vec<str> ctrs = split(container, "::");
  for(int i = 0; i < ctrs.size(); ++i){
    str type = ctrs[0];
    for(int j = 1; j <= i; ++j)
      type += "::" + ctrs[j];
    if(contains(type)) return type; }
  return ""; }

bool TypeMgr::declared(const str& name, const str& container){
  return search(name, container) != ""; }

bool TypeMgr::defined(const str& name, const str& container){
  str type = search(name, container);
  return type != "" && types[type].defined == true; }

void TypeMgr::declare(const str& name, const str& container){
  assert(!declared(name, container), "TypeMgr.add", "Type already declared");
  Node node;
  node.defined = false;
  node.name = name;
  node.container = container;
  types[container + "::" + name] = node; }

// vars[i] = {full type name, var name}
void TypeMgr::define(const str& name, const str& container,
    const vec<str>& parents, const vec<pair<str, str> >& vars,
    const vec<str>& fns){
  const str _fn = "TypeMgr.add";
  assert(!defined(name, container), _fn, "Type already defined");
  Node node;
  node.defined = true;
  node.name = name;
  node.container = container;
  node.size = 0;
  for(const pair<str, str>& p : vars){
    assert(defined(p.first, container), _fn, "Member type does not exist");
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
  types[container + "::" + name] = node; }

#endif
