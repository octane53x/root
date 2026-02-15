// MTG SIMULATOR

#ifndef SIM_HH
#define SIM_HH

#include "game.hh"

void sim(const umap<str, Card>& cards){
  vec<Card> card_list;
  for(const pair<str, Card>& e : cards)
    card_list.pb(e.second);

  Deck d1, d2;
  d1.fill(card_list);
  d2.fill(card_list);
  Game g;
  g.decks.pb(d1);
  g.decks.pb(d2);

  while(1){
    g.init();
    g.play();
    printf("\nDeck 1: %lf\nDeck 2: %lf\n",
        g.decks[0].score(), g.decks[1].score());
    g.decks[0].replace(card_list, 10);
    g.decks[1].replace(card_list, 10); } }

#endif
