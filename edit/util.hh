// EDITOR UTIL

#ifndef EDITOR_UTIL_HH
#define EDITOR_UTIL_HH

#include "../gl/image.hh"

typedef umap<char, image> font;

const int
    FRAME_HEIGHT_OFFSET = -31,
    WIN_WIDTH_OFFSET = 15,
    WIN_HEIGHT_OFFSET = 43,
    WIN_XPOS_OFFSET = 7,
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
    DIVIDER_COLOR = DKDKGRAY,
    SELECT_COLOR = BLUE,
    CURSOR_COLOR = CYAN,

    // Highlighting
    COLOR_CODE = color(255, 255, 255),
    COLOR_KEYWORD = color(0, 208, 208),
    COLOR_TYPE = color(0, 208, 82),
    COLOR_NAME = color(242, 242, 82),
    COLOR_FUNCTION = color(120, 132, 176),
    COLOR_STRING = color(242, 64, 76),
    COLOR_COMMENT = color(255, 92, 0);

const vec<color> TEXT_COLORS =
    {COLOR_CODE, COLOR_KEYWORD, COLOR_TYPE, COLOR_NAME, COLOR_FUNCTION,
    COLOR_STRING, COLOR_COMMENT};

const str
    _FONT_LOC = "edit/symbols.bmp",
    _SYMBOLS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        " .,/?:;'\"+=-_\\|~!@#$%^&*()[]{}<>";

const uset<str> KEYWORDS = {
    // Crab
    "obj", "fn", "if", "else", "for", "while", "return", "break", "continue",
    "const", "virtual", "final", "include", "this", "operator", "static",
    "template", "abstract", "force", "true", "false", "enum", "switch", "case",
    "try", "catch", "typedef", "enum", "namespace",
    // Other languages
    "struct", "def", "delete", "in", "and", "or", "bitand", "bitor", "xor",
    "static_cast", "dynamic_cast", "typename", "class", "do", "inline", "new",
    "public", "protected", "private"};

enum Dir { UP, LEFT, DOWN, RIGHT };

bool name_or_val(const char c){
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
      || (c >= 'a' && c <= 'z') || c == '_'; }

bool in_selection(const ipoint& p0, const ipoint& pf, const ipoint& p){
  bool after_p0 = (p.y > p0.y || (p.y == p0.y && p.x >= p0.x));
  bool before_pf = (p.y < pf.y || (p.y == pf.y && p.x <= pf.x));
  return (after_p0 && before_pf); }

bool starts_with(const str& s, const str& t){
  if(t.size() > s.size()) return false;
  for(int i = 0; i < t.size(); ++i)
    if(s[i] != t[i]) return false;
  return true; }

bool is_upper(const char c){
  return (c >= 'A' && c <= 'Z'); }

bool is_lower(const char c){
  return (c >= 'a' && c <= 'z'); }

bool is_alpha(const char c){
  return is_upper(c) || is_lower(c); }

bool is_digit(const char c){
  return (c >= '0' && c <= '9'); }

str strip(const str& s){
  int i = 0;
  while(i < s.size() && s[i] == ' ')
    ++i;
  int j = s.size() - 1;
  while(j >= 0 && s[j] == ' ')
    --j;
  if(i <= j)
    return s.substr(i, j - i + 1);
  return ""; }

str next_tok(const str& s){
  if(s == "") return "";
  int i = 0;
  if(is_alpha(s[0]) || is_digit(s[0]) || s[0] == '_')
    while(i < s.size() && (is_alpha(s[i]) || is_digit(s[i]) || s[i] == '_'))
      ++i;
  else if(s[0] == '(' || s[0] == ')' || s[0] == '[' || s[0] == ']'
      || s[0] == '{' || s[0] == '}')
    ++i;
  else
    while(!is_alpha(s[i]) && !is_digit(s[i]) && s[i] != '_'
        && s[i] != '(' && s[i] != ')' && s[i] != '[' && s[i] != ']'
        && s[i] != '{' && s[i] != '}')
      ++i;
  return s.substr(0, i); }

str delete_tok(const str& s){
  str tok = next_tok(s);
  return s.substr(tok.size()); }

#endif
