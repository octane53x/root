// COMPILER

#ifndef COMPILER_HH
#define COMPILER_HH

#include "util.hh"
#include "file.hh"
#include "fn.hh"

const str
    MAIN_FILE = "___main";

struct Compiler {

  Fn* compile(const umap<str, File>& files, umap<str, Fn>* fns);
  void process_file(const File& file, umap<str, Fn>* fns);
  void process_obj_decl(const vec<str>& code, const int line,
      const bool _abstract, const bool _template);
  void process_member_decl(const str& decl);
  void process_fn_decl(const vec<str>& code, const int line, const bool _const,
      const bool _virtual, const bool _force, const bool _final);
  void process_fn_call(const str& call, const bool _const); };

// Returns pointer to main function
Fn* Compiler::compile(const umap<str, File>& files, umap<str, Fn>* fns){
  str main_file = "";
  for(pair<str, File> p : files)
    if(p.second.main){
      main_file = p.first;
      break; }
  assert(main_file != "", "Compiler.compile",
      "main file not marked in includer");
  process_file(files.at(main_file), fns);
  return &(*fns)[MAIN_FILE]; }

void Compiler::process_file(const File& file, umap<str, Fn>* fns){
  // Process included files first
  for(File* incl : file.includes)
    process_file(*incl, fns);

  // Process all code in file
  const str fn = "Compiler.process_file";
  for(int line = 0; line < file.code.size(); ++line){
    str s = strip(file.code[line]);
    bool _abstract, _template, _const;
    _abstract = _template = _const = false;
    while(1){

      // Obj declaration
      if(starts_with(s, "obj")){
        assert(!_const, fn, "invalid keyword");
        process_obj_decl(file.code, line, _abstract, _template);
        break;

      // Fn declaration
      }else if(starts_with(s, "fn")){
        assert(!_abstract && !_template, fn, "invalid keyword");
        process_fn_decl(file.code, line, _const, false, false, false);
        break;

      // Abstract keyword
      }else if(starts_with(s, "abstract")){
        assert(!_abstract, fn, "keyword declared twice");
        s = strip(delete_tok(s));
        _abstract = true;
        continue;

      // Template keyword
      }else if(starts_with(s, "template")){
        assert(!_template, fn, "keyword declared twice");
        s = strip(delete_tok(s));
        _template = true;
        continue;

      // Const keyword
      }else if(starts_with(s, "const")){
        assert(!_const, fn, "keyword declared twice");
        s = strip(delete_tok(s));
        _const = true;
        continue;

      // Function call
      }else if(s != "" && is_upper(s[0]) || is_lower(s[0]) || s[0] == '_'){
        assert(!_template, fn, "invalid keyword");
        process_fn_call(s, _const);
        break;

      // Otherwise error
      }else
        err(fn, "syntax error"); } } }

void Compiler::process_obj_decl(const vec<str>& code, const int line,
    const bool _abstract, const bool _template){
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
    assert(!Type::registry->declared(type), fn, "Type already declared");
    Type::registry->declare(type);
    return; }

  // Definition expected
  assert(!Type::registry->defined(type), fn, "Type already defined");
  str sym = next_tok(s);
  assert(sym == ":", fn, "expected colon after type");
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
  for(int i = line; i < end; ++i){
    str s = strip(code[i]);
    bool _abstract2, _template2, _const, _virtual, _force, _final;
    _abstract2 = _template2 = _const = _virtual = _force = _final = false;
    while(1){

      // Obj declaration
      if(starts_with(s, "obj")){
        assert(!_const && !_virtual && !_force && !_final,
            fn, "invalid keyword");
        process_obj_decl(code, i, _abstract2, _template2);
        break;

      // Fn, Constructor, Operator declaration
      }else if(starts_with(s, "fn") || starts_with(s, type)
          || starts_with(s, "operator")){
        assert(!_abstract2 && !_template2, fn, "invalid keyword");
        process_fn_decl(code, i, _const, _virtual, _force, _final);
        break;

      // Abstract keyword
      }else if(starts_with(s, "abstract")){
        assert(!_abstract2, fn, "keyword declared twice");
        s = strip(delete_tok(s));
        _abstract2 = true;
        continue;

      // Template keyword
      }else if(starts_with(s, "template")){
        assert(!_template2, fn, "keyword declared twice");
        s = strip(delete_tok(s));
        _template2 = true;
        continue;

      // Virtual keyword
      }else if(starts_with(s, "virtual")){
        assert(!_virtual, fn, "keyword declared twice");
        s = strip(delete_tok(s));
        _virtual = true;
        continue;

      // Force keyword
      }else if(starts_with(s, "force")){
        assert(!_force, fn, "keyword declared twice");
        s = strip(delete_tok(s));
        _force = true;
        continue;

      // Final keyword
      }else if(starts_with(s, "final")){
        assert(!_final, fn, "keyword declared twice");
        s = strip(delete_tok(s));
        _final = true;
        continue;

      // Member declaration
      }else if(s != "" && (is_upper(s[0]) || s[0] == '_')){
        assert(!_abstract2 && !_template2 && !_const && !_virtual && !_force
            && !_final, fn, "invalid keyword");
        process_member_decl(s);
        break;

      // Otherwise error
      }else
        err(fn, "syntax error"); } }

  //! log def in typemgr
}

void Compiler::process_member_decl(const str& decl){

}

void Compiler::process_fn_decl(const vec<str>& code, const int line,
    const bool _const, const bool _virtual, const bool _force,
    const bool _final){

  //! log fn def in fnmgr
}

void Compiler::process_fn_call(const str& call, const bool _const){
  //! assert objs and fns have been defined and declared
}

#endif
