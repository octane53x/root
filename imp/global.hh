// Impact global variables

#ifndef GLOBAL_HH
#define GLOBAL_HH

#include "impact.hh"

// Time execution began
clock_t time_exec;
// Global object to process inputs and call game and graphics functions
Impact imp;

// Static members
int scene::win_w, scene::win_h;
umap<str, font> scene::fonts;

#endif
