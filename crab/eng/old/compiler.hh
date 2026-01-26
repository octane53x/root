// COMPILER

#ifndef COMPILER_HH
#define COMPILER_HH

#include "util.hh"
#include "file.hh"
#include "fn.hh"

// Function name of fn containing main file code
const str
    MAIN_FN = "___main";

// Stores type and function declarations and interprets function calls
struct Compiler {

  virtual void validate();

  void init();
  Fn* compile(const umap<str, File>& files);
  void process_file(const File& file);
  void process_obj_decl(const vec<str>& code, const int line, Type* container,
      const uset<Mod>& mods);
  Type::_Var process_member_decl(const str& decl, Type* container,
      const uset<Mod>& mods);
  Fn* process_fn_decl(const vec<str>& code, const int line,
      Type* container, const uset<Mod>& mods);
  Fn::FnCall process_fn_call(const str& call, const uset<Mod>& mods); };

// Ensure valid state, O(N)
// Called between compilation and execution
void Compiler::validate(){
  umap<str, Type>::iterator it;
  for(it = Type::registry.begin(); it != Type::registry.end(); ++it)
    it->second.validate();
  umap<str, Fn>::iterator it2;
  for(it2 = Fn::registry.begin(); it2 != Fn::registry.end(); ++it2)
    it2->second.validate(); }

// Initialize compiler
void Compiler::init(){
  // Declare primitives
  Type::declare("Void", NULL);
  Type::declare("Char", NULL);
  Type::declare("Block", NULL); }

// Calls recursive process_file
// Returns pointer to main function
Fn* Compiler::compile(const umap<str, File>& files){
  str main_file = "";
  for(const pair<str, File>& p : files)
    if(p.second.main){
      main_file = p.first;
      break; }
  assert(main_file != "", "Compiler.compile",
      "main file not marked in includer");
  process_file(files.at(main_file));
  validate();
  return &Fn::registry[MAIN_FN]; }

// Depth-first recursively process each file
void Compiler::process_file(const File& file){
  const str _fn = "Compiler.process_file";
  // Process included files first
  for(File* incl : file.includes)
    process_file(*incl);

  // Process all code in file
  vec<Fn::FnCall> code;
  for(int line = 0; line < file.code.size(); ++line){
    str s = strip(file.code[line]);
    str tok = next_tok(s);
    uset<Mod> mods;
    while(1){

      // Empty line
      if(s == "" && mods.empty())
        break;

      // Obj declaration
      if(tok == "obj"){
        process_obj_decl(file.code, line, NULL, mods);
        break;

      // Fn declaration
      }else if(tok == "fn"){
        process_fn_decl(file.code, line, NULL, mods);
        break;

      // Keyword modifier
      }else if(contains(TOK_TO_MOD, tok)){
        Mod mod = TOK_TO_MOD.at(tok);
        assert(!contains(mods, mod), _fn, "keyword declared twice");
        mods.insert(mod);
        s = strip(delete_tok(s));
        tok = next_tok(s);

      // Function call, includes variable declaration
      }else if(s != "" && (is_alpha(s[0]) || s[0] == '_')){
        assert(file.main, _fn, "function call outside main file or function");
        code.pb(process_fn_call(s, mods));
        break;

      // Otherwise error
      }else
        err(_fn, "syntax error"); } }

  // Add main function
  if(file.main)
    Fn::define(Driver::USER, MAIN_FN, NULL, {}, {}, code, {}); }

