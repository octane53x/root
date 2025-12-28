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
  void process_obj_def(const vec<str>& code, const int line,
      const bool _template);
  void process_fn_def(const vec<str>& code, const int line, const bool _const,
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
  const str fn = "Compiler.process_file";
  for(int i = 0; i < file.code.size(); ++i){
    str line = strip(file.code[i]);
    bool _template, _const, _virtual, _force, _final;
    _template = _const = _virtual = _force = _final = false;
    while(1){

      // Obj definition
      if(starts_with(line, "obj")){
        assert(!_const, fn, "const is for functions or declarations");
        assert(!_virtual, fn, "virtual is for functions");
        assert(!_force, fn, "force is for functions");
        assert(!_final, fn, "final is for functions");
        process_obj_def(file.code, i, _template);
        break;

      // Fn definition
      }else if(starts_with(line, "fn")){
        assert(!_template, fn, "template is for objects");
        process_fn_def(file.code, i, _const, _virtual, _force, _final);
        break;

      // Template keyword
      }else if(starts_with(line, "template")){
        assert(!_template, fn, "template declared twice");
        line = strip(delete_tok(line));
        _template = true;
        continue;

      // Const keyword
      }else if(starts_with(line, "const")){
        assert(!_const, fn, "const declared twice");
        line = strip(delete_tok(line));
        _const = true;
        continue;

      // Virtual keyword
      }else if(starts_with(line, "virtual")){
        assert(!_virtual, fn, "virtual declared twice");
        line = strip(delete_tok(line));
        _virtual = true;
        continue;

      // Force keyword
      }else if(starts_with(line, "force")){
        assert(!_force, fn, "force declared twice");
        line = strip(delete_tok(line));
        _force = true;
        continue;

      // Final keyword
      }else if(starts_with(line, "final")){
        assert(!_final, fn, "final declared twice");
        line = strip(delete_tok(line));
        _final = true;
        continue;

      // Function call
      }else if(is_upper(line[i]) || is_lower(line[i]) || line[i] == '_'){
        process_fn_call(line, _const);
        break;

      // Otherwise error
      }else
        err(fn, "syntax error"); } } }

void Compiler::process_obj_def(const vec<str>& code, const int line,
    const bool _template){
  const str fn = "Compiler.process_obj_def";
  // Get type name
  str s = code[line];
  while(next_tok(s) != "obj")
    s = delete_tok(s);
  s = delete_tok(s);
  str type = next_tok(s);
  assert(is_type(type), fn, "expected type name");
  s = delete_tok(s);
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

  for(int i = line; i < end; ++i){
    //! fn defs and member decls
  }

  //! log obj def in typemgr
}

void Compiler::process_fn_def(const vec<str>& code, const int line,
    const bool _const, const bool _virtual, const bool _force,
    const bool _final){

  //! log fn def in fns
}

void Compiler::process_fn_call(const str& call, const bool _const){}

#endif
