// CHESS INPUTS

#ifndef INPUT_HH
#define INPUT_HH

#include "chess.hh"

// Map input functions
void Chess::map_fns(){
  keymap["LCLICK"] = click;
  keymap["CTRL+q"] = kill; }

// Click the mouse
bool Chess::click(const KeyEvent& ke){
  int i = 7 - ke.cursor.y / SQUARE;
  int j = ke.cursor.x / SQUARE;
  if(selected){
    //!

  }else{
    if(board.board[i][j].unit == Unit::NONE)
      return false;
    //!

  }
  return true; }

// Terminate application
bool Chess::kill(const KeyEvent& ke){
  quit();
  return true; }

#endif
