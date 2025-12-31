// COMPILER

//! store keyword mods

#ifndef COMPILER_HH
#define COMPILER_HH

#include "util.hh"
#include "file.hh"
#include "fn.hh"

const str
    MAIN_FILE = "___main";

struct Compiler {

  Fn* compile(const umap<str, File>& files);
  void process_file(const File& file);
  void process_obj_decl(const vec<str>& code, const int line,
      const str& container, const umap<str, bool>& keywords);
  pair<str, str> process_member_decl(const str& decl, const str& container,
      const umap<str, bool>& keywords);
  str process_fn_decl(const vec<str>& code, const int line,
      const str& container, const umap<str, bool>& keywords);
  void process_fn_call(const str& call, const umap<str, bool>& keywords); };

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
  return &(Fn::registry->fns)[MAIN_FILE]; }

void Compiler::process_file(const File& file){
  // Process included files first
  for(File* incl : file.includes)
    process_file(*incl);

  // Process all code in file
  const str fn = "Compiler.process_file";
  for(int line = 0; line < file.code.size(); ++line){
    str s = strip(file.code[line]);
    umap<str, bool> keywords =
        {{"abstract", false}, {"template", false}, {"const", false}};
    str tok;
    while(1){

      // Obj declaration
      if(starts_with(s, "obj")){
        assert(!keywords["const"], fn, "invalid keyword");
        process_obj_decl(file.code, line, "", keywords);
        break;

      // Fn declaration
      }else if(starts_with(s, "fn")){
        assert(!keywords["abstract"] && !keywords["template"],
            fn, "invalid keyword");
        process_fn_decl(file.code, line, "", keywords);
        break;

      // Keyword modifier
      }else if(keywords.find(tok = next_tok(s)) != keywords.end()){
        assert(!keywords[tok], fn, "keyword declared twice");
        s = strip(delete_tok(s));
        keywords[tok] = true;

      // Function call
      }else if(s != "" && is_upper(s[0]) || is_lower(s[0]) || s[0] == '_'){
        assert(!keywords["abstract"] && !keywords["template"],
            fn, "invalid keyword");
        assert(file.main, fn, "function call outside main file or function");
        process_fn_call(s, keywords);
        break;

      // Otherwise error
      }else
        err(fn, "syntax error"); } } }

void Compiler::process_obj_decl(const vec<str>& code, const int line,
    const str& container, const umap<str, bool>& keywords){
  const str fn = "Compiler.process_obj_def";
  // Get type name
  str s = code[line];
  while(next_tok(s) != "obj")
    s = delete_tok(s);
  s = delete_tok(s);
  str type = next_tok(s);
  assert(is_type(type), fn, "expected type name");
  s = strip(delete_tok(s));

  // Forward declaration, no definition
  if(s == ""){
    assert(!Type::registry->declared(type, container),
        fn, "Type already declared");
    Type::registry->declare(type, container);
    return; }

  // Definition expected
  assert(!Type::registry->defined(type, container), fn, "Type already defined");
  str tok = next_tok(s);
  assert(tok == ":", fn, "expected colon after type");
  s = strip(delete_tok(s));

  // Get any inherited base types
  vec<str> bases;
  if(s != ""){
    assert(ends_with(s, ":"), fn, "expected colon after types");
    vec<str> types = split(s, ",");
    for(const str& t : types){
      assert(is_type(strip(t)), fn, "expected type name");
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
  vec<pair<str, str> > vars;
  vec<str> fns;
  for(int i = line; i < end; ++i){
    str s = strip(code[i]);
    umap<str, bool> _keywords =
        {{"abstract", false}, {"template", false}, {"const", false},
        {"static", false}, {"virtual", false}, {"force", false},
        {"final", false}};
    while(1){

      // Obj declaration
      if(starts_with(s, "obj")){
        assert(!_keywords["const"] && !_keywords["virtual"]
            && !_keywords["force"] && !_keywords["final"]
            && !_keywords["static"], fn, "invalid keyword");
        process_obj_decl(code, i, container + "::" + type, _keywords);
        break;

      // Fn, Constructor, Operator declaration
      }else if(starts_with(s, "fn") || starts_with(s, type)
          || starts_with(s, "operator")){
        assert(!_keywords["abstract"] && !_keywords["template"]
            && !_keywords["static"], fn, "invalid keyword");
        fns.pb(process_fn_decl(code, i, container, _keywords));
        break;

      // Keyword modifier
      }else if(_keywords.find(tok = next_tok(s)) != _keywords.end()){
        assert(!_keywords[tok], fn, "keyword declared twice");
        s = strip(delete_tok(s));
        _keywords[tok] = true;

      // Member declaration
      }else if(s != "" && (is_upper(s[0]) || s[0] == '_')){
        assert(!_keywords["abstract"] && !_keywords["template"]
            && !_keywords["virtual"] && !_keywords["force"]
            && !_keywords["final"], fn, "invalid keyword");
        vars.pb(process_member_decl(s, container + "::" + type, _keywords));
        break;

      // Otherwise error
      }else
        err(fn, "syntax error"); } }

  // Log definition in TypeMgr
  Type::registry->define(type, container, bases, vars, fns); }

// Returns {full type name, var name}
pair<str, str> Compiler::process_member_decl(const str& decl,
    const str& container, const umap<str, bool>& keywords){
  const str fn = "Compiler.process_member_decl";
  assert(!(keywords.at("const") && keywords.at("static")),
      fn, "const member variables are already static");
  str s = decl;
  str type = next_tok(s);
  assert(is_type(type), fn, "expected type");
  s = strip(delete_tok(s));
  str name = next_tok(s);
  assert(keywords.at("const") ? is_const_name(name) : is_mutable_name(name),
      fn, "expected variable name");
  str full_type = Type::registry->search(type, container);
  assert(full_type != "", fn, "type not declared");
  return pair<str, str>(full_type, name); }

str Compiler::process_fn_decl(const vec<str>& code, const int line,
    const str& container, const umap<str, bool>& keywords){

  //! log fn def in fnmgr
  return ""; }

// Includes variable declarations (constructor call is a fn call)
void Compiler::process_fn_call(const str& call,
    const umap<str, bool>& keywords){
  //! assert objs and fns have been defined and declared
}

#endif
