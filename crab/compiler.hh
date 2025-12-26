// COMPILER

#ifndef COMPILER_HH
#define COMPILER_HH

#include "file.hh"
#include "fn.hh"

struct Compiler {

  umap<Control, Fn*> controls;

  void init(umap<ID, Fn>* fns);
  Fn* compile(const umap<str, File>& files, umap<ID, Fn>* fns); };

void Compiler::init(umap<ID, Fn>* fns){
  // Add control functions
  for(Control ctrl : CONTROL){
    Fn fn;
    fn.control = &ctrl;
    fn.name = ctrl;
    (*fns)[fn.id] = fn;
    controls[ctrl] = &(*fns)[fn.id]; } }

// Returns pointer to main function
Fn* Compiler::compile(const umap<str, File>& files, umap<ID, Fn>* fns){
  return NULL; }

#endif
