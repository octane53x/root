// MTG MECHANICS

#ifndef MECH_HH
#define MECH_HH

#include "game.hh"

// Perform the effect of the top card on the stack
void Game::resolve(){
  stack.pop_back();
}

#endif
