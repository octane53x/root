// MTG DECK

#ifndef DECK_HH
#define DECK_HH

#include "card.hh"

const int
    NUM_CARDS = 60,
    MAX_SAME_CARD = 4;

struct Deck {

  struct CardData {

    Card card;
    int count, draws, points;

    CardData();

    double score() const; };

  umap<str, CardData> cards;
  vec<Card> pile;

  double score() const;

  void shuffle();
  Card draw();
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
  for(const auto& [key, cd] : cards)
    for(int i = 0; i < cd.count; ++i)
      r += cd.score();
  return r; }

// Shuffle the deck
void Deck::shuffle(){
  for(int i = pile.size() - 1; i > 0; --i){
    int j = lrand() % i;
    if(i == j)
      continue;
    Card c = pile[j];
    pile[j] = pile[i];
    pile[i] = c; } }

// Draw a card
Card Deck::draw(){
  Card c = pile.back();
  pile.pop_back();
  return c; }

// Fill the deck with random cards
void Deck::fill(const vec<Card>& card_list){
  int n = 0;
  for(const auto& [key, cd] : cards)
    n += cd.count;
  for(int i = 0; i < NUM_CARDS - n; ++i){
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
      cards[c.name] = cd; } }
  pile.clear();
  for(const auto& [key, cd] : cards)
    for(int i = 0; i < cd.count; ++i)
      pile.pb(cd.card); }

// Remove the n least effective sets of cards and refill deck
void Deck::replace(const vec<Card>& card_list, const int n){
  for(int i = 0; i < n; ++i){
    if(cards.empty()) break;
    double m = INFD;
    str pick;
    for(const auto& [key, cd] : cards)
      if(cd.score() < m){
        m = cd.score();
        pick = key; }
    cards.erase(pick); }
  fill(card_list); }

#endif
