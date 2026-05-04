// DRAW OPERATIONS

#ifndef DRAW_HH
#define DRAW_HH

#include "../gl/polygon.hh"
#include "chess.hh"

void Chess::draw(){
  draw_board();
  draw_moves();
  draw_pieces(); }

void Chess::draw_board(){
  for(int i = 0; i < 8; ++i){
    color c = (i & 1) ? BLACK : WHITE;
    for(int j = 0; j < 8; ++j){
      polygon p;
      p.add(point(i * SQUARE, j * SQUARE));
      p.add(point((i + 1) * SQUARE - 1, j * SQUARE));
      p.add(point((i + 1) * SQUARE - 1, (j + 1) * SQUARE - 1));
      p.add(point(i * SQUARE, (j + 1) * SQUARE - 1));
      p.fill = c;
      p.object::draw(&frame);
      c = (c == WHITE) ? BLACK : WHITE; } } }

void Chess::draw_moves(){
  if(select.x == -1)
    return;
  int i = select.x, j = select.y;
  polygon p;
  p.add(point(j * SQUARE, (7 - i) * SQUARE));
  p.add(point((j + 1) * SQUARE - 1, (7 - i) * SQUARE));
  p.add(point((j + 1) * SQUARE - 1, (8 - i) * SQUARE - 1));
  p.add(point(j * SQUARE, (8 - i) * SQUARE - 1));
  p.fill = YELLOW;
  p.object::draw(&frame);
  vec<Move> m = board.moves(true);
  for(const Move& t : m){
    if(t.src != select)
      continue;
    p.clear();
    int x = t.dest.y * SQUARE;
    int y = (7 - t.dest.x) * SQUARE;
    p.add(point(x + SQUARE / 2, y + 10));
    p.add(point(x + SQUARE - 10, y + SQUARE / 2));
    p.add(point(x + SQUARE / 2, y + SQUARE - 10));
    p.add(point(x + 10, y + SQUARE / 2));
    p.fill = ORANGE;
    p.object::draw(&frame); } }

void Chess::draw_pieces(){
  for(int i = 0; i < 8; ++i)
    for(int j = 0; j < 8; ++j){
      if(board.board[i][j].unit == Unit::NONE)
        continue;
      color c = (board.board[i][j].player == Player::WHITE) ? RED : BLUE;
      char ch;
      switch(board.board[i][j].unit){
        case Unit::PAWN: ch = 'P'; break;
        case Unit::ROOK: ch = 'R'; break;
        case Unit::KNIGHT: ch = 'L'; break;
        case Unit::BISHOP: ch = 'B'; break;
        case Unit::QUEEN: ch = 'Q'; break;
        case Unit::KING: ch = 'K'; break;
        default: ch = '?'; }
      point pos(j * SQUARE + 15, SQUARE * 7 - i * SQUARE);
      display_text(ch, SQUARE, "Arial", pos, c); } }

#endif
