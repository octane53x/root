// MTG GAME

#ifndef GAME_HH
#define GAME_HH

#include "deck.hh"

struct Player {

  int life;
  Deck deck;
  vec<Card> hand, field;

  void act();
  void attack();
  void block(); };

struct Game {

  int turn;
  vec<Player> players;
  vec<Card> stack;

  bool over() const;

  void play();
  void upkeep(const int player);
  void main(const int player);
  void combat(const int player);
  void end(const int player); };

bool Game::over(){
  for(int i = 0; i < life.size(); ++i)
    if(life[i] <= 0)
      return true;
  return false; }

void Game::play(){
  life.clear();
  for(int i = 0; i < decks.size(); ++i){
    life.pb(20);
    decks[i].shuffle(); }
  turn = 0;
  while(1){
    upkeep(turn);
    main(turn);
    if(over()) break;
    combat(turn);
    if(over()) break;
    main(turn);
    if(over()) break;
    end(turn); } }

void Game::upkeep(const int player){
  for(int i = 0; i < field[player].size(); ++i)
    field[player][i].tapped = false;
  hand[player].pb(decks[player].draw()); }

void Game::main(const int player){

}

void Game::combat(const int player){

}

void Game::end(const int player){
  turn = (turn == decks.size()) ? 0 : turn + 1; }

#endif
