// MTG CONSOLE

#include "parse.hh"

int main(){
  // Parse cards from JSON
  printf("Parsing JSON ...\n");
  clock_t t0 = clock();
  umap<str, Card> cards = parse();
  clock_t tf = clock();
  printf("Parsed %d cards in %lf seconds\n",
      cards.size(), (double)(tf - t0) / CPS);

  for(pair<str, Card> entry : cards){
    for(Cost& c : entry.second.cost)
      if(c.color == 'Y' || c.color == 'Z')
        entry.second.print(), printf("\n\n"); }

  return 0; }