// Object declaration or definition, recursive for nested objects
void Compiler::process_obj_decl(const vec<str>& code, const int line,
    Type* container, const uset<Mod>& mods){
  const str _fn = "Compiler.process_obj_decl";
  // Get type name
  str s = code[line];
  while(next_tok(s) != "obj")
    s = delete_tok(s);
  s = delete_tok(s);
  str type = next_tok(s);
  assert(is_type(type), _fn, "expected type name");
  s = strip(delete_tok(s));

  // Forward declaration, no definition
  if(s == ""){
    assert(!Type::declared(type, container), _fn, "Type already declared");
    Type::declare(type, container);
    return; }
  Type* location = Type::declare(type, container);

  // Definition expected
  assert(!Type::defined(type, container), _fn, "Type already defined");
  str tok = next_tok(s);
  assert(tok == ":", _fn, "expected colon after type");
  s = strip(delete_tok(s));

  // Get any inherited base types
  vec<Type*> bases;
  if(s != ""){
    assert(ends_with(s, ":"), _fn, "expected colon after types");
    vec<str> types = split(s, ",");
    for(const str& t : types){
      assert(is_type(strip(t)), _fn, "expected type name");
      Type* type = Type::search(t, container);
      assert(type != NULL && type->_defined, _fn, "base type not defined");
      bases.pb(type); } }

  // Find the end of the definition
  s = code[line];
  int spaces = 0;
  while(spaces < s.size() && s[spaces] == ' ')
    ++spaces;
  int end = line + 1;
  while(1){
    if(code[end] == ""){
      ++end;
      continue; }
    int spaces2 = 0;
    while(spaces2 < code[end].size() && code[end][spaces2] == ' ')
      ++spaces2;
    if(spaces2 <= spaces) break;
    ++end; }

  // Process internal declarations
  vec<Type::_Var> vars;
  vec<Fn*> fns;
  for(int i = line; i < end; ++i){
    str s = strip(code[i]);
    str tok = next_tok(s);
    uset<Mod> _mods;
    while(1){

      // Empty line
      if(s == "" && mods.empty())
        break;

      // Obj declaration
      if(tok == "obj"){
        process_obj_decl(code, i, location, _mods);
        break;

      // Fn, Constructor, Operator declaration
      }else if(tok == "fn" || tok == "operator" || tok == type){
        fns.pb(process_fn_decl(code, i, location, _mods));
        break;

      // Keyword modifier
      }else if(contains(TOK_TO_MOD, tok)){
        Mod mod = TOK_TO_MOD.at(tok);
        assert(!contains(_mods, mod), _fn, "keyword declared twice");
        _mods.insert(mod);
        s = strip(delete_tok(s));
        tok = next_tok(s);

      // Member declaration
      }else if(s != "" && (is_upper(s[0]) || s[0] == '_')){
        vars.pb(process_member_decl(s, location, _mods));
        break;

      // Otherwise error
      }else
        err(_fn, "syntax error"); } }

  // Log definition in TypeMgr
  Type::define(type, container, bases, vars, fns); }

// Object member variable
Type::_Var Compiler::process_member_decl(const str& decl, Type* container,
    const uset<Mod>& mods){
  const str _fn = "Compiler.process_member_decl";
  str s = decl;
  str type = next_tok(s);
  assert(is_type(type), _fn, "expected type");
  s = strip(delete_tok(s));
  str name = next_tok(s);
  assert(contains(mods, CONST) ? is_const_name(name)
      : is_mutable_or_fn_name(name), _fn, "expected variable name");
  Type* t = Type::search(type, container);
  assert(t != NULL, _fn, "type not declared");
  Type::_Var var;
  var.type = t;
  var.name = name;
  return var; }

// Function declaration or definition
Fn* Compiler::process_fn_decl(const vec<str>& code, const int line,
    Type* container, const uset<Mod>& mods){
  const str _fn = "Compiler.process_fn_decl";
  // Get function name
  str s = code[line];
  str name = next_tok(s);
  while(contains(mods, TOK_TO_MOD.at(name)) || name == "fn"){
    s = strip(delete_tok(s));
    name = next_tok(s); }
  assert(is_mutable_or_fn_name(name), _fn, "expected function name");

  // Parse parameters
  s = strip(delete_tok(s));
  str tok = next_tok(s);
  assert(tok == "(", _fn, "expected parenthesis");
  vec<Type::_Var> params;
  while(1){
    s = strip(delete_tok(s));
    tok = next_tok(s);
    if(tok == ")") break;
    assert(s != "", _fn, "missing parenthesis");
    assert(is_type(tok), _fn, "expected type");
    Type* type = Type::search(tok, container);
    s = strip(delete_tok(s));
    tok = next_tok(s);
    assert(is_name(tok), _fn, "expected variable name");
    Type::_Var var;
    var.type = type;
    var.name = tok;
    params.pb(var);
    s = strip(delete_tok(s));
    tok = next_tok(s);
    if(tok == ")") break;
    assert(tok == ",", _fn, "expected parenthesis or comma"); }

  // // Declaration, no definition
  // s = strip(delete_tok(s));
  // if(s == ""){
  //   assert(!Fn::registry->declared(//!type, container),
  //       _fn, "Type already declared");
  //   Type::registry->declare(type, container);
  //   return; }

  // //! copied
  // // Definition expected
  // assert(!Type::registry->defined(type, container), _fn, "Type already defined");
  // str tok = next_tok(s);
  // assert(tok == ":", _fn, "expected colon after type");
  // s = strip(delete_tok(s));

  // //! copied
  // // Find the end of the definition
  // s = code[line];
  // int spaces = 0;
  // while(spaces < s.size() && s[spaces] == ' ')
  //   ++spaces;
  // int end = line + 1;
  // while(1){
  //   if(code[end] == ""){
  //     ++end;
  //     continue; }
  //   int spaces2 = 0;
  //   while(spaces2 < code[end].size() && code[end][spaces2] == ' ')
  //     ++spaces2;
  //   if(spaces2 <= spaces) break;
  //   ++end; }

  //! log fn def in fnmgr
  return NULL; }

// Includes variable declarations (constructor call is a fn call)
Fn::FnCall Compiler::process_fn_call(const str& call, const uset<Mod>& mods){
  //! assert objs and fns have been defined and declared
  Fn::FnCall fc; //!
  return fc;
}

#endif
