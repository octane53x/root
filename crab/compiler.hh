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
  void process_obj_def(const File& file, const int line, const bool _template);
  void process_fn_def(const File& file, const int line,
      const bool _const, const bool _virtual, const bool _block);
  void process_var_decl(const str& decl);
  void process_fn_call(const str& call); };

// Returns pointer to main function
Fn* Compiler::compile(const umap<str, File>& files, umap<str, Fn>* fns){
  str main_file = "";
  for(pair<str, File> p : files)
    if(p.second.main){
      main_file = p.first;
      break; }
  assert(main_file != "", "Compiler.compile",
      "main file not marked in includer");
  process_file(files[main_file], fns);
  return &fns[MAIN_FILE]; }

void Compiler::process_file(const File& file, umap<str, Fn>* fns){
  const str fn = "Compiler.process_file";
  //! obj def, fn def, var decl, fn call
  for(int i = 0; i < file.code.size(); ++i){
    str line = file.code[i].strip();

    bool _template = false, _const = false, _virtual = false, _block = false;
    while(1){
      if(starts_with(line, "obj")){
        assert(!_const, fn, "cannot have const object definition");
        assert(!_virtual, fn, "cannot have virtual object definition");
        assert(!_block, fn, "cannot have blocked object definition");
        process_obj_def(file, i, _template);
        break;

      }else if(starts_with(line, "fn")){
        assert(!_template, fn, "cannot have templated fn definition");
        process_fn_def(file, i, _const, _virtual, _block);
        break;

      }else if(starts_with(line, "template")){
        line = delete_tok(line).strip();
        _template = true;
        continue;

      }else if(starts_with(line, "virtual")){
        line = delete_tok(line).strip();
        _virtual = true;
        continue;

      }else if(starts_with(line, "const")){
        line = delete_tok(line).strip();
        _const = true;
        continue;

      }else if(starts_with(line, "block")){
        line = delete_tok(line).strip();
        _block = true;
        continue;

      // Variable declaration or function call, otherwise syntax error
      }else{

      }
    }
  }
}

void Compiler::process_obj_def(const File& file, const int line,
    const bool _template){}

void Compiler::process_fn_def(const File& file, const int line,
    const bool _const, const bool _virtual, const bool _block){}

void Compiler::process_var_decl(const str& decl){}

void Compiler::process_fn_call(const str& call){}

#endif
