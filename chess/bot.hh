// CHESS BOT

//! Doesn't work too well

#ifndef BOT_HH
#define BOT_HH

#include "chess.hh"

const int DEPTH = 3;

// Score the board
int Chess::score(const Board& b) const {
  int s = 0;
  for(int i = 0; i < 8; ++i)
    for(int j = 0; j < 8; ++j){
      int val;
      switch(b.board[i][j].unit){
        case Unit::PAWN: val = 1; break;
        case Unit::ROOK: val = 5; break;
        case Unit::KNIGHT: val = 3; break;
        case Unit::BISHOP: val = 3; break;
        case Unit::QUEEN: val = 10; break;
        case Unit::KING: val = 1000; break;
        default: val = 0; }
      // ???
      int z = (b.board[i][j].player == b.turn)
          ? ((DEPTH & 1) ? -1 : 1) : ((DEPTH & 1) ? 1 : -1);
      s += val * z; }
  return s; }

// Recurse moves to DEPTH depth
double Chess::delve(const Board& b, int depth, bool mx) const {
  if(depth == DEPTH)
    return score(b);
  vec<Move> m = b.moves(true);
  double n = mx ? -INFD : INFD;
  for(const Move& t : m){
    if(b.board[t.src.xi()][t.src.yi()].player != b.turn)
      continue;
    Board b2 = b;
    b2.move(t);
    double s = delve(b2, depth + 1, !mx);
    n = mx ? max(n, s) : min(n, s); }
  return n; }

// Compute a move
void Chess::bot_move(){
  vec<Move> m = board.moves(true);
  double md = -INFD;
  Move mf;
  for(const Move& t : m){
    if(board.board[t.src.xi()][t.src.yi()].player != board.turn)
      continue;
    Board b = board;
    b.move(t);
    double diff = delve(b, 1, false);
    if(diff > md){
      md = diff;
      mf = t; } }
  board.move(mf); }

#endif
