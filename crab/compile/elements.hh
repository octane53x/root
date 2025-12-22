// VERIFY ELEMENTS

#ifndef ELEMENTS_HH
#define ELEMENTS_HH

#include "compiler.hh"

// Check variables and functions for declarations and definitions
void Compiler::verify_elements(const str& fname){
  File& file = files[fname];
  if(file.vis) return;
  file.vis = true;
  // Depth-first recursion to compile sources first
  for(File* f : file.sources)
    verify_elements(f->name);

}

#endif
