// CHESS INPUTS

#ifndef INPUT_HH
#define INPUT_HH

#include "chess.hh"

// Map input functions
void Chess::map_fns(){
  keymap["LCLICK"] = click; }

// Click the mouse
bool Chess::click(const KeyEvent& ke){
  return false; }

#endif
