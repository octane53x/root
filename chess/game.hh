// GAME OPERATIONS

#ifndef GAME_HH
#define GAME_HH

#include "chess.hh"

// Is the location on the board
bool Board::in_bounds(pair<int, int> loc) const {
  return loc.first >= 0 && loc.first < 8 && loc.second >= 0 && loc.second < 8; }

// Does player have a check on other player
bool Board::check(Player p) const {
  Player ENEMY = (p == Player::WHITE) ? Player::BLACK : Player::WHITE;
  vec<Move> m = moves(p);
  for(const Move& t : m)
    if(board[t.dest.first][t.dest.second].owner == ENEMY
        && board[t.dest.first][t.dest.second].unit == Unit::KING)
      return true;
  return false; }

// Does player have a checkmate on other player
bool Board::mate(Player p) const {
  // If other king has moves, return false
  Player ENEMY = (p == Player::WHITE) ? Player::BLACK : Player::WHITE;
  vec<Move> m = moves(ENEMY);
  for(int i = 0; i < m.size(); ++i)
    if(board[m[i].src.first][m[i].src.second].unit == Unit::KING)
      return false;
  // Return whether other player is in check
  return check(p); }

// Is the game in stalemate
bool Board::stale() const {
  // Not if checkmate
  if(mate(Player::WHITE) || mate(Player::BLACK))
    return false;
  // No moves
  if(moves(Player::WHITE).empty() || moves(Player::BLACK).empty())
    return true;
  // Insufficient material
  //!

  return false; }

// Available moves for player
vec<Move> Board::moves(Player p) const {
  int DIR = (p == Player::WHITE) ? 1 : -1;
  Player ENEMY = (p == Player::WHITE) ? Player::BLACK : Player::WHITE;
  vec<Move> m;
  for(int i = 0; i < 8; ++i)
    for(int j = 0; j < 8; ++j){
      if(board[i][j].unit == Unit::NONE || board[i][j].owner != p)
        continue;

      // Pawn
      //! En passant
      if(board[i][j].unit == Unit::PAWN){
        if(in_bounds({i + DIR, j}) && board[i + DIR][j].unit == Unit::NONE)
          m.pb(Move({i, j}, {i + DIR, j}));
        if(((p == Player::WHITE && i == 1) || (p == Player::BLACK && i == 6))
            && board[i + DIR][j].unit == Unit::NONE
            && board[i + DIR * 2][j].unit == Unit::NONE)
          m.pb(Move({i, j}, {i + DIR * 2, j}));
        if(in_bounds({i + DIR, j - 1}) && board[i + DIR][j - 1].owner == ENEMY)
          m.pb(Move({i, j}, {i + DIR, j - 1}));
        if(in_bounds({i + DIR, j + 1}) && board[i + DIR][j + 1].owner == ENEMY)
          m.pb(Move({i, j}, {i + DIR, j + 1})); }

      // Bishop or Queen
      if(board[i][j].unit == Unit::BISHOP){
        for(int di = -1; di <= 1; di += 2)
          for(int dj = -1; dj <= 1; dj += 2)
            for(int k = 1; 1; ++k){
              int ii = i + k * di;
              int jj = j + k * dj;
              if(!in_bounds({ii, jj}) || board[ii][jj].owner == p)
                break;
              m.pb(Move({i, j}, {ii, jj}));
              if(board[ii][jj].owner == ENEMY)
                break; } }

      // Knight
      if(board[i][j].unit == Unit::KNIGHT){
        for(int di = -1; di <= 1; di += 2)
          for(int dj = -1; dj <= 1; dj += 2)
            for(int ki = 1; ki <= 2; ++ki){
              int kj = (ki == 1) ? 2 : 1;
              int ii = i + ki * di;
              int jj = j + kj * dj;
              if(!in_bounds({ii, jj}) || board[ii][jj].owner == p)
                break;
              m.pb(Move({i, j}, {ii, jj})); } }

      // Rook or Queen
      if(board[i][j].unit == Unit::ROOK){
        for(int di = -1; di <= 1; di += 2)
          for(int k = 1; 1; ++k){
            int ii = i + k * di;
            if(!in_bounds({ii, j}) || board[ii][j].owner == p)
              break;
            m.pb(Move({i, j}, {ii, j}));
            if(board[ii][j].owner == ENEMY)
              break; }
        for(int dj = -1; dj <= 1; dj += 2)
          for(int k = 1; 1; ++k){
            int jj = j + k * dj;
            if(!in_bounds({i, jj}) || board[i][jj].owner == p)
              break;
            m.pb(Move({i, j}, {i, jj}));
            if(board[i][jj].owner == ENEMY)
              break; } }

      // King
      if(board[i][j].unit == Unit::KING){
        vec<pair<int, int> > t = {{-1, -1}, {-1, 0}, {-1, 1},
            {1, -1}, {1, 0}, {1, 1}, {0, -1}, {0, 1}};
        for(pair<int, int> tt : t){
          int ii = i + tt.first;
          int jj = j + tt.second;
          if(!in_bounds({ii, jj}) || board[ii][jj].owner == p)
            continue;
          Board b = *this;
          b.board[ii][jj] = b.board[i][j];
          b.board[i][j] = Piece(Player::NONE, Unit::NONE);
          if(!b.check(ENEMY))
            m.pb(Move({i, j}, {ii, jj})); } } }
  return m; }

// Initialize game
void Chess::init_game(){
  turn = Player::WHITE;
  board.board[0][0] = board.board[0][7] = Piece(Player::WHITE, Unit::ROOK);
  board.board[0][1] = board.board[0][6] = Piece(Player::WHITE, Unit::KNIGHT);
  board.board[0][2] = board.board[0][5] = Piece(Player::WHITE, Unit::BISHOP);
  board.board[0][3] = Piece(Player::WHITE, Unit::QUEEN);
  board.board[0][4] = Piece(Player::WHITE, Unit::KING);
  for(int i = 0; i < 8; ++i)
    board.board[1][i] = Piece(Player::WHITE, Unit::PAWN);
  board.board[7][0] = board.board[7][7] = Piece(Player::BLACK, Unit::ROOK);
  board.board[7][1] = board.board[7][6] = Piece(Player::BLACK, Unit::KNIGHT);
  board.board[7][2] = board.board[7][5] = Piece(Player::BLACK, Unit::BISHOP);
  board.board[7][3] = Piece(Player::BLACK, Unit::QUEEN);
  board.board[7][4] = Piece(Player::BLACK, Unit::KING);
  for(int i = 0; i < 8; ++i)
    board.board[6][i] = Piece(Player::BLACK, Unit::PAWN);
  for(int i = 2; i <= 5; ++i)
    for(int j = 0; j < 8; ++j)
      board.board[i][j] = Piece(Player::NONE, Unit::NONE); }

#endif
