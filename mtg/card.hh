// CARD

#ifndef CARD_HH
#define CARD_HH

#include "../core/util.hh"

struct Cost {

  enum CostType {
    COLOR, GENERIC, HYBRID_2COL, HYBRID_2GEN, PHYREXIAN, SNOW, VARIABLE
  } type;

  char color, color2;
  int count;

  Cost(); };

struct Card {

  int pow, tuf;
  str name, text;
  vec<Cost> cost;
  uset<str> types;

  Card();

  void print();
  bool add_field(const str& key, const str& val);
  void condense_cost();
  void compare(const Card& c); };

Cost::Cost(): color('-'), color2('-'), count(0) {}

Card::Card(): pow(0), tuf(0) {}

void Card::print(){
  printf("%s\n", name.c_str());
  for(Cost& c : cost){
    if(c.type == Cost::GENERIC)
      printf("%d ", c.count);
    else if(c.type == Cost::COLOR)
      printf("%c ", c.color);
    else if(c.type == Cost::VARIABLE)
      printf("X "); }
  printf("\n");
  if(contains(types, "Creature"))
    printf("%d / %d\n", pow, tuf);
  for(const str& t : types)
    printf("%s ", t.c_str());
  printf("\n%s\n", text.c_str()); }

bool Card::add_field(const str& key, const str& val){
  if(key == "power"){
    try{
      pow = stoi(val);
    }catch(invalid_argument& e){
      pow = -1; }
  }else if(key == "toughness"){
    try{
      tuf = stoi(val);
    }catch(invalid_argument& e){
      tuf = -1; }
  }else if(key == "name"){
    name = val;
  }else if(key == "text"){
    text = val;

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
      try{
        c.type = Cost::GENERIC;
        c.count = stoi(s);

      }catch(invalid_argument& e){
        if(s.size() == 1){
          if(s == "X")
            c.type = Cost::VARIABLE;
          else if(s == "W" || s == "U" || s == "K" || s == "R" || s == "G"
              || s == "C"){
            c.type = Cost::COLOR;
            c.color = s[0];
          }else if(s == "S")
            c.type = Cost::SNOW;
          else
            return false;
          c.count = 1;

        }else if(s.size() == 3 && s[1] == '/'){
          // c.color = s[0];
          // if(s[2] == 'P')
          //   c.type = Cost::PHYREXIAN;
          // else{
          //   c.type = Cost::HYBRID_2COL

        }else{
          //printf("%s\n", s.c_str());
          //!
        }
      }

      cost.pb(c); }
    condense_cost();

  }else if(key == "types" || key == "subtypes"){
    //!
  } }

void Card::condense_cost(){
  //!
}

void Card::compare(const Card& c){
  //!
}

#endif
