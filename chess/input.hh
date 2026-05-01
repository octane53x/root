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
  // Select piece
  if(_app->board.board[i][j].player == Player::WHITE){
    _app->select = ipoint(i, j);
    return true;

  }else if(_app->select.x != -1){
    vec<Move> m = _app->board.moves(Player::WHITE);
    for(const Move& t : m){
      if(t.src != _app->select || t.dest != ipoint(i, j))
        continue;

      // Move piece
      _app->board.board[t.dest.x][t.dest.y] =
          _app->board.board[t.src.x][t.src.y];
      _app->board.board[t.src.x][t.src.y] = Piece(Player::NONE, Unit::NONE);
      _app->select = ipoint(-1, -1);
      return true; }

    // Unselect piece
    _app->select = ipoint(-1, -1);
    return true; }
  return false; }

// Terminate application
bool Chess::kill(const KeyEvent& ke){
  quit();
  return true; }

#endif
