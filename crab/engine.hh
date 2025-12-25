// CRAB ENGINE

#ifndef ENGINE_HH
#define ENGINE_HH

#include "includer.hh"
#include "verifier.hh"
#include "compiler.hh"

struct Engine {

  umap<str, File> files;
  umap<llu, Fn> fns;

  Includer includer;
  Verifier verifier;
  Compiler compiler;

  void process_script(const str& fname); };

void Engine::process_script(const str& fname){
  includer.process_file(fname, &files);
  verifier.verify_files(files);
  Fn* main_fn = compiler.compile(files, &fns);
  main_fn->call(); }

#endif
