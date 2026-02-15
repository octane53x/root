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

  //!
  for(pair<str, Card> e : cards){
    e.second.print();
    char c;
    scanf("%c", &c); }

  return 0; }
