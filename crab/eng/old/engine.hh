// CRAB ENGINE

#ifndef ENGINE_HH
#define ENGINE_HH

#include "includer.hh"
#include "verifier.hh"
#include "cleaner.hh"
#include "compiler.hh"

struct Engine {

  // Files
  umap<str, File> files;

  // Execution prep
  Includer includer;
  Verifier verifier;
  Cleaner cleaner;
  Compiler compiler;

  void init();
  void process_script(const str& fname); };

// Initialize compiler and allocate initial memory
void Engine::init(){
  compiler.init();
  Var::allocator.init(); }

// Process all files and execute code from main file
void Engine::process_script(const str& fname){
  includer.process_file(fname, &files);
  verifier.verify_files(files);
  cleaner.clean_files(&files);
  Fn* main_fn = compiler.compile(files);
  assert(main_fn != NULL, "Engine.process_script",
      "Compiler returned NULL main function");
  main_fn->call(vec<Fn::FnCall>()); }

#endif
