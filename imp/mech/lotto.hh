// LOTTERY

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
    llu r1 = (llu)rand() * rand() + rand();
    llu r2 = (llu)rand() * rand() + rand();
    llu r = (llu)floor(pow((double)r1 / r2 + 1.0, C));
    hist.pb(r);
    check();
    return r;
  }
};

#endif
