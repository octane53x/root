// CRAB ENGINE MAIN

#include "engine/engine.hh"

// Main function
int main(int argc, const char** argv){
  if(argc != 2)
    err("main", "requires one command line argument");
  Engine eng;
  eng.init();
  eng.process_script(argv[1]);
  return 0; }
