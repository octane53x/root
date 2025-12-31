// FUNCTION

#ifndef FN_HH
#define FN_HH

#include "language.hh"
#include "fnmgr.hh"
#include "accessmgr.hh"

struct Fn {

  struct FnCall {
    Fn* fn;
    vec<FnCall> params; };

  static llu scope;
  static Driver driver;

  bool defined;
  bool if_executed, break_loop;
  Language::Control control;
  str name, container;
  uset<Mod> keywords;
  // params[i] = {full type, var name}
  vec<pair<Type*, str> > params;
  uset<Type*> returns;
  vec<FnCall> code;

  Fn();

  static void define(const Driver::Control control, const str& name,
      const str& container, const vec<pair<Type*, str> > params,
      const uset<Type*> returns, const vec<FnCall>& code, const uset<Mod> mods);

  virtual void validate();

  Var call(const vec<FnCall>& params);
  Var run(); };

llu Fn::scope = 0;

Fn::Fn(): if_executed(false), break_loop(false), control(Language::INVALID) {}

void Fn::validate(){
  assert(control != Language::INVALID, "Fn.validate", "invalid control");
  assert(!(container == "" && (contains(keywords, VIRTUAL)
      || contains(keywords, FORCE) || contains(keywords, FINAL))),
      "Fn.validate", "invalid keyword"); }

Var Fn::call(const vec<FnCall>& params){
  validate();
  const str _fn = "Fn.call";
  ++scope;

  // Handle parameters
  vec<Var> param_values;
  for(int i = 0; i < params.size(); ++i){
    // Don't execute last for parameter (iteration)
    if(control == Language::FOR && i == params.size() - 1) break;
    // Resolve parameter values
    Var v = params[i].fn->call(params[i].params);
    assert(*v.type.name != "Void", _fn, "parameter resolved to Void");
    // Add to access and param_values
    access->add(scope, v);
    param_values.pb(v); }

  // User-defined function call
  Var r;
  if(control == Language::USER){
    r = run();

  // Variable declaration
  }else if(control == Language::DECL){
    r = run();
    access->add(scope, r);

  // If statement
  }else if(control == Language::IF){
    assert(param_values.size() == 1, _fn, "if didn't have one parameter");
    bool b = *((bool*)param_values[0].addr);
    if(b)
      run();
    r = Var("Bool", (Char)b);

  // Else statement
  }else if(control == Language::ELSE){
    assert(param_values.empty(), _fn, "else didn't have zero parameters");
    if(!if_executed)
      run();

  // For loop
  // Params: 1. Variable declaration  2. Stop condition  3. Variable iteration
  }else if(control == Language::FOR){
    assert(param_values.size() == 3, _fn, "for didn't have three parameters");
    Var v = param_values[1];
    while(*((bool*)v.addr)){
      run();
      if(break_loop) break;
      // Iterate
      params[2].fn->call(params[2].params);
      // Check stop condition
      v = params[1].fn->call(params[1].params); }

  // While loop
  }else if(control == Language::WHILE){
    assert(param_values.size() == 1, _fn, "while didn't have one parameter");
    Var v = param_values[0];
    while(*((bool*)v.addr)){
      run();
      if(break_loop) break;
      // Check stop condition
      v = params[0].fn->call(params[0].params); }

  // Break loop
  }else if(control == Language::BREAK){
    assert(param_values.empty(), _fn, "break didn't have zero parameters");
    assert(code.empty(), _fn, "break had instructions");

  // Continue loop
  }else if(control == Language::CONTINUE){
    assert(param_values.empty(), _fn, "continue didn't have zero parameters");
    assert(code.empty(), _fn, "continue had instructions");

  // Return statement
  }else if(control == Language::RETURN){
    assert(param_values.size() == 1, _fn, "return didn't have one parameter");
    assert(code.empty(), _fn, "return had instructions");
    r = param_values[0];

  // System call
  }else
    r = lang->process_fn(control, param_values);

  // Deallocate all variables at scope
  for(pair<str, Var> p : access->scope_table[scope])
    p.second.deallocate();
  // Remove from access all variables at scope
  access->remove(scope);

  --scope;
  validate();
  return r; }

Var Fn::run(){
  const str _fn = "Fn.run";
  for(int i = 0; i < code.size(); ++i){
    const FnCall& instr = code[i];
    Var v = instr.fn->call(instr.params);

    Language::Control ctrl = instr.fn->control;
    if(ctrl == Language::IF){
      assert(*v.type.name == "Bool", _fn, "if returned non-bool");
      if_executed = *((bool*)v.addr);

    }else if(ctrl == Language::BREAK){
      assert(i == code.size() - 1, _fn, "break precedes dangling code");
      assert(*v.type.name == "Void", _fn, "break returned non-Void");
      break_loop = true;
      break;

    }else if(ctrl == Language::CONTINUE){
      assert(i == code.size() - 1, _fn, "continue precedes dangling code");
      assert(*v.type.name == "Void", _fn, "continue returned non-Void");
      break;

    }else if(ctrl == Language::RETURN){
      assert(i == code.size() - 1, _fn, "return precedes dangling code");
      assert(control == Language::USER, _fn,
          "returning from non-user-defined function");
      return v; }

    //! Add declaration to access ???
  }
  return Var(); }

#endif
