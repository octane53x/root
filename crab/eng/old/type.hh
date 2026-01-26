// TYPE

#ifndef TYPE_HH
#define TYPE_HH

#include "lang.hh"

struct Var;
struct Fn;

// Object type
struct Type {

  // Member variable
  struct _Var {
    Type* type;
    str name; };

  // Container for all types
  // Key = full name (container::name)
  static umap<str, Type> registry;

  // False if only forward declared
  bool _defined;
  // Total size of member vars
  size_t size;
  // Containing class
  Type* container;
  // Type name
  str name;
  // Keyword modifiers
  uset<Mod> mods;
  // Direct base classes, pointers to Type::registry
  uset<Type*> parents;
  // Member vars, key = var name
  umap<str, _Var> vars;
  // Member fns, pointers to Fn::registry
  uset<Fn*> fns;

  Type();

  // Defined in fn.hh
  virtual void validate();

  static str full_name(const str& name, Type* container);
  static Type* search(const str& name, Type* container);
  static bool declared(const str& name, Type* container);
  static bool defined(const str& name, Type* container);
  static Type* declare(const str& name, Type* container);
  static Type* define(const str& name, Type* container,
      const vec<Type*>& parents, const vec<_Var>& vars, const vec<Fn*>& fns); };

umap<str, Type> Type::registry;

// Set default member state
Type::Type(): _defined(false), size(0) {}

// Recursively determine the full name (Obj::Obj::etc)
str Type::full_name(const str& name, Type* container){
  if(container == NULL) return name;
  return full_name(container->name, container->container) + "::" + name; }

// Searches all containing classes
// Returns NULL if not found
Type* Type::search(const str& name, Type* container){
  if(contains(registry, name)) return &registry[name];
  if(container == NULL) return NULL;
  vec<str> ctrs = split(full_name(container->name, container->container), "::");
  str ctr = "";
  for(int i = 0; i < ctrs.size(); ++i){
    ctr += ctrs[i] + "::";
    if(contains(registry, ctr + name)) return &registry[ctr + name]; }
  return NULL; }

// Has the type been declared
bool Type::declared(const str& name, Type* container){
  return search(name, container) != NULL; }

// Has the type been defined
bool Type::defined(const str& name, Type* container){
  Type* type = search(name, container);
  return type != NULL && type->_defined; }

// Declare the type
Type* Type::declare(const str& name, Type* container){
  assert(!declared(name, container), "Type.declare", "type already declared");
  Type type;
  type._defined = false;
  type.name = name;
  type.container = container;
  return &(registry[full_name(name, container)] = type); }

// Define the type
Type* Type::define(const str& name, Type* container, const vec<Type*>& parents,
    const vec<_Var>& vars, const vec<Fn*>& fns){
  const str _fn = "Type.define";
  assert(!defined(name, container), _fn, "type already defined");
  Type* type;
  if(declared(name, container))
    type = search(name, container);
  else
    type = &(registry[full_name(name, container)] = Type());
  type->_defined = true;
  type->name = name;
  type->container = container;
  type->size = 0;
  for(const _Var& var : vars){
    assert(var.type != NULL && var.type->_defined,
        _fn, "member type is not defined");
    assert(var.name != "" && !contains(type->vars, var.name),
        _fn, "member variable name already declared");
    type->vars[var.name] = var;
    type->size += var.type->size; }
  for(Fn* fn : fns){
    assert(fn != NULL && !contains(type->fns, fn),
        _fn, "member function name already declared");
    type->fns.insert(fn); }
  for(Type* base : parents){
    assert(base != NULL && base->_defined, _fn, "base class not defined");
    type->parents.insert(base); }
  return type; }

#endif
