// CRAB COMPILER MAIN

#include "compiler.hh"

int main(int argc, const char** argv){
  if(argc != 2)
    err("main", "requires one command line argument");
  str fname(argv[1]);
  Compiler c;
  c.compile(fname);
  printf("Compiled to ./main\n");
  return 0; }
