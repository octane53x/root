// FUNCTION

#ifndef FN_HH
#define FN_HH

#include "language.hh"
#include "accessmgr.hh"

typedef llu ID;

struct Fn {

  struct FnCall {
    Fn* fn;
    vec<FnCall> params; };

  Control* control;
  ID id;
  str name;
  vec<FnCall> code;
  static llu next_id, scope;
  static Language* lang;
  static AccessMgr* access;

  Fn();

  Var call(const vec<FnCall>& params); };

Fn::Fn(): id(next_id++) {}

Var Fn::call(const vec<FnCall>& params){
  vec<Var> param_values;
  for(const FnCall& param : params){
    Var v = param.fn->call(param.params);
    //! Add param->call() to access and param_values
  }
  ++scope;

  Var r;
  if(*control == "USER"){
    for(const FnCall& instr : code){
      Var v = instr.fn->call(instr.params);
      //! Add declarations to access
      //! Set return var r
    }
  }else
    r = lang->process_fn(*control, param_values);

  //! Deallocate at scope
  //! Remove from access at scope
  --scope;
  return r; }

#endif
