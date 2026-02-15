// MTG CARD

#ifndef CARD_HH
#define CARD_HH

#include "../core/util.hh"

const uset<str> MODS {
  "flying", "defender", "trample", "deathtouch", "haste", "lifelink", "reach",
  "first strike", "double strike", "flash", "hexproof", "indestructible",
  "menace", "vigilance" };

struct Card {

  struct Cost {

    enum CostType {
      NONE, COLOR, GENERIC, HYBRID_2COL, HYBRID_2GEN, PHYREXIAN, SNOW, VARIABLE
    } type;

    char color, color2;
    int count;

    Cost(); };

  // Base data
  int pow, tuf;
  str name, text;
  vec<Cost> cost;
  uset<str> types;

  // Abilities from text
  uset<str> mods;

  // Game data
  int p1p1cntr;

  Card();

  bool is_color(const char c) const;
  str next_tok(const int i) const;
  void print() const;

  bool add_field(const str& key, const str& val);
  void condense_cost();
  bool process_text(); };

Card::Cost::Cost(): type(NONE), color('-'), color2('-'), count(0) {}

Card::Card(): pow(0), tuf(0) {}

bool Card::is_color(const char c) const {
  return (c == 'W' || c == 'U' || c == 'K' || c == 'R' || c == 'G'
      || c == 'C'); }

str Card::next_tok(const int i) const {
  int j = i;
  while(is_alpha(text[j]))
    ++j;
  if(j == i)
    ++j;
  return text.substr(i, j - i); }

void Card::print() const {
  printf("%s\n", name.c_str());
  for(const Cost& c : cost){
    if(c.type == Cost::GENERIC)
      printf("%d ", c.count);
    else if(c.type == Cost::COLOR)
      printf("%d%c ", c.count, c.color);
    else if(c.type == Cost::VARIABLE)
      printf("%dX ", c.count);
    else
      printf("%d# ", c.count); }
  if(!cost.empty())
    printf("\n");
  if(contains(types, str("Creature")))
    printf("%d / %d\n", pow, tuf);
  for(const str& t : types)
    printf("%s ", t.c_str());
  printf("\n%s\n", text.c_str()); }

bool Card::add_field(const str& key, const str& val){
  if(key == "power"){
    if(is_integer(val))
      pow = stoi(val);
    else
      pow = -1;
  }else if(key == "toughness"){
    if(is_integer(val))
      tuf = stoi(val);
    else
      tuf = -1;
  }else if(key == "name"){
    name = val;
  }else if(key == "text"){
    text = val;
  }else if(key == "types" || key == "subtypes"){
    vec<str> sv = split(val.substr(1, val.size() - 2), ", ");
    for(const str& s : sv)
      types.insert(s.substr(1, s.size() - 2));

  }else if(key == "manaCost"){
    for(int i = 0; i < val.size(); ++i){
      if(val[i] != '{') continue;
      int j;
      for(j = i + 1; val[j] != '}'; ++j);
      str s = val.substr(i + 1, j - i - 1);
      for(j = 0; j < s.size(); ++j)
        if(s[j] == 'B')
          s[j] = 'K';

      Cost c;
      if(is_integer(s)){
        c.type = Cost::GENERIC;
        c.count = stoi(s);

      }else if(s.size() == 1){
        if(s == "X")
          c.type = Cost::VARIABLE;
        else if(is_color(s[0])){
          c.type = Cost::COLOR;
          c.color = s[0];
        }else if(s == "S")
          c.type = Cost::SNOW;
        else
          return false;
        c.count = 1;

      }else if(s.size() == 3 && s[1] == '/'){
        if(s[0] == '2' && is_color(s[2])){
          c.type = Cost::HYBRID_2GEN;
          c.color = s[2];
        }else if(s[2] == 'P' && is_color(s[0])){
          c.type = Cost::PHYREXIAN;
          c.color = s[0];
        }else if(is_color(s[0]) && is_color(s[2])){
          c.type = Cost::HYBRID_2COL;
          c.color = s[0];
          c.color2 = s[2];
        }else
          return false;
        c.count = 1;

      }else if(s.size() == 5 && s[1] == '/' && s[3] == '/'){
        if(s[4] == 'P' && is_color(s[0]) && is_color(s[2])){
          c.type = Cost::PHYREXIAN;
          c.color = s[0];
          c.color2 = s[2];
          c.count = 1;
        }else
          return false;

      }else
        return false;

      cost.pb(c); }
    condense_cost(); }
  return true; }

void Card::condense_cost(){
  vec<Cost> cn;
  while(!cost.empty()){
    Cost c = cost[0];
    for(int i = 1; i < cost.size(); ++i)
      if(c.type == cost[i].type
          && ((c.color == cost[i].color && c.color2 == cost[i].color2)
          || (c.color == cost[i].color2 && c.color2 == cost[i].color))){
        c.count += cost[i].count;
        cost.erase(cost.begin() + i);
        --i; }
    cn.pb(c);
    cost.erase(cost.begin()); }
  cost = cn; }

bool Card::process_text(){
  int i = 0;
  while(i < text.size()){
    str tok = next_tok(i);
    if(contains(MODS, to_lower(tok))){
      mods.insert(to_lower(tok));
      i += tok.size();
    }else if(tok == "," || tok == " ")
      ++i;
    else
      return false; }
  return true; }

#endif
