// TRADERS

/*!
1. Item listings
2. Reputations
3. Travel distance + work nodes
4. Fuel + pump nodes
5. Taxes for regions and item groups
6. Planned changes over time
7. Cars for faster transit
8. Opening hours
*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define vec std::vector
#define pb push_back

// Trader takes item1 and provides floor(quantity*rate) of item2
struct Listing {
  int item1, item2;
  double rate;
  Listing(){}
};

struct Trader {
  vec<Listing> listings;
  Trader(){}
};

const int
    MOD = 1000000000,
    NTRADERS = 100,
    NITEMS = 100,
    NLISTINGS = 100;

vec<Trader> traders;
map<int, int> inventory; // item : amount

void create(){
  // Create listings
  for(int i = 0; i < NTRADERS; ++i){
    Trader tr;
    for(int j = 0; j < NLISTINGS; ++j){
      int i1 = rand() % NITEMS + 1;
      int i2 = i1;
      while(i2 == i1)
        i2 = rand() % NITEMS + 1;
      double r = 11.0;
      while(r < 0.1 || r > 10.0)
        r = ((double)(rand() % MOD) / MOD) * 10.0;

      Listing li;
      li.item1 = i1;
      li.item2 = i2;
      li.rate = r;
      tr.listings.pb(li);
    }
    traders.pb(tr);
  }
  // Supply starting items
  inventory[1] = 10;
}

void solve(){

}

int main(){
  create();
  solve();
  return 0;
}
