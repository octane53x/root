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
    color c = (i & 1) ? WHITE : BLACK;
    for(int j = 0; j < 8; ++j){
      polygon p;
      p.add(ipoint(i * 100, j * 100));
      p.add(ipoint((i + 1) * 100 - 1, j * 100));
      p.add(ipoint((i + 1) * 100 - 1, (j + 1) * 100 - 1));
      p.add(ipoint(i * 100, (j + 1) * 100 - 1));
      p.fill = c;
      p.draw(&frame, viewport()); } } }

void Chess::draw_pieces(){

}

#endif
