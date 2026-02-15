// MTG CONSOLE

#include "parse.hh"
#include "sim.hh"

int main(){
  // Parse cards from JSON
  printf("Parsing JSON ...\n");
  clock_t t0 = clock();
  umap<str, Card> all_cards = parse();
  clock_t tf = clock();
  printf("Parsed %d cards in %lf seconds\n\n",
      all_cards.size(), (double)(tf - t0) / CPS);

  printf("Processing card text ... \n");
  t0 = clock();
  umap<str, Card> cards;
  for(pair<str, Card> e : all_cards)
    if(e.second.process_text())
      cards[e.second.name] = e.second;
  tf = clock();
  printf("Added %d cards to playlist in %lf seconds\n\n",
      cards.size(), (double)(tf - t0) / CPS);

  sim(cards);

  //!
  // int i = 0;
  // for(pair<str, Card> e : cards){
  //   ++i;
  //   const Card& c = e.second;
  //   if(c.mods.empty()) continue;
  //   // int n = 0;
  //   // for(const str& s : c.text)
  //   //   n += s.size();
  //   // if(n > 40) continue;
  //   printf("%d:\n", i);
  //   e.second.print();
  //   char ch;
  //   scanf("%c", &ch); }

  return 0; }
