// MTG SIMULATOR

#ifndef SIM_HH
#define SIM_HH

#include "game.hh"

void sim(const umap<str, Card>& cards){
  vec<Card> card_list;
  for(const auto& [key, card] : cards)
    card_list.pb(card);

  Game g;
  Player p1, p2;
  p1.deck.fill(card_list);
  p2.deck.fill(card_list);
  g.players.pb(p1);
  g.players.pb(p2);

  while(1){
    g.init();
    g.play();
    printf("\nDeck 1: %lf\nDeck 2: %lf\n",
        g.players[0].deck.score(), g.players[1].deck.score());
    g.players[0].deck.replace(card_list, 10);
    g.players[1].deck.replace(card_list, 10); } }

#endif
