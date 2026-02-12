// EDITOR UTIL

#ifndef EDITOR_UTIL_HH
#define EDITOR_UTIL_HH

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
    _FONT_LOC = "edit/symbols.bmp",
    _SYMBOLS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        " .,/?:;'\"+=-_\\|~!@#$%^&*()[]{}<>";

const uset<str> KEYWORDS = {
    // Crab
    "obj", "fn", "if", "else", "for", "while", "return", "break", "continue",
    "const", "virtual", "final", "include", "this", "operator", "static",
    "template", "abstract", "force", "true", "false", "enum", "switch", "case",
    "try", "catch", "typedef", "enum", "namespace", "NULL",
    // Other languages
    "struct", "def", "delete", "in", "and", "or", "bitand", "bitor", "xor",
    "static_cast", "dynamic_cast", "typename", "class", "do", "inline", "new",
    "public", "protected", "private"};

enum FileType { NO_FILE_TYPE, CPP, PYTH, CRAB };

enum Dir { UP, LEFT, DOWN, RIGHT };

str clean_path(const str& s){
  str r = s;
  for(int i = 0; i < s.size(); ++i)
    if(r[i] == '\\')
      r[i] = '/';
  if(r.back() != '/')
    r += "/";
  return r; }

bool in_selection(const ipoint& p0, const ipoint& pf, const ipoint& p){
  bool after_p0 = (p.y > p0.y || (p.y == p0.y && p.x >= p0.x));
  bool before_pf = (p.y < pf.y || (p.y == pf.y && p.x <= pf.x));
  return (after_p0 && before_pf); }

bool starts_with(const str& s, const str& t){
  if(t.size() > s.size()) return false;
  for(int i = 0; i < t.size(); ++i)
    if(s[i] != t[i]) return false;
  return true; }

bool ends_with(const str& s, const str& t){
  if(t.size() > s.size()) return false;
  for(int i = t.size() - 1, j = s.size() - 1; i >= 0; --i, --j)
    if(s[j] != t[i]) return false;
  return true; }

bool is_upper(const char c){
  return (c >= 'A' && c <= 'Z'); }

bool is_lower(const char c){
  return (c >= 'a' && c <= 'z'); }

bool is_alpha(const char c){
  return is_upper(c) || is_lower(c); }

bool is_digit(const char c){
  return (c >= '0' && c <= '9'); }

bool name_or_val(const char c){
  return is_alpha(c) || is_digit(c) || c == '_'; }

bool type_or_name(const str& s){
  if(!(is_alpha(s[0]) || s[0] == '_')) return false;
  for(int i = 1; i < s.size(); ++i)
    if(!name_or_val(s[i])) return false;
  return true; }

bool all_spaces(const str& s){
  int spaces = 0;
  for(; spaces < s.size() && s[spaces] == ' '; ++spaces);
  return spaces == s.size(); }

str next_tok(const str& s){
  if(s == "") return "";
  uset<str> ops =
      {"//", "/*", "*/", "::", "->", "==", "!=", ">=", "<=", "&&", "||",
      "<<", ">>", "<<=", ">>=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^="};
  int i = 0;
  // Numeric constant
  if(is_digit(s[0])){
    while(i < s.size() && is_digit(s[i]))
      ++i;
  // Type or name
  }else if(is_alpha(s[0]) || s[0] == '_'){
    ++i;
    while(i < s.size() && name_or_val(s[i]))
      ++i;
  // Spaces
  }else if(s[0] == ' '){
    while(i < s.size() && s[i] == ' ')
      ++i;
  // Three char operator
  }else if(s.size() > 2 && ops.find(s.substr(0, 3)) != ops.end())
    i += 3;
  // Two char operator
  else if(s.size() > 1 && ops.find(s.substr(0, 2)) != ops.end())
    i += 2;
  // Otherwise return single character
  else
    ++i;
  return s.substr(0, i); }

str delete_tok(const str& s){
  str tok = next_tok(s);
  return s.substr(tok.size()); }

RECT get_rect(const ipoint& pos, const ipoint& size){
  RECT r;
  r.left = pos.x;
  r.top = pos.y;
  r.right = pos.x + size.x;
  r.bottom = pos.y + size.y;
  return r; }

#endif
