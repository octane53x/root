// TYPE

#ifndef TYPE_HH
#define TYPE_HH

#include "lang.hh"

struct Type {

  // Container for all types
  static umap<str, Type> registry;

  // False if only forward declared
  bool defined;
  // Total size of member vars
  size_t size;
  // Type name
  str name;
  // Containing classes
  str container;
  // Keyword modifiers
  uset<Keyword> keywords;
  // Member vars: var name -> type
  umap<str, Type*> vars;
  // Member fns: fn name
  uset<str> fns;
  // Direct base classes
  vec<_Type*> parents;

  Type();
  Type(const str& s);

  Type& operator=(const str& s);

  static bool contains(const str& full_name) const;
  static Type* search(const str& name, const str& container) const;
  static bool declared(const str& name, const str& container) const;
  static bool defined(const str& name, const str& container) const;

  static void declare(const str& name, const str& container);
  static void define(const str& name, const str& container,
      const vec<str>& parents, const vec<pair<str, str> >& vars,
      const vec<str>& fns);

  size_t size() const; };

Type::Type(): Type("Void") {}

Type::Type(const str& s){
  *this = s; }

Type& Type::operator=(const str& s){
  name = &registry->types[s].name;
  return *this; }

size_t Type::size() const {
  return registry->types[*name].size; }

bool Type::contains(const str& full_name) const {
  return types.find(full_name) != types.end(); }

// Searches all containing classes
// Returns NULL if not found
//!
Type* Type::search(const str& name, const str& container) const {
  if(contains(name)) return name;
  vec<str> ctrs = split(container, "::");
  for(int i = 0; i < ctrs.size(); ++i){
    str type = ctrs[0];
    for(int j = 1; j <= i; ++j)
      type += "::" + ctrs[j];
    if(contains(type)) return type; }
  return ""; }

bool Type::declared(const str& name, const str& container) const {
  return search(name, container) != ""; }

bool Type::defined(const str& name, const str& container) const {
  str type = search(name, container);
  return type != "" && types[type].defined == true; }

void Type::declare(const str& name, const str& container){
  assert(!declared(name, container), "Type.declare", "Type already declared");
  _Type node;
  node.defined = false;
  node.name = name;
  node.container = container;
  types[container + "::" + name] = node; }

// vars[i] = {full type name, var name}
void Type::define(const str& name, const str& container,
    const vec<str>& parents, const vec<pair<str, str> >& vars,
    const vec<str>& fns){
  const str _fn = "Type.define";
  assert(!defined(name, container), _fn, "Type already defined");
  _Type node;
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
    umap<str, _Type>::iterator it = types.find(s);
    assert(it != types.end(), _fn, "base class not yet added");
    node.parents.pb(&it->second); }
  types[container + "::" + name] = node; }

#endif
