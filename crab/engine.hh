// CRAB ENGINE

#ifndef ENGINE_HH
#define ENGINE_HH

#include "includer.hh"
#include "verifier.hh"
#include "compiler.hh"

struct Engine {

  umap<str, File> files;
  umap<ID, Fn> fns;

  // Execution prep
  Includer includer;
  Verifier verifier;
  Compiler compiler;

  // Execution
  Language lang;
  TypeMgr types;
  Allocator allocator;
  AccessMgr access;

  void init();
  void process_script(const str& fname); };

void Engine::init(){
  compiler.init(&fns);
  allocator.init(); }

void Engine::process_script(const str& fname){
  includer.process_file(fname, &files);
  verifier.verify_files(files);
  Fn* main_fn = compiler.compile(files, &fns);
  assert(main_fn != NULL, "Engine.process_script",
      "Compiler returned NULL main function");
  main_fn->call(vec<Fn::FnCall>()); }

#endif
