// CRAB ENGINE MAIN

#include "engine.hh"

int main(int argc, const char** argv){
  if(argc != 2)
    err("main", "requires one command line argument");
  Engine eng;
  eng.process_script(argv[1]);
  return 0; }
