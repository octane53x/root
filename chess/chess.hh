// CHESS

#ifndef CHESS_HH
#define CHESS_HH

#include "../os/win/application.hh"

// Pixel size of board square
const int SQUARE = 80;

// White or Black
enum class Player : uchar { NONE, WHITE, BLACK };
// Piece type
enum class Unit : uchar { NONE, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

// Chess piece
struct Piece {

  // Player of piece
  Player player;
  // Type of piece
  Unit unit;

  Piece();
  Piece(Player p, Unit t);

  Piece& operator=(const Piece& o); };

// Chess move
struct Move {

  // Source and destination squares
  ipoint src, dest;

  Move(ipoint s, ipoint d); };

// Chess board
struct Board {

  // (0,0) bottom left
  Piece board[8][8];

  Board();

  Board& operator=(const Board& o);

  // Defined in game.hh
  bool in_bounds(ipoint loc) const;
  bool check(Player p) const;
  bool mate(Player p) const;
  bool stale() const;
  vec<Move> moves(Player p) const; };

// Chess game application
struct Chess : Application {

  // Selected piece by location, (-1,-1) if none
  ipoint select;
  // Whose turn it is
  Player turn;
  // Active board
  Board board;

  Chess();

  virtual void init();

  // Defined in draw.hh
  virtual void draw();
  void draw_board();
  void draw_moves();
  void draw_pieces();

  // Defined in input.hh
  virtual void map_fns();
  static bool click(const KeyEvent& ke);
  static bool kill(const KeyEvent& ke);

  // Defined in game.hh
  void init_game(); };

// Set default member state
Piece::Piece():
  player(Player::NONE), unit(Unit::NONE) {}

// Set default member state
Piece::Piece(Player p, Unit u):
  player(p), unit(u) {}

// Set default member state
Move::Move(ipoint s, ipoint d):
  src(s), dest(d) {}

// Set default member state
Board::Board(){}

// Set default member state
Chess::Chess(){}

// Assignment operator
Piece& Piece::operator=(const Piece& o){
  player = o.player;
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
  win_pos = ipoint(0, 0);
  frame.size = ipoint(SQUARE * 8, SQUARE * 8);
  select = ipoint(-1, -1);
  init_game(); }

#endif
