// AI TEST

#include "lang/fn.hh"

struct Fn1 : virtual Fn {
  Fn1(const Name& _name, Thing* _ctr): Fn(_name, _ctr) {}
  void call(const vec<Var*>& params){
    printf(("Called "+to_str()).c_str()); } };

int main(){

  Thing t("t", NULL);
  Fn1 f("f", &t);
  t.add(&f);

  return 0; }
