// MTG GAME

#ifndef GAME_HH
#define GAME_HH

#include "deck.hh"

struct Game {

  vec<int> life;
  vec<Deck> decks;

  void init();
  void play(); };

void Game::init(){
  life.clear();
  for(int i = 0; i < decks.size(); ++i)
    life.pb(20); }

void Game::play(){
  //!
}

#endif
