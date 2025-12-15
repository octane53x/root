// AI TEST

#include "lang/fn.hh"

int main(){

  Thing t("t", NULL);
  Fn f("f", &t);
  t.add(f);

  return 0; }
