// DRAW OPERATIONS

#ifndef DRAW_HH
#define DRAW_HH

#include "../gl/polygon.hh"
#include "chess.hh"

void Chess::draw(){
  draw_board();
  draw_pieces(); }

void Chess::draw_board(){
  for(int i = 0; i < 8; ++i){
    color c = (i & 1) ? BLACK : WHITE;
    for(int j = 0; j < 8; ++j){
      polygon p;
      p.add(ipoint(i * SQUARE, j * SQUARE));
      p.add(ipoint((i + 1) * SQUARE - 1, j * SQUARE));
      p.add(ipoint((i + 1) * SQUARE - 1, (j + 1) * SQUARE - 1));
      p.add(ipoint(i * SQUARE, (j + 1) * SQUARE - 1));
      p.fill = c;
      p.draw(&frame, viewport());
      c = (c == WHITE) ? BLACK : WHITE; } } }

void Chess::draw_pieces(){
  for(int i = 0; i < 8; ++i)
    for(int j = 0; j < 8; ++j){
      if(board.board[i][j].unit == Unit::NONE)
        continue;
      color c = (board.board[i][j].owner == Player::WHITE) ? RED : BLUE;
      char ch;
      switch(board.board[i][j].unit){
        case Unit::PAWN: ch = 'P'; break;
        case Unit::ROOK: ch = 'R'; break;
        case Unit::KNIGHT: ch = 'L'; break;
        case Unit::BISHOP: ch = 'B'; break;
        case Unit::QUEEN: ch = 'Q'; break;
        case Unit::KING: ch = 'K'; break;
        default: ch = '?'; }
      ipoint pos(j * SQUARE + 15, SQUARE * 7 - i * SQUARE);
      display_text(ch, SQUARE, "Arial", pos, c); } }

#endif
