// CONSTANTS

#ifndef CONST_HH
#define CONST_HH

#include "str.hh"

const int
    // Infinity
    INF = INT_MAX,
    // Divide (clock() - last_clock) by this
    CPS = CLOCKS_PER_SEC;

const double
    // Infinity for double
    INFD = DBL_MAX,
    // Margin of error for comparing doubles
    SAFE_ZERO = 0.00000001,
    // Mathematical pi
    PI = 3.14159265358979323846;

const str
    // Debug log file location, print goes here when there's no console
    DEBUG_FILE = "c:/home/root/debug.txt";

#endif
