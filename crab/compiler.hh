// COMPILER

#ifndef COMPILER_HH
#define COMPILER_HH

#include "file.hh"
#include "fn.hh"

struct Compiler {

  Fn* compile(const umap<str, File>& files, umap<llu, Fn>* fns); };

Fn* Compiler::compile(const umap<str, File>& files, umap<llu, Fn>* fns){

  return 0; }

#endif
