// FUNCTION

//! Returns destruct

#ifndef FN_HH
#define FN_HH

#include "driver.hh"

// Function
struct Fn {

  // Function call
  struct FnCall {
    Fn* fn;
    vec<FnCall> params;

    virtual void validate(); };

  // Used to destruct all variables at scope when leaving a function
  static llu scope;
  // Language dependencies
  static Driver driver;
  // Container for all functions
  static umap<str, Fn> registry;

  // False if only declared
  bool _defined;
  // Used for control flow
  bool if_executed, break_loop;
  // Function type
  Driver::Control control;
  // Containing class
  Type* container;
  // Function name
  str name;
  // Keyword modifiers
  uset<Mod> mods;
  // Parameters
  vec<Type::_Var> params;
  // Possible return types
  uset<Type*> returns;
  // Each line of operational code
  vec<FnCall> code;

  Fn();

  virtual void validate();

  static Fn* search(const str& name, Type* container);
  static bool declared(const str& name, Type* container);
  static bool defined(const str& name, Type* container);
  static Fn* declare(const str& name, Type* container);
  static Fn* define(const Driver::Control control, const str& name,
      Type* container, const vec<Type::_Var> params, const uset<Type*> returns,
      const vec<FnCall>& code, const uset<Mod> mods);

  Var call(const vec<FnCall>& params);
  Var run(); };

llu Fn::scope = 0;
Driver Fn::driver;
umap<str, Fn> Fn::registry;

// Set default member state
Fn::Fn(): _defined(false), if_executed(false), break_loop(false),
    control(Driver::INVALID) {}

// Ensure valid state, O(N)
// To be called when definition is expected
void Fn::validate(){
  const str& _fn = "Fn.validate";
  assert(_defined, _fn, "function not defined");
  assert(control != Driver::INVALID, _fn, "invalid control");
  assert(name != "", _fn, "function has no name");
  for(const Type::_Var& var : params)
    assert(var.type != NULL && var.type->_defined,
        _fn, "param type not defined");
  for(Type* type : returns)
    assert(type != NULL && type->_defined, _fn, "return type not defined");
  for(FnCall& fc : code)
    fc.validate(); }

// Ensure valid state, O(N) recursive
void Fn::FnCall::validate(){
  assert(fn != NULL && fn->_defined,
      "FnCall.validate", "function is not defined");
  for(FnCall& fc : params)
    fc.validate(); }

// Ensure valid state, O(N)
// To be called when full definition is expected
void Type::validate(){
  const str& _fn = "Type.validate";
  assert(_defined, _fn, "type not defined");
  assert(name != "", _fn, "type has no name");
  for(const pair<str, _Var>& p : vars)
    assert(p.second.type != NULL && p.second.type->_defined,
        _fn, "member variable type not defined");
  for(Fn* fn : fns)
    assert(fn != NULL && fn->_defined, _fn, "member function not defined");
  for(Type* type : parents)
    assert(type != NULL && type->_defined, _fn, "base type not defined"); }

// Searches all containing classes
// Returns NULL if not found
Fn* Fn::search(const str& name, Type* container){
  if(contains(registry, name)) return &registry[name];
  if(container == NULL) return NULL;
  vec<str> ctrs =
      split(Type::full_name(container->name, container->container), "::");
  str ctr = "";
  for(int i = 0; i < ctrs.size(); ++i){
    ctr += ctrs[i] + "::";
    if(contains(registry, ctr + name)) return &registry[ctr + name]; }
  return NULL; }

// Has the function been declared
bool Fn::declared(const str& name, Type* container){
  return search(name, container) != NULL; }

// Has the function been defined
bool Fn::defined(const str& name, Type* container){
  Fn* fn = search(name, container);
  return fn != NULL && fn->_defined; }

// Declare the function
Fn* Fn::declare(const str& name, Type* container){
  assert(!declared(name, container), "Fn.declare", "Function already declared");
  Fn fn;
  fn._defined = false;
  fn.name = name;
  fn.container = container;
  return &(registry[Type::full_name(name, container)] = fn); }

