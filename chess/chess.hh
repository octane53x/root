// CHESS

#ifndef CHESS_HH
#define CHESS_HH

#include "../core/util.hh"

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

  Piece(Player p, Type t); };

// Chess move
struct Move {

  // Source and destination squares
  pair<int, int> src, dest;

  Move(pair<int, int> s, pair<int, int> dest); };

// Chess game application
struct Chess : Application {

  // Whose turn it is
  Player turn;
  // (0,0) bottom left
  Piece board[8][8];

  virtual void init();
  virtual void update();

  // Defined in draw.hh
  virtual void draw();
  void draw_board();
  void draw_pieces();

  // Defined in input.hh
  virtual void map_fns();
  void click(const KeyEvent& ke);

  // Defined in game.hh
  bool in_bounds(pair<int, int> loc);
  bool is_mate(Player p);
  vec<Move> moves(Player p);
  void init_game(); };

// Set default member state
Piece::Piece(Player p, Unit u):
  owner(p), unit(u) {}

// Set default member state
Move::Move(pair<int, int> s, pair<int, int> d):
  src(s), dest(d) {}

// Initialize application
void Chess::init(){

  init_game();
}

// Update -> draw if updated -> repeat
void Chess::update(){

}

#endif
