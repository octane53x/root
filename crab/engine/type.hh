// TYPE

#ifndef TYPE_HH
#define TYPE_HH

#include "lang.hh"

// Object type
struct Type {

  // Container for all types
  // Key = full name (container::name)
  static umap<str, Type> registry;

  // False if only forward declared
  bool defined;
  // Total size of member vars
  size_t size;
  // Type name and containing classes
  str name, container;
  // Keyword modifiers
  uset<Mod> keywords;
  // Member vars: Key = var name
  umap<str, Type*> vars;
  // Member fns
  uset<Fn*> fns;
  // Direct base classes
  uset<Type*> parents;

  virtual void validate();

  static Type* search(const str& name, const str& container) const;
  static bool declared(const str& name, const str& container) const;
  static bool defined(const str& name, const str& container) const;

  static void declare(const str& name, const str& container);
  static void define(const str& name, const str& container,
      const vec<str>& parents, const vec<pair<str, str> >& vars,
      const vec<str>& fns); };

// Ensure valid state, O(N)
// To be called when full definition is expected
void Type::validate(){
  const str& _fn = "Type.validate";
  assert(defined, _fn, "type not defined");
  assert(name != "", _fn, "type has no name");
  for(pair<str, Type*> p : vars)
    assert(p.second != NULL && p.second->defined,
        _fn, "member variable type not defined");
  for(Fn* fn : fns)
    assert(fn != NULL && fn->defined, _fn, "member function not defined");
  for(Type* type : parents)
    assert(type != NULL && type->defined, _fn, "base type not defined"); }

// Searches all containing classes
// Returns NULL if not found
Type* Type::search(const str& name, const str& container) const {
  if(contains(registry, name)) return &registry[name];
  vec<str> ctrs = split(container, "::");
  str ctr = "";
  for(int i = 0; i < ctrs.size(); ++i){
    ctr += ctrs[i] + "::";
    if(contains(registry, ctr + type)) return &registry[ctr + type]; }
  return NULL; }

bool Type::declared(const str& name, const str& container) const {
  return search(name, container) != NULL; }

bool Type::defined(const str& name, const str& container) const {
  Type* type = search(name, container);
  return type != NULL && type->defined; }

void Type::declare(const str& name, const str& container){
  assert(!declared(name, container), "Type.declare", "type already declared");
  Type type;
  type.defined = false;
  type.name = name;
  type.container = container;
  registry[container + "::" + name] = type; }

void Type::define(const str& name, const str& container,
    const vec<Type*>& parents, const vec<VarDecl>& vars, const vec<Fn*>& fns){
  const str _fn = "Type.define";
  assert(!defined(name, container), _fn, "type already defined");
  Type type;
  type.defined = true;
  type.name = name;
  type.container = container;
  type.size = 0;
  for(const VarDecl& vd : vars){
    assert(vd.first != NULL && vd.first->defined,
        _fn, "member type is not defined");
    assert(!contains(type.vars, vd.second),
        _fn, "member variable name already declared");
    type.vars[vd.second] = vd.first;
    type.size += vd.first->size; }
  for(Fn* fn : fns){
    assert(fn != NULL && !contains(type.fns, fn),
        _fn, "member function name already declared");
    type.fns.insert(fn); }
  for(Type* base : parents){
    assert(base != NULL && base->defined, _fn, "base class not defined");
    type.parents.insert(base); }
  registry[container + "::" + name] = type; }

#endif