// Define the function
Fn* Fn::define(const Driver::Control control, const str& name,
    Type* container, const vec<Type::_Var> params, const uset<Type*> returns,
    const vec<FnCall>& code, const uset<Mod> mods){
  const str& _fn = "Fn.define";
  assert(!defined(name, container), _fn, "function already defined");
  Fn* fn;
  if(declared(name, container))
    fn = search(name, container);
  else
    fn = &(registry[Type::full_name(name, container)] = Fn());
  fn->_defined = true;
  fn->control = control;
  fn->name = name;
  fn->container = container;
  for(const Type::_Var& var : params){
    assert(var.type != NULL && var.type->_defined,
        _fn, "param type not defined");
    fn->params.pb(var); }
  for(Type* type : returns){
    assert(type != NULL && type->_defined, _fn, "return type not defined");
    fn->returns.insert(type); }
  for(const FnCall& fc : code){
    assert(fc.fn != NULL && fc.fn->_defined, _fn, "function not defined");
    fn->code.pb(fc); }
  return fn; }

// Execute the function
Var Fn::call(const vec<FnCall>& params){
  const str _fn = "Fn.call";
  ++scope;

  // Handle parameters
  vec<Var> param_values;
  for(int i = 0; i < params.size(); ++i){
    // Don't execute last for parameter (iteration)
    if(control == Driver::FOR && i == params.size() - 1) break;
    // Resolve parameter values
    Var v = params[i].fn->call(params[i].params);
    assert(v.type != NULL && v.type != &Type::registry["Void"],
        _fn, "parameter resolved to Void");
    // Add to access and param_values
    Var::declare(scope, v);
    param_values.pb(v); }

  // User-defined function call
  Var r;
  if(control == Driver::USER){
    r = run();

  // Variable declaration
  }else if(control == Driver::DECL){
    r = run();
    Var::declare(scope, r);

  // If statement
  }else if(control == Driver::IF){
    assert(param_values.size() == 1, _fn, "if didn't have one parameter");
    bool b = *((bool*)param_values[0].addr);
    if(b)
      run();
    r = Var("Bool", (void*)&b, sizeof(bool));

  // Else statement
  }else if(control == Driver::ELSE){
    assert(param_values.empty(), _fn, "else didn't have zero parameters");
    if(!if_executed)
      run();

  // For loop
  // Params: 1. Variable declaration  2. Stop condition  3. Variable iteration
  }else if(control == Driver::FOR){
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
  }else if(control == Driver::WHILE){
    assert(param_values.size() == 1, _fn, "while didn't have one parameter");
    Var v = param_values[0];
    while(*((bool*)v.addr)){
      run();
      if(break_loop) break;
      // Check stop condition
      v = params[0].fn->call(params[0].params); }

  // Break loop
  }else if(control == Driver::BREAK){
    assert(param_values.empty(), _fn, "break didn't have zero parameters");
    assert(code.empty(), _fn, "break had instructions");

  // Continue loop
  }else if(control == Driver::CONTINUE){
    assert(param_values.empty(), _fn, "continue didn't have zero parameters");
    assert(code.empty(), _fn, "continue had instructions");

  // Return statement
  }else if(control == Driver::RETURN){
    assert(param_values.size() == 1, _fn, "return didn't have one parameter");
    assert(code.empty(), _fn, "return had instructions");
    r = param_values[0];

  // System call
  }else
    r = driver.process_fn(control, param_values);

  // Deallocate all variables at scope
  for(Var* var : Var::scope_table[scope])
    var->destruct();
  // Remove from access all variables at scope
  Var::descope(scope);

  --scope;
  return r; }

// Execute the code within the function
Var Fn::run(){
  const str _fn = "Fn.run";
  for(int i = 0; i < code.size(); ++i){
    const FnCall& instr = code[i];
    Var v = instr.fn->call(instr.params);

    Driver::Control ctrl = instr.fn->control;
    // If statement
    if(ctrl == Driver::IF){
      assert(v.type->name == "Bool", _fn, "if returned non-bool");
      if_executed = *((bool*)v.addr);

    // Break loop
    }else if(ctrl == Driver::BREAK){
      assert(i == code.size() - 1, _fn, "break precedes dangling code");
      assert(v.type->name == "Void", _fn, "break returned non-Void");
      break_loop = true;
      break;

    // Continue loop
    }else if(ctrl == Driver::CONTINUE){
      assert(i == code.size() - 1, _fn, "continue precedes dangling code");
      assert(v.type->name == "Void", _fn, "continue returned non-Void");
      break;

    // Return from function
    }else if(ctrl == Driver::RETURN){
      assert(i == code.size() - 1, _fn, "return precedes dangling code");
      assert(control == Driver::USER, _fn,
          "returning from non-user-defined function");
      return v; }

    //! Add declaration to access ???
  }
  return Var(); }

#endif
