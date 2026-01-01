// FUNCTION

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
  bool defined;
  // Used for control flow
  bool if_executed, break_loop;
  // Function type
  Driver::Control control;
  // Function name and containing classes
  str name, container;
  // Keyword modifiers
  uset<Mod> keywords;
  // Parameters: {Type, var name}
  vec<VarDecl> params;
  // Possible return types
  uset<Type*> returns;
  // Each line of operational code
  vec<FnCall> code;

  Fn();

  virtual void validate();

  static Fn* search(const str& name, const str& container) const;
  static void declared(const str& name, const str& container) const;
  static void defined(const str& name, const str& container) const;

  static void declare(const str& name, const str& container);
  static void define(const Driver::Control control, const str& name,
      const str& container, const vec<VarDecl> params,
      const uset<Type*> returns, const vec<FnCall>& code, const uset<Mod> mods);

  Var call(const vec<FnCall>& params);
  Var run(); };

// Scope starts at 0, becomes 1 in main function, etc
llu Fn::scope = 0;

// Set default member state
Fn::Fn(): defined(false), if_executed(false), break_loop(false),
    control(Language::INVALID) {}

// Ensure valid state, O(N)
// To be called when definition is expected
void Fn::validate(){
  const str& _fn = "Fn.validate";
  assert(defined, _fn, "function not defined");
  assert(control != Language::INVALID, _fn, "invalid control");
  assert(name != "", _fn, "function has no name");
  for(const VarDecl& vd : params)
    assert(vd.first != NULL && vd->defined, _fn, "param type not defined");
  for(Type* type : returns)
    assert(type != NULL && type->defined, _fn, "return type not defined");
  for(const FnCall& fc : code)
    fc.validate(); }

// Ensure valid state, O(N) recursive
void Fn::FnCall::validate(){
  assert(fn != NULL && fn->defined,
      "FnCall.validate", "function is not defined");
  for(const FnCall& fc : params)
    fc.validate(); }

// Searches all containing classes
// Returns NULL if not found
Fn* Type::search(const str& name, const str& container) const {
  if(contains(registry, name)) return &registry[name];
  vec<str> ctrs = split(container, "::");
  str ctr = "";
  for(int i = 0; i < ctrs.size(); ++i){
    ctr += ctrs[i] + "::";
    if(contains(registry, ctr + type)) return &registry[ctr + type]; }
  return NULL; }

void Fn::declared(const str& name, const str& container) const {
  return search(name, container) != NULL; }

void Fn::defined(const str& name, const str& container) const {
  Fn* fn = search(name, container);
  return fn != NULL && fn->defined; }

void Fn::declare(const str& name, const str& container){
  assert(!declared(name, container), "Fn.declare", "Function already declared");
  Fn fn;
  fn.defined = false;
  fn.name = name;
  fn.container = container;
  registry[container + "::" + name] = fn; }

void Fn::define(const Driver::Control control, const str& name,
    const str& container, const vec<VarDecl> params, const uset<Type*> returns,
    const vec<FnCall>& code, const uset<Mod> mods){
  const str& _fn = "Fn.define";
  assert(!defined(name, container), _fn, "function already defined");
  Fn fn;
  fn.defined = true;
  fn.control = control;
  fn.name = name;
  fn.container = container;
  for(const VarDecl& vd : params){
    assert(vd.first != NULL && vd.first->defined,
        _fn, "param type not defined");
    fn.params.pb(vd); }
  for(Type* type : returns){
    assert(type != NULL && type->defined, _fn, "return type not defined");
    fn.returns.insert(type); }
  for(const FnCall& fc : code){
    assert(fc.fn != NULL && fc.fn->defined, _fn, "function not defined");
    fn.code.pb(fc); }
  registry[container + "::" + name] = fn; }

// Execute the function
Var Fn::call(const vec<FnCall>& params){
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
