// Impact global variables

#ifndef GLOBAL_HH
#define GLOBAL_HH

#include "impact.hh"

// Global variables accessed by UI inputs
clock_t time_exec;
Impact imp;

// Static members
int scene::win_w, scene::win_h;
umap<str, font> scene::fonts;

#endif
