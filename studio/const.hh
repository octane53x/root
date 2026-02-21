// STUDIO CONSTANTS

#ifndef STUDIO_CONST_HH
#define STUDIO_CONST_HH

#include "../gl/image.hh"

typedef umap<char, image> font;

const int
    // Pixels
    FRAME_HEIGHT_OFFSET = -31,
    WIN_WIDTH_OFFSET = 15,
    WIN_HEIGHT_OFFSET = 39,
    WIN_XPOS_OFFSET = 7,
    LINE_HEIGHT_SCALE_1 = 18,
    CHAR_WIDTH_SCALE_1 = 9,
    VERTICAL_DIVIDE = 20,
    PANEL_CHARS = 80;

const double
    SCALE_FACTOR = 1.1,
    // Seconds
    CURSOR_BLINK = 0.5;

const color
    BKGD_COLOR = BLACK,
    CMD_BAR_COLOR = RED,
    FOCUS_FILE_BAR_COLOR = LTGRAY,
    UNFOCUS_FILE_BAR_COLOR = DKGRAY,
    BAR_TEXT_COLOR = BLACK,
    DIVIDER_COLOR = DKDKGRAY,
    SELECT_COLOR = BLUE,
    CURSOR_COLOR = CYAN,

    // Highlighting
    COLOR_CODE = color(255, 255, 255),
    COLOR_KEYWORD = color(0, 235, 235),
    COLOR_TYPE = color(80, 255, 140),
    COLOR_NAME = color(255, 255, 100),
    COLOR_FUNCTION = color(100, 200, 255),
    COLOR_BASE = color(120, 255, 255),
    COLOR_STRING = color(255, 140, 140),
    COLOR_COMMENT = color(255, 92, 0),
    COLOR_PREPROCESSOR = color(160, 160, 255);

const vec<color> TEXT_COLORS =
    {COLOR_CODE, COLOR_KEYWORD, COLOR_TYPE, COLOR_NAME, COLOR_FUNCTION,
    COLOR_BASE, COLOR_STRING, COLOR_COMMENT, COLOR_PREPROCESSOR};

const str
    FONT_LOC = "edit/symbols.bmp",
    SYMBOLS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        " .,/?:;'\"+=-_\\|~!@#$%^&*()[]{}<>";

const uset<str>
    KEYWORDS_SHARED = {
    "if", "else", "for", "while", "return", "break", "continue"},
    KEYWORDS_CPP_CRAB = {
    "const", "virtual", "final", "this", "operator", "static",
    "template", "abstract", "force", "true", "false", "enum", "switch", "case",
    "try", "catch", "typedef", "enum", "namespace", "NULL"},
    KEYWORDS_CPP = {"struct", "class", "typename", "do", "new", "delete",
    "volatile", "override", "inline", "static_cast", "dynamic_cast",
    "reinterpret_cast", "public", "protected", "private"},
    KEYWORDS_PYTH = {"def", "import", "in", "and", "or", "elif", "as", "with",
    "True", "False", "None", "assert", "class", "del", "except", "finally",
    "from", "global", "is", "lambda", "not", "pass", "raise", "yield"},
    KEYWORDS_CRAB = {"obj", "fn", "include", "force"};

enum FileType { NO_FILE_TYPE, CPP, PYTH, CRAB };

enum Dir { UP, LEFT, DOWN, RIGHT };

#endif
