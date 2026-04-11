// MTG DECK

#ifndef DECK_HH
#define DECK_HH

#include "card.hh"

const int
    MIN_CARDS = 60,
    MAX_SAME_CARD = 4;

struct Deck {

  struct CardData {

    Card card;
    int count, draws, points;

    CardData();

    double score() const; };

  umap<str, CardData> cards;

  double score() const;

  void fill(const vec<Card>& card_list);
  void replace(const vec<Card>& card_list, int n); };

// Set default member state
Deck::CardData::CardData(): count(0), draws(0), points(0) {}

// Get card score relative to deck
double Deck::CardData::score() const {
  return (draws == 0) ? 0 : (double)points / draws; }

// Get deck score from games played
double Deck::score() const {
  double r = 0.0;
  for(const pair<str, CardData>& e : cards)
    for(int i = 0; i < e.second.count; ++i)
      r += e.second.score();
  return r; }

// Fill the deck with random cards
void Deck::fill(const vec<Card>& card_list){
  int n = 0;
  for(pair<str, CardData> e : cards)
    n += e.second.count;
  for(int i = 0; i < MIN_CARDS - n; ++i){
    Card c = card_list[lrand() % card_list.size()];
    if(contains(cards, c.name)){
      if(cards[c.name].count == MAX_SAME_CARD){
        --i;
        continue;
      }else
        ++cards[c.name].count;
    }else{
      CardData cd;
      cd.card = c;
      cd.count = 1;
      cards[c.name] = cd; } } }

// Remove the n least effective sets of cards and refill deck
void Deck::replace(const vec<Card>& card_list, const int n){
  for(int i = 0; i < n; ++i){
    if(cards.empty()) break;
    double m = INFD;
    str pick;
    for(pair<str, CardData> e : cards)
      if(e.second.score() < m){
        m = e.second.score();
        pick = e.first; }
    cards.erase(pick); }
  fill(card_list); }

#endif
