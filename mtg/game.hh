// MTG GAME

#ifndef GAME_HH
#define GAME_HH

#include "deck.hh"

struct Game;

struct Player {

  int life;
  Deck deck;
  vec<Card> hand, field;

  // Defined int bot.hh
  void act(Game& g);
  void attack(Game& g);
  void block(Game& g); };

struct Game {

  bool in_main;
  int turn;
  vec<Player> players;
  vec<Card> stack;

  bool over() const;

  void init();
  void play();
  void upkeep();
  void main();
  void combat();
  void end();
  bool prompt();

  // Defined in mech.hh
  void resolve(); };

bool Game::over() const {
  for(int i = 0; i < players.size(); ++i)
    if(players[i].life <= 0)
      return true;
  return false; }

void Game::init(){
  for(int i = 0; i < players.size(); ++i){
    players[i].life = 20;
    players[i].deck.shuffle();
    for(int j = 0; j < players[i].deck.pile.size(); ++j){
      players[i].deck.pile[j].tapped = false;
      players[i].deck.pile[j].attacking = NULL;
      players[i].deck.pile[j].blocking = NULL;
      players[i].deck.pile[j].counters.clear(); } }
  turn = 0;
  in_main = false; }

void Game::play(){
  // Draw 7
  //! Mulligan
  for(int i = 0; i < players.size(); ++i)
    for(int j = 0; j < 7; ++j)
      players[i].hand.pb(players[i].deck.draw());
  // Game loop
  while(1){
    upkeep();
    main();
    if(over()) break;
    combat();
    if(over()) break;
    main();
    if(over()) break;
    end(); } }

void Game::upkeep(){
  for(int i = 0; i < players[turn].field.size(); ++i)
    players[turn].field[i].tapped = false;
  players[turn].hand.pb(players[turn].deck.draw()); }

void Game::main(){
  in_main = true;
  while(prompt());
  in_main = false; }

void Game::combat(){
  players[turn].attack(*this);
  for(int i = 0; i < players.size(); ++i)
    if(i != turn)
      players[i].block(*this); }

void Game::end(){
  turn = (turn == players.size()) ? 0 : turn + 1; }

bool Game::prompt(){
  bool action = false;
  int prio = turn;
  int pass = 0;
  while(!stack.empty() || pass < players.size()){
    int ss = stack.size();
    players[prio].act(*this); //*
    printf("Stack: %d\n", stack.size()); //!
    if(stack.size() > ss){
      action = true;
      pass = 0;
      continue; }
    prio = (prio == players.size() - 1) ? 0 : prio + 1;
    ++pass;
    printf("PASS %d\n", pass); //!
    if(!stack.empty() && pass == players.size()){
      resolve();
      prio = turn;
      pass = 0; } }
  return action; }

#endif
