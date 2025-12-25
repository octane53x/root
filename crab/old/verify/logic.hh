// VERIFY LOGIC

#ifndef LOGIC_HH
#define LOGIC_HH

#include "compiler.hh"

// Check code for valid logic
void Compiler::verify_logic(const str& fname){
  File& file = files[fname];
  if(file.vis) return;
  file.vis = true;
  // Depth-first recursion to compile sources first
  for(File* f : file.sources)
    verify_logic(f->name);

}

#endif
