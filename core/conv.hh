// CORE OBJECT CONVERSIONS

#ifndef CONV_HH
#define CONV_HH

#include "str.hh"

const int BITS_PER_HEX = 4;

enum class PrintMode { BIT, HEX, VAL, STRUCTURE };

Str Byte::to_str(const PrintMode mode){
  Str r;
  switch(mode){
    case PrintMode::BIT:
      for(int s = BITS - 1; s >= 0; --s)
        r += Str((data >> s) & 1);
      break;

    case PrintMode::HEX:
      mask = 0xFF;
      for(int s = BITS - BITS_PER_HEX; s >= 0; s -= BITS_PER_HEX){
        uchar h = ((data >> s) & 0xFF);
        r += Str((h > 9) ? ('A' + h - 9) : h); }
      break;

    case PrintMode::VAL:
    case PrintMode::STRUCTURE:
    case default:
      err("Byte.to_str()", "Bad PrintMode"); } }

Str Block::to_str(const PrintMode mode){
  Str r;
  const ull mask = 0xFF;
  for(int s = BITS - Byte::BITS; s >= 0; s -= Byte::BITS)
    r += Byte((uchar)((data >> s) & mask)).to_str(mode);
  return r; }

#endif
