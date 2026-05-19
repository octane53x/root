// MTG BOT

#ifndef BOT_HH
#define BOT_HH

#include "game.hh"

void Player::act(Game& g){
  if(!(rand() % 4))
    g.stack.pb(hand[0]);
}

void Player::attack(Game& g){

}

void Player::block(Game& g){
  --life;
  printf("---- LIFE: %d\n", life);
}

#endif
