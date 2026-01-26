// VARIABLE

#ifndef VAR_HH
#define VAR_HH

#include "type.hh"
#include "allocator.hh"

// Variable
struct Var {

  // Memory allocator
  static Allocator allocator;
  // Container for all variables
  // Key = full name (obj name.obj name.name)
  static umap<str, Var> access;
  // Variables per scope so we know what to remove when a function completes
  // Key 1: scope, Key 2: var name
  static umap<llu, uset<Var*> > scope_table;

  // Memory address
  void* addr;
  // Variable type
  Type* type;
  // Containing class
  Type* container;
  // Variable name
  str name;
  // Keyword modifiers, or reference
  uset<Mod> mods;
  // Member values, key = var name
  uset<Var*> members;

  Var();
  Var(const str& _type, void* _addr, size_t size);

  static void declare(const llu scope, const Var& var);
  static void descope(const llu scope);

  void construct();
  void destruct(); };

Allocator Var::allocator;
umap<str, Var> Var::access;
umap<llu, uset<Var*> > Var::scope_table;

// Set default member state
Var::Var(): type(&Type::registry["Void"]), addr(NULL) {}

// Construct var from parameters
Var::Var(const str& _type, void* _addr, size_t size){
  type = &Type::registry[_type];
  construct();
  memcpy(addr, _addr, size); }

// Add variable to access and allocate
void Var::declare(const llu scope, const Var& var){
  umap<str, Var>::iterator it = access.find(var.name);
  assert(it == access.end(), "Var.declare",
      "variable already added to access (declared twice?)");
  access[var.name] = var;
  umap<llu, uset<Var*> >::iterator it2 = scope_table.find(scope);
  if(it2 == scope_table.end()){
    uset<Var*> m;
    m.insert(&access[var.name]);
    scope_table[scope] = m;
  }else
    it2->second.insert(&access[var.name]); }

// Remove all variables at scope from access and deallocate
void Var::descope(const llu scope){
  const str _fn = "Var.descope";
  umap<llu, uset<Var*> >::iterator it = scope_table.find(scope);
  assert(it != scope_table.end(), _fn, "removing nonexistent scope");
  for(Var* var : it->second){
    assert(var != NULL, _fn, "var in scope_table is NULL");
    umap<str, Var>::iterator it2 = access.find(var->name);
    assert(it2 != access.end(),
        _fn, "variable in scope_table but not in access");
    access.erase(it2); }
  scope_table.erase(it); }

//!
// Allocate variable
void Var::construct(){}

//!
// Recursively deallocate members
// Does NOT follow references
void Var::destruct(){}

#endif
