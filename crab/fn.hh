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

  bool if_executed, break_loop, continue_loop;
  Language::Control control;
  ID id;
  str name;
  vec<FnCall> code;
  static llu next_id, scope;
  static Language* lang;
  static AccessMgr* access;

  Fn();

  virtual void validate();

  Var call(const vec<FnCall>& params);
  Var run(); };

Fn::Fn(): if_executed(false), break_loop(false), continue_loop(false),
    control(Language::INVALID), id(next_id++) {}

void Fn::validate(){
  assert(control != Language::INVALID, "Fn.validate", "invalid control"); }

Var Fn::call(const vec<FnCall>& params){
  validate();
  vec<Var> param_values;
  for(const FnCall& param : params){
    Var v = param.fn->call(param.params);
    //! Add param->call() to access and param_values
  }
  ++scope;

  Var r;
  if(control == Language::USER){
    r = run();

  }else if(control == Language::IF){


  }else if(control == Language::ELSE){


  }else if(control == Language::FOR){


  }else if(control == Language::WHILE){


  }else if(control == Language::BREAK){


  }else if(control == Language::CONTINUE){


  }else if(control == Language::RETURN){
    assert(param_values.size() == 1, "Fn.call",
        "return didn't have one parameter");
    r = param_values[0];

  }else
    r = lang->process_fn(control, param_values);

  //! Deallocate at scope
  //! Remove from access at scope
  --scope;
  validate();
  return r; }

Var Fn::run(){
  for(int i = 0; i < code.size(); ++i){
    const FnCall& instr = code[i];
    Var v = instr.fn->call(instr.params);
    if(instr.fn->control == Language::RETURN){
      assert(i == code.size() - 1, "Fn.run",
          "return call precedes dangling code");
      return v; }
    //! Add declaration to access ???
  }
  return Var(); }

#endif
