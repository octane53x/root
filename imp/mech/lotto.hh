// LOTTERY MECHANIC

#ifndef LOTTO_HH
#define LOTTO_HH

#include "../gl/util.hh"

const double C = 10.0;

struct Lotto {
  vec<llu> hist;

  void init(){

  }

  void check(){

  }

  llu gen(){
    llu r1 = lrand();
    llu r2 = lrand();
    llu r = (llu)floor(pow((double)r1 / r2 + 1.0, C));
    hist.pb(r);
    check();
    return r;
  }
};

#endif
