// GAME OPERATIONS

#ifndef GAME_HH
#define GAME_HH

#include "chess.hh"

// Is the location on the board
bool Board::in_bounds(point loc) const {
  return loc.x >= 0 && loc.x < 8 && loc.y >= 0 && loc.y < 8; }

// Does player have a check on other player
bool Board::check(Player p) const {
  Player ENEMY = (p == Player::WHITE) ? Player::BLACK : Player::WHITE;
  vec<Move> m = moves(true);
  for(const Move& t : m)
    if(board[t.dest.xi()][t.dest.yi()].player == ENEMY
        && board[t.dest.xi()][t.dest.yi()].unit == Unit::KING)
      return true;
  return false; }

// Does player have a checkmate on other player
bool Board::mate(Player p) const {
  // If other king has moves, return false
  Player ENEMY = (p == Player::WHITE) ? Player::BLACK : Player::WHITE;
  vec<Move> m = moves(true);
  for(int i = 0; i < m.size(); ++i)
    if(board[m[i].src.xi()][m[i].src.yi()].unit == Unit::KING
        && board[m[i].src.xi()][m[i].src.yi()].player == ENEMY)
      return false;
  // Return whether other player is in check
  return check(p); }

// Is the game in stalemate
bool Board::stale() const {
  // Not if checkmate
  if(mate(Player::WHITE) || mate(Player::BLACK))
    return false;
  // No moves
  vec<Move> m = moves(true);
  int w = 0, b = 0;
  for(const Move& t : m)
    ++((board[t.src.xi()][t.src.yi()].player == Player::WHITE) ? w : b);
  if(w == 0 || b == 0)
    return true;
  // Insufficient material
  //!

  return false; }

// Available moves for player
vec<Move> Board::moves(bool filter) const {
  vec<Move> m;
  for(int i = 0; i < 8; ++i)
    for(int j = 0; j < 8; ++j){
      if(board[i][j].unit == Unit::NONE)
        continue;
      Player p = board[i][j].player;
      Player ENEMY = (p == Player::WHITE) ? Player::BLACK : Player::WHITE;
      int DIR = (p == Player::WHITE) ? 1 : -1;

      // Pawn
      //! En passant
      if(board[i][j].unit == Unit::PAWN){
        // One step forward
        if(in_bounds(point(i + DIR, j)) && board[i + DIR][j].unit == Unit::NONE)
          m.pb(Move(point(i, j), point(i + DIR, j)));
        // Two steps forward
        if(((p == Player::WHITE && i == 1) || (p == Player::BLACK && i == 6))
            && board[i + DIR][j].unit == Unit::NONE
            && board[i + DIR * 2][j].unit == Unit::NONE)
          m.pb(Move(point(i, j), point(i + DIR * 2, j)));
        // Diagonal capture left
        if(in_bounds(point(i + DIR, j - 1))
            && board[i + DIR][j - 1].player == ENEMY)
          m.pb(Move(point(i, j), point(i + DIR, j - 1)));
        // Diagonal capture right
        if(in_bounds(point(i + DIR, j + 1))
            && board[i + DIR][j + 1].player == ENEMY)
          m.pb(Move(point(i, j), point(i + DIR, j + 1))); }

      // Bishop or Queen
      if(contains(vec<Unit>({Unit::BISHOP, Unit::QUEEN}), board[i][j].unit)){
        for(int di = -1; di <= 1; di += 2)
          for(int dj = -1; dj <= 1; dj += 2)
            for(int k = 1; 1; ++k){
              int ii = i + k * di;
              int jj = j + k * dj;
              if(!in_bounds(point(ii, jj)) || board[ii][jj].player == p)
                break;
              m.pb(Move(point(i, j), point(ii, jj)));
              if(board[ii][jj].player == ENEMY)
                break; } }

      // Knight
      if(board[i][j].unit == Unit::KNIGHT){
        for(int di = -1; di <= 1; di += 2)
          for(int dj = -1; dj <= 1; dj += 2)
            for(int ki = 1; ki <= 2; ++ki){
              int kj = (ki == 1) ? 2 : 1;
              int ii = i + ki * di;
              int jj = j + kj * dj;
              if(!in_bounds(point(ii, jj)) || board[ii][jj].player == p)
                continue;
              m.pb(Move(point(i, j), point(ii, jj))); } }

      // Rook or Queen
      if(contains(vec<Unit>({Unit::ROOK, Unit::QUEEN}), board[i][j].unit)){
        for(int di = -1; di <= 1; di += 2)
          for(int k = 1; 1; ++k){
            int ii = i + k * di;
            if(!in_bounds(point(ii, j)) || board[ii][j].player == p)
              break;
            m.pb(Move(point(i, j), point(ii, j)));
            if(board[ii][j].player == ENEMY)
              break; }
        for(int dj = -1; dj <= 1; dj += 2)
          for(int k = 1; 1; ++k){
            int jj = j + k * dj;
            if(!in_bounds(point(i, jj)) || board[i][jj].player == p)
              break;
            m.pb(Move(point(i, j), point(i, jj)));
            if(board[i][jj].player == ENEMY)
              break; } }

      // King
      //! Castle
      if(board[i][j].unit == Unit::KING){
        vec<point> t = {{-1, -1}, {-1, 0}, {-1, 1},
            {1, -1}, {1, 0}, {1, 1}, {0, -1}, {0, 1}};
        for(point tt : t){
          int ii = i + tt.x;
          int jj = j + tt.y;
          if(!in_bounds(point(ii, jj)) || board[ii][jj].player == p)
            continue;
          m.pb(Move(point(i, j), point(ii, jj))); } } }

  if(!filter)
    return m;
  // Can't move into check
  vec<Move> mf;
  for(const Move& t : m){
    if(board[t.src.xi()][t.src.yi()].player != turn)
      continue;
    Player ENEMY = (turn == Player::WHITE) ? Player::BLACK : Player::WHITE;
    bool chk = false;

    // Moving king
    for(const Move& u : m){
      if(board[u.src.xi()][u.src.yi()].player != ENEMY)
        continue;
      if(board[t.src.xi()][t.src.yi()].unit == Unit::KING && u.dest == t.dest){
        chk = true;
        break; } }
    if(chk)
      continue;

    // Moving another piece
    Board b = *this;
    b.move(t);
    vec<Move> m2 = b.moves(false);
    for(const Move& u : m2){
      if(b.board[u.src.xi()][u.src.yi()].player != ENEMY)
        continue;
      if(b.board[u.dest.xi()][u.dest.yi()].unit == Unit::KING){
        chk = true;
        break; } }
    if(!chk)
      mf.pb(t); }
  return mf; }

// Move a piece
void Board::move(const Move& m){
  board[m.dest.xi()][m.dest.yi()] = board[m.src.xi()][m.src.yi()];
  board[m.src.xi()][m.src.yi()] = Piece(Player::NONE, Unit::NONE);
  // Turn pawn to queen
  if(board[m.dest.xi()][m.dest.yi()].unit == Unit::PAWN
      && ((turn == Player::WHITE && m.dest.x == 7)
      || (turn == Player::BLACK && m.dest.x == 0)))
    board[m.dest.xi()][m.dest.yi()].unit = Unit::QUEEN;
  turn = (turn == Player::WHITE) ? Player::BLACK : Player::WHITE; }

// Initialize game
void Chess::init_game(){
  board.turn = Player::WHITE;
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
