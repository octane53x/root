// EDITOR UTIL

#ifndef EDITOR_UTIL_HH
#define EDITOR_UTIL_HH

#include "../gl/image.hh"

typedef umap<char, image> font;

const int
    WIDTH_OFFSET = -16,
    HEIGHT_OFFSET = -38,
    WINDOW_OFFSET = 10,
    PANEL_OFFSET = 12,
    LINE_HEIGHT_SCALE_1 = 18,
    CHAR_WIDTH_SCALE_1 = 9,
    VERTICAL_DIVIDE = 20,
    PANEL_CHARS = 80;

const double
    SCALE_FACTOR = 1.1,
    CURSOR_BLINK = 0.5;

const color
    BKGD_COLOR = BLACK,
    CMD_BAR_COLOR = RED,
    FOCUS_FILE_BAR_COLOR = LTGRAY,
    UNFOCUS_FILE_BAR_COLOR = DKGRAY,
    BAR_TEXT_COLOR = BLACK,
    SELECT_COLOR = BLUE,
    CURSOR_COLOR = CYAN;

const str
    _FONT_LOC = "../symbols.png",
    _SYMBOLS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        " .,/?:;'\"+=-_\\|~!@#$%^&*()[]{}<>";

enum Dir { UP, LEFT, DOWN, RIGHT };

bool name_or_val(const char c) const {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
      || (c >= 'a' && c <= 'z') || c == '_'; }

#endif
