// COMPILER

#ifndef COMPILER_HH
#define COMPILER_HH

#include "file.hh"
#include "fn.hh"

struct Compiler {

  Fn* compile(const umap<str, File>& files, umap<ID, Fn>* fns); };

// Returns pointer to main function
Fn* Compiler::compile(const umap<str, File>& files, umap<ID, Fn>* fns){
  return NULL; }

#endif
