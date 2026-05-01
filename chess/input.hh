// CHESS INPUTS

#ifndef INPUT_HH
#define INPUT_HH

#include "chess.hh"

// Map input functions
void Chess::map_fns(){
  keymap["LCLICK"] = click;
  keymap["CTRL+q"] = kill; }

// Project pointer for static input functions, set in cc file
Chess* _app;

// Click the mouse
bool Chess::click(const KeyEvent& ke){
  if(!ke.down)
    return false;
  int i = 7 - ke.cursor.y / SQUARE;
  int j = ke.cursor.x / SQUARE;
  if(_app->select.x == -1){
    if(_app->board.board[i][j].player != Player::WHITE)
      return false;
    // Select piece
    _app->select = ipoint(i, j);

  }else{
    vec<Move> m = _app->board.moves(Player::WHITE);
    for(const Move& t : m){
      if(t.src != _app->select || t.dest != ipoint(i, j))
        continue;

      // Move piece

      return true; }

    // Unselect piece
    _app->select = ipoint(-1, -1);
  }
  return true; }

// Terminate application
bool Chess::kill(const KeyEvent& ke){
  quit();
  return true; }

#endif
