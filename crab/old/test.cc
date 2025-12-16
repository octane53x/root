// AI TEST

#include "lang/fn.hh"

struct Fn1 : virtual Fn {
  Fn1(){ type = "Fn1"; }
  void fn(){
    printf(("Called "+to_str()).c_str()); } };

int main(){

  Thing t("t", NULL);
  Fn1 f("f", &t);
  t.add(&f);

  return 0; }
