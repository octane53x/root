// VARIABLE

#ifndef VAR_HH
#define VAR_HH

#include "type.hh"
#include "allocator.hh"

typedef uchar Char;
typedef llu Block;

struct Var {

  // Memory allocator
  static Allocator allocator;
  // All variables in scope to fetch the memory address
  static umap<str, Var> access;
  // Variables per scope so we know what to remove when a function completes
  // Key 1: scope, Key 2: var name
  static umap<llu, umap<str, Var> > scope_table;

  Type* type;
  void* addr;
  str name;
  // Key = var name
  umap<str, Var> members;

  Var();
  Var(const str& _type, const Char val);
  Var(const str& _type, const Block val);

  static Var get(const str& name);
  static void add(const llu scope, const Var& var);
  static void remove(const llu scope); };

  void destruct(); };

Var::Var(): constant(false), addr(NULL) {}

Var::Var(const str& _type, const Char val): type(_type), name("") {
  addr = allocator->allocate(SIZE_CHAR);
  *((Char*)addr) = val; }

Var::Var(const str& _type, const Block val): type(_type), name("") {
  addr = allocator->allocate(SIZE_BLOCK);
  *((Block*)addr) = val; }

Var Var::get(const str& name){
  umap<str, Var>::iterator it = access.find(name);
  assert(it != access.end(), "Var.get", "variable not accessible");
  return it->second; }

void Var::add(const llu scope, const Var& var){
  umap<str, Var>::iterator it = access.find(var.name);
  assert(it == access.end(), "Var.add",
      "variable already added to access (declared twice?)");
  access[var.name] = var;
  umap<llu, umap<str, Var> >::iterator it2 = scope_table.find(scope);
  if(it2 == scope_table.end()){
    umap<str, Var> m;
    m[var.name] = var;
    scope_table[scope] = m;
  }else
    it2->second[var.name] = var; }

void Var::remove(const llu scope){
  umap<llu, umap<str, Var> >::iterator it = scope_table.find(scope);
  assert(it != scope_table.end(), "Var.remove",
      "removing nonexistent scope");
  for(pair<str, Var> p : it->second){
    umap<str, Var>::iterator it2 = access.find(p.first);
    assert(it2 != access.end(), "Var.remove",
        "variable in scope_table but not in access");
    access.erase(it2); }
  scope_table.erase(it); }

// Recursively deallocate members
// Does NOT follow references
void Var::deallocate(){}

#endif
