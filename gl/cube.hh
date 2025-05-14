// CUBE

#ifndef CUBE_HH
#define CUBE_HH

#include "gl_obj.hh"

const llu INIT_SIZE = 1000000000000000000LLU;

struct cube {
  llu size;
  cube* outer, *T, *N, *W, *E, *S, *B,
               *TNW, *TNE, *TSW, *TSE, *BNW, *BNE, *BSW, *BSE;
  cube** inner, **side;
  cube(): size(INIT_SIZE) {}
  void split(){
    inner = new cube*[8];
    // Link handles
    TNW = inner[0], TNE = inner[1], TSW = inner[2], TSE = inner[3],
    BNW = inner[4], BNE = inner[5], BSW = inner[6], BSE = inner[7];
    // Connect inner
    TNW->E = TNE, TNW->S = TSW, TNW->B = BNW;
    TNE->W = TNW, TNE->S = TSE, TNE->B = BNE;
    TSW->E = TSE, TSW->N = TNW, TSW->B = BSW;
    TSE->W = TSW, TSE->N = TNE, TSE->B = BSE;
    BNW->E = BNE, BNW->S = BSW, BNW->T = TNW;
    BNE->W = BNW, BNE->S = BSE, BNE->T = TNE;
    BSW->E = BSE, BSW->N = BNW, BSW->T = TSW;
    BSE->W = BSW, BSE->N = BNE, BSE->T = TSE;
    //! Connect sides and inner to adjacent
  }
};

#endif
