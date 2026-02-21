#include "../core/str.hh"

int main(){

  const str s = str("TEST");
  const str t = str("STR");
  printf("%s\n", (t + s).c_str());

  return 0; }
