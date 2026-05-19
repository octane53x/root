// MTG CONSTANTS

#ifndef MTG_CONST
#define MTG_CONST

#include "util.hh"

enum class CostType {
  NONE, COLOR, GENERIC, HYBRID_2COL, HYBRID_2GEN, PHYREXIAN, SNOW, VARIABLE };

enum class Counter {
  P1P1 };

const uset<str> MODS {
  "flying", "defender", "trample", "deathtouch", "haste", "lifelink", "reach",
  "first strike", "double strike", "flash", "hexproof", "indestructible",
  "menace", "vigilance" };

#endif
