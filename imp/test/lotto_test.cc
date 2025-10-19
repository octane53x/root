// Test

#include "imp/lotto.hh"

int main(){
  Lotto lotto;
  lotto.init();
  while(1){
    llu ticket = lotto.gen();
    printf("%llu\n", ticket);
    sleep(1000);
  }
  return 0;
}
