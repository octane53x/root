// PARSE CARDS

#ifndef PARSE_HH
#define PARSE_HH

#include "card.hh"

const str CARDS_JSON = "C:/home/data/AllPrintings.json";

int find(const str& text, const str& tok, int pos){
  while(pos < text.size()){
    int i = pos, j = 0;
    while(text[i] == tok[j])
      ++i, ++j;
    if(j == tok.size()) return pos;
    ++pos; }
  return -1; }

umap<str, Card> parse(){
  // Read file
  ifstream fs(CARDS_JSON);
  str text;
  getline(fs, text);

  // Parse cards
  umap<str, Card> cards;
  int i = 0;

  while(1){
    i = find(text, "\"cards\": [", i);
    if(i == -1) break;
    i += 10;

    while(1){
      if(text[i] == ']')
        break;
      assert(text[i] == '{', "parse", "bad char 1");
      ++i;
      bool keep = true;
      Card card;

      while(1){
        assert(text[i] == '"', "parse", "bad char 2");
        ++i;
        bool found = false;
        int j = i;

        while(!found){
          j = find(text, "\"", j);
          if(text[j - 1] != '\\')
            found = true;
          else
            ++j; }

        str key = text.substr(i, j - i), val;
        i = j + 1;
        assert(text[i] == ':', "parse", "bad char 3");
        i += 2;

        if(text[i] == '"'){
          ++i;
          found = false;
          j = i;

          while(!found){
            j = find(text, "\"", j);
            if(text[j - 1] != '\\' || text[j - 2] == '\\')
              found = true;
            else
              ++j; }

          val = text.substr(i, j - i);
          i = j + 1;

        }else if(text[i] == '{' || text[i] == '['){
          char c0 = text[i], cf;
          if(text[i] == '{')
            cf = '}';
          else
            cf = ']';
          int b = 1;
          bool quote = false;
          j = i + 1;

          while(b > 0){
            if(text[j] == '"' && (text[j - 1] != '\\' || text[j - 2] == '\\'))
              quote = !quote;
            if(text[j] == c0 && !quote)
              ++b;
            else if(text[j] == cf && !quote)
              --b;
            ++j; }

          val = text.substr(i, j - i);
          i = j;

        }else{
          j = find(text, ",", i);
          val = text.substr(i, j - i);
          i = j; }

        if(!card.add_field(key, val))
          keep = false;
        if(text[i] == ',')
          i += 2;
        else if(text[i] == '}')
          break;
        else{
          err("parse", "bad char 5"); } }

      if(contains(cards, card.name)){
        if(card.name.find("//") != str::npos){
          card.name += "(2)";
          cards[card.name] = card; }
      }else if(keep)
        cards[card.name] = card;

      ++i;
      if(text[i] == ',')
        i += 2;
      else if(text[i] == ']')
        break;
      else
        err("parse", "bad char 6"); } }

  return cards; }

#endif
