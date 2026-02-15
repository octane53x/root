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
CardData::CardData(): count(0), draws(0), points(0) {}

// Get card score relative to deck
double Deck::CardData::score() const {
  return (double)points / draws; }

// Get deck score from games played
double Deck::score() const {
  double r = 0.0;
  for(const pair<str, CardData>& e : cards)
    for(int i = 0; i < e.second.count)
      r += e.second.score();
  return r; }

// Fill the deck with random cards
void Deck::fill(const vec<Card>& card_list){
  int n = 0;
  for(const pair<str, int>& e : cards)
    n += e.second;
  for(int i = 0; i < MIN_CARDS - n; ++i){
    Card c = card_list[lrand() % card_list.size()];
    if(contains(cards, c.name)){
      if(cards[c.name] == MAX_SAME_CARD){
        --i;
        continue;
      }else
        ++cards[c.name];
    }else
      cards[c.name] = 1;
    eff[c.name] = 0.0; } }

// Remove the n least effective sets of cards and refill deck
void Deck::replace(const vec<Card>& card_list, const int n){
  for(int i = 0; i < n; ++i){
    if(eff.empty()) break;
    double m = INFD;
    str pick;
    for(const pair<str, double>& e : eff)
      if(e.second < m){
        m = e.second;
        pick = e.first; }
    cards.erase(pick);
    eff.erase(pick); }
  fill(card_list); }

#endif
