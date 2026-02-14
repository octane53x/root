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

  // int indent = 0;
  // bool quote = false;
  // for(int i = 1; i < 50000; ++i){
  //   if(text[i] == '"' && text[i-1] != '\\')
  //     quote = !quote;
  //   if(text[i] == '{' && !quote){
  //     indent += 2;
  //     printf("\n");
  //     for(int j = 0; j < indent; ++j)
  //       printf(" ");
  //     continue; }
  //   if(text[i] == '}' && !quote){
  //     indent -= 2;
  //     printf("\n");
  //     for(int j = 0; j < indent; ++j)
  //       printf(" ");
  //     continue; }
  //   if(text[i] == ',' && !quote){
  //     printf("\n");
  //     for(int j = 0; j < indent; ++j)
  //       printf(" ");
  //     continue; }
  //   printf("%c", text[i]); }
  // printf("\n");

  // Parse cards
  umap<str, Card> cards;
  int i = 0;

  while(1){
    i = find(text, "\"cards\": [", i);
    if(i == -1) break;
    i += 10;

    while(1){
      assert(text[i] == '{', "parse", "bad char 1");
      ++i;

      while(1){
        assert(text[i] == '"', "parse", "bad char 2");
        ++i;
        bool found = false
        int j = i;

        while(!found){
          j = find(text, "\"", j);
          if(text[j - 1] != '\\')
            found = true;
          else
            ++j; }

        str key = text.substr(i, j - i);
        i = j + 1;
        assert(text[i] == ':', "parse", "bad char 3");
        i += 2;

        //! value
      }
    }
  }
  return cards;
}

#endif
