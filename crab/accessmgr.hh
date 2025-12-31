// MEMORY ACCESS MANAGER

#ifndef ACCESS_MGR_HH
#define ACCESS_MGR_HH

#include "var.hh"

struct AccessMgr {

  // All variables in scope to fetch the memory address
  umap<str, Var> var_table;
  // Variables per scope so we know what to remove when a function completes
  // Key 1: scope, Key 2: var name
  umap<llu, umap<str, Var> > scope_table;

  Var get(const str& name);
  void add(const llu scope, const Var& var);
  void remove(const llu scope); };

Var AccessMgr::get(const str& name){
  umap<str, Var>::iterator it = var_table.find(name);
  assert(it != var_table.end(), "AccessMgr.get", "variable not accessible");
  return it->second; }

void AccessMgr::add(const llu scope, const Var& var){
  umap<str, Var>::iterator it = var_table.find(var.name);
  assert(it == var_table.end(), "AccessMgr.add",
      "variable already added to access (declared twice?)");
  var_table[var.name] = var;
  umap<llu, umap<str, Var> >::iterator it2 = scope_table.find(scope);
  if(it2 == scope_table.end()){
    umap<str, Var> m;
    m[var.name] = var;
    scope_table[scope] = m;
  }else
    it2->second[var.name] = var; }

void AccessMgr::remove(const llu scope){
  umap<llu, umap<str, Var> >::iterator it = scope_table.find(scope);
  assert(it != scope_table.end(), "AccessMgr.remove",
      "removing nonexistent scope");
  for(pair<str, Var> p : it->second){
    umap<str, Var>::iterator it2 = var_table.find(p.first);
    assert(it2 != var_table.end(), "AccessMgr.remove",
        "variable in scope_table but not in var_table");
    var_table.erase(it2); }
  scope_table.erase(it); }

#endif
