// CHESS

#ifndef CHESS_HH
#define CHESS_HH

#include "../os/win/application.hh"

// White or Black
enum class Player : uchar { NONE, WHITE, BLACK };
// Piece type
enum class Unit : uchar { NONE, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

// Chess piece
struct Piece {

  // Owner of piece
  Player owner;
  // Type of piece
  Unit unit;

  Piece();
  Piece(Player p, Unit t);

  Piece& operator=(const Piece& o); };

// Chess move
struct Move {

  // Source and destination squares
  pair<int, int> src, dest;

  Move(pair<int, int> s, pair<int, int> dest); };

// Chess board
struct Board {

  // (0,0) bottom left
  Piece board[8][8];

  Board();

  Board& operator=(const Board& o);

  // Defined in game.hh
  bool in_bounds(pair<int, int> loc) const;
  bool check(Player p) const;
  bool mate(Player p) const;
  bool stale() const;
  vec<Move> moves(Player p) const; };

// Chess game application
struct Chess : Application {

  // Whose turn it is
  Player turn;
  Board board;

  Chess();

  virtual void init();
  virtual void update();

  // Defined in draw.hh
  virtual void draw();
  void draw_board();
  void draw_pieces();

  // Defined in input.hh
  virtual void map_fns();
  static bool click(const KeyEvent& ke);

  // Defined in game.hh
  void init_game(); };

// Set default member state
Piece::Piece():
  owner(Player::NONE), unit(Unit::NONE) {}

// Set default member state
Piece::Piece(Player p, Unit u):
  owner(p), unit(u) {}

// Set default member state
Move::Move(pair<int, int> s, pair<int, int> d):
  src(s), dest(d) {}

// Set default member state
Board::Board(){}

// Set default member state
Chess::Chess(){}

// Assignment operator
Piece& Piece::operator=(const Piece& o){
  owner = o.owner;
  unit = o.unit;
  return *this; }

// Assignment operator
Board& Board::operator=(const Board& o){
  for(int i = 0; i < 8; ++i)
    for(int j = 0; j < 8; ++j)
      board[i][j] = o.board[i][j];
  return *this; }

// Initialize application
void Chess::init(){
  Application::init();
  start_maximized = false;
  win_pos = ipoint(50, 50);
  win_size = ipoint(800, 800);
  init_game(); }

// Update -> draw if updated -> repeat
void Chess::update(){
  Application::update();

}

#endif
