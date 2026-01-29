// COMPILE AND EXECUTE SCRIPT

#include "eng/parse.hh"

int main(int argc, const char** argv){
  if(argc != 2)
    err("main", "requires one command line argument");
  Engine eng;
  eng.init();
  if(eng.compile(argv[1]))
    eng.run();
  for(const str& error : eng.errors)
    printl(error + "\n");
  return 0; }
