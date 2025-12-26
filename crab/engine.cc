// CRAB ENGINE MAIN

#include "engine.hh"

// Global engine
Engine eng;

// Static members
TypeMgr* Type::registry = &eng.types;
Allocator* Var::allocator = &eng.allocator;
llu Fn::next_id = 1;
llu Fn::scope = 0;
Language* Fn::lang = &eng.lang;
AccessMgr* Fn::access = &eng.access;

// Main function
int main(int argc, const char** argv){
  if(argc != 2)
    err("main", "requires one command line argument");
  eng.init();
  eng.process_script(argv[1]);
  return 0; }
