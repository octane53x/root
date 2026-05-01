// CHESS INPUTS

#ifndef INPUT_HH
#define INPUT_HH

#include "chess.hh"

// Map input functions
void Chess::map_fns(){
  keymap["LCLICK"] = click;
  keymap["CTRL+q"] = kill; }

// Cast the application pointer for static input functions
Chess* _app;

// Click the mouse
bool Chess::click(const KeyEvent& ke){
  int i = 7 - ke.cursor.y / SQUARE;
  int j = ke.cursor.x / SQUARE;
  if(_app->select.first == -1){
    if(_app->board.board[i][j].player != Player::WHITE)
      return false;
    _app->select = pair<int, int>(i, j);
    _app->updated = true;

  }else{
    //!

  }
  return true; }

// Terminate application
bool Chess::kill(const KeyEvent& ke){
  quit();
  return true; }

#endif
