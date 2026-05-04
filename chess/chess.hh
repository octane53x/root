// CHESS

#ifndef CHESS_HH
#define CHESS_HH

#include "../os/win/application.hh"

// Pixel size of board square
const int SQUARE = 80;

// Project pointer for static input functions, set in constructor
struct Chess;
Chess* _app;

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
  point src, dest;

  Move();
  Move(point s, point d); };

// Chess board
struct Board {

  // Whose turn it is
  Player turn;
  // (0,0) bottom left
  Piece board[8][8];

  Board();

  Board& operator=(const Board& o);

  // Defined in game.hh
  bool in_bounds(point loc) const;
  bool check(Player p) const;
  bool mate(Player p) const;
  bool stale() const;
  vec<Move> moves(bool filter) const;
  void move(const Move& m); };

// Chess game application
struct Chess : Application {

  // Selected piece by location, (-1,-1) if none
  point select;
  // Active board
  Board board;

  Chess();

  virtual void init();
  virtual void update();

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
  void init_game();

  // Defined in bot.hh
  int score(const Board& b) const;
  double delve(const Board& b, int depth, bool mx) const;
  void bot_move(); };

// Set default member state
Piece::Piece():
  player(Player::NONE), unit(Unit::NONE) {}

// Supply members
Piece::Piece(Player p, Unit u):
  player(p), unit(u) {}

// Set default member state
Move::Move(){}

// Supply members
Move::Move(point s, point d):
  src(s), dest(d) {}

// Set default member state
Board::Board(){}

// Set default member state
Chess::Chess(){
  _app = this; }

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
  win_pos = point(0, 0);
  frame.size = point(SQUARE * 8, SQUARE * 8);
  select = point(-1, -1);
  init_game(); }

// Update -> Draw -> Repeat
void Chess::update(){
  Application::update();
  if(board.turn == Player::BLACK)
    bot_move(); }

#endif
