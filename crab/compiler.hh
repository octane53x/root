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

  void init();
  Fn* compile(const umap<str, File>& files);
  void process_file(const File& file);
  void process_obj_decl(const vec<str>& code, const int line,
      const str& container, const uset<Mod>& mods);
  VarDecl process_member_decl(const str& decl, const str& container,
      const uset<Mod>& mods);
  str process_fn_decl(const vec<str>& code, const int line,
      const str& container, const uset<Mod>& mods);
  Fn::FnCall process_fn_call(const str& call, const uset<Mod>& mods); };

// Initialize compiler
void Compiler::init(){
  // Declare primitives
  Type::declare("Void", "");
  Type::declare("Char", "");
  Type::declare("Block", ""); }

// Calls recursive process_file
// Returns pointer to main function
Fn* Compiler::compile(const umap<str, File>& files){
  str main_file = "";
  for(pair<str, File> p : files)
    if(p.second.main){
      main_file = p.first;
      break; }
  assert(main_file != "", "Compiler.compile",
      "main file not marked in includer");
  process_file(files.at(main_file));
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
    uset<Keyword> mods;
    while(1){

      // Empty line
      if(s == "" && mods.empty())
        break;

      // Obj declaration
      if(tok == "obj"){
        process_obj_decl(file.code, line, "", mods);
        break;

      // Fn declaration
      }else if(tok == "fn"){
        process_fn_decl(file.code, line, "", mods);
        break;

      // Keyword modifier
      }else if(contains(TOK_TO_MOD, tok)){
        Mod mod = TOK_TO_MOD[tok];
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
        err(fn, "syntax error"); } }

  // Add main function
  if(file.main)
    Fn::define(Driver::USER, MAIN_FN, "", {}, {}, code, {}); }

void Compiler::process_obj_decl(const vec<str>& code, const int line,
    const str& container, const uset<Mod>& mods){
  const str fn = "Compiler.process_obj_def";
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

  // Definition expected
  assert(!Type::defined(type, container), _fn, "Type already defined");
  str tok = next_tok(s);
  assert(tok == ":", _fn, "expected colon after type");
  s = strip(delete_tok(s));

  // Get any inherited base types
  vec<str> bases;
  if(s != ""){
    assert(ends_with(s, ":"), _fn, "expected colon after types");
    vec<str> types = split(s, ",");
    for(const str& t : types){
      assert(is_type(strip(t)), _fn, "expected type name");
      bases.pb(t); } }

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
  vec<pair<str, Type*> > vars;
  vec<str> fns;
  for(int i = line; i < end; ++i){
    str s = strip(code[i]);
    str tok = next_tok(s);
    str location = (container == "") ? type : container + "::" + type;
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
        Mod mod = TOK_TO_MOD[tok]
        assert(!contains(_mods, mod, _fn, "keyword declared twice");
        _mods.insert(mod);
        s = strip(delete_tok(s));
        tok = next_tok(s);

      // Member declaration
      }else if(s != "" && (is_upper(s[0]) || s[0] == '_')){
        vars.pb(process_member_decl(s, location, _mods));
        break;

      // Otherwise error
      }else
        err(fn, "syntax error"); } }

  // Log definition in TypeMgr
  Type::define(type, container, bases, vars, fns); }

VarDecl Compiler::process_member_decl(const str& decl,
    const str& container, const uset<Mod>& mods){
  const str fn = "Compiler.process_member_decl";
  assert(!(contains(mods, CONST) && contains(mods, STATIC)),
      _fn, "const member variables are already static");
  str s = decl;
  str type = next_tok(s);
  assert(is_type(type), _fn, "expected type");
  s = strip(delete_tok(s));
  str name = next_tok(s);
  assert(contains(mods, CONST) ? is_const_name(name)
      : is_mutable_or_fn_name(name), _fn, "expected variable name");
  Type* t = Type::search(type, container);
  assert(t != NULL, _fn, "type not declared");
  return VarDecl(t, name); }

str Compiler::process_fn_decl(const vec<str>& code, const int line,
    const str& container, const umap<Mod>& mods){
  const str _fn = "Compiler.process_fn_decl";
  // Get function name
  str s = code[line];
  str name = next_tok(s);
  while(contains(mods, TOK_TO_MOD[name]) || name == "fn"){
    s = strip(delete_tok(s));
    name = next_tok(s); }
  assert(is_mutable_or_fn_name(name), _fn, "expected function name");

  // Parse parameters
  s = strip(delete_tok(s));
  str tok = next_tok(s);
  assert(tok == "(", _fn, "expected parenthesis");
  VarDecl params;
  while(1){
    s = strip(delete_tok(s));
    tok = next_tok(s);
    if(tok == ")") break;
    assert(s != "", _fn, "missing parenthesis");
    assert(is_type(tok), _fn, "expected type");
    Type* type = Type::search(type, container);
    s = strip(delete_tok(s));
    tok = next_tok(s);
    assert(is_name(tok), _fn, "expected variable name");
    params.pb(VarDecl(type, tok));
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
  return ""; }

// Includes variable declarations (constructor call is a fn call)
void Compiler::process_fn_call(const str& call,
    const uset<Mod>& mods){
  //! assert objs and fns have been defined and declared
}

#endif
