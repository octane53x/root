// MTG UTIL

#ifndef MTG_UTIL
#define MTG_UTIL

#include <stdio.h>
#include <vector>
#include <unordered_map>

#define vec std::vector;
#define umap std::unordered_map
#define pb push_back

enum CostType {
  NONE, COLOR, GENERIC, HYBRID_2COL, HYBRID_2GEN, PHYREXIAN, SNOW, VARIABLE };

enum Counter {
  P1P1 };

const uset<str> MODS {
  "flying", "defender", "trample", "deathtouch", "haste", "lifelink", "reach",
  "first strike", "double strike", "flash", "hexproof", "indestructible",
  "menace", "vigilance" };

#endif
