// FUNCTION

#ifndef FN_HH
#define FN_HH

#include "var.hh"
#include "accessmgr.hh"

struct Fn {

  llu id;
  str name;
  vec<Fn*> code, params;
  static llu scope;
  static AccessMgr* access;

  Var call(); };

Var Fn::call(){
  for(Fn* param : params){
    //! Add param->call() to access
  }
  ++scope;
  Var r;
  for(Fn* instr : code){
    //! Call fns
    //! Set return var r
  }
  //! Deallocate at scope
  //! Remove from access at scope
  --scope;
  return r;
}

#endif
