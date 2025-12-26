// COMPILER

#ifndef COMPILER_HH
#define COMPILER_HH

#include "file.hh"
#include "fn.hh"

struct Compiler {

  umap<Language::Control, Fn*> controls;

  void init(umap<ID, Fn>* fns);
  Fn* compile(const umap<str, File>& files, umap<ID, Fn>* fns); };

void Compiler::init(umap<ID, Fn>* fns){}

Fn* Compiler::compile(const umap<str, File>& files, umap<ID, Fn>* fns){
  return NULL; }

#endif
