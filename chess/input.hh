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
  if(!ke.down)
    return false;
  int i = 7 - ke.cursor.y / SQUARE;
  int j = ke.cursor.x / SQUARE;
  // Select piece
  if(_app->board.board[i][j].player == _app->board.turn
      && _app->select != point(i, j)){
    _app->select = point(i, j);
    return true;

  }else if(_app->select.x != -1){
    vec<Move> m = _app->board.moves(true);
    for(const Move& t : m){
      if(t.src != _app->select || t.dest != point(i, j))
        continue;

      // Move piece
      _app->board.move(t);
      _app->select = point(-1, -1);
      return true; }

    // Unselect piece
    _app->select = point(-1, -1);
    return true; }
  return false; }

// Terminate application
bool Chess::kill(const KeyEvent& ke){
  quit();
  return true; }

#endif
