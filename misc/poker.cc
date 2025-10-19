// POKER

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define vec std::vector
#define pb push_back

const vec<char> SUIT = {'C', 'D', 'S', 'H'};
const vec<char> RANK =
    {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

struct Card {
  char suit, rank;
  Card(char r, char s): suit(s), rank(r) {} };

struct Deck {
  vec<Card> cards;
  Deck(){
    for(char s : SUIT)
      for(char r : RANK)
        cards.pb(Card(s, r));
    shuffle(); }
  Card draw(){
    Card r = cards.back();
    cards.pop_back();
    return r; }
  void shuffle(){
    vec<Card> t;
    while(!cards.empty()){
      int r = rand() % cards.size();
      vec<Card>::iterator it = cards.begin() + r;
      t.pb(*it);
      cards.erase(it); }
    cards = t; } };

struct Hand {
  vec<Card> cards;
  vec<int> ranks, ranks_by_suit[4];
  Hand(){}

  void add(Card card){
    cards.pb(card);
    umap<char, int>::iterator it;
    it = ranks.find(card.rank);
    if(it == m.end()) ranks[card.rank] = 1;
    else ++it->second;
    it = suits.find(card.suit);
    if(it == m.end()) suits[card.suit] = 1;
    else ++it->second; }

  void discard(Card card){
    bool found = false;
    vec<Card>::iterator it;
    for(it = cards.begin(); it != cards.end(); ++it)
      if(it->rank == card.rank && it->suit == card.suit){
        cards.erase(it);
        found = true;
        break; }
    if(!found) return;
    umap<char, int>::iterator it2;
    it2 = ranks.find(card.rank);
    if(it2->second == 1) ranks.erase(it2);
    else --it2->second;
    it2 = suits.find(card.rank);
    if(it2->second == 1) suits.erase(it2);
    else --it2->second; }

  // Returns how many of n of a kind
  int kind(int n){
    int r = 0;
    umap<char, int>::iterator it;
    for(it = ranks.begin(); it != ranks.end(); ++it)
      if(it->second >= n) ++r;
    return r; }

  // Returns max length straight
  int straight(){
    int r = 0;
    for(int i = 0; i < RANK.size(); ++i){
      int len = 0;
      for(j = i; j < RANK.size(); ++j){
        if(ranks.find(RANK[j]) == ranks.end()) break;
        else ++len; }
      if(len > r) r = len; }
    // Straight starting with ace
    if(ranks.find('A') != ranks.end()){
      int len = 1;
      for(j = 0; j < RANK.size(); ++j){
        if(ranks.find(RANK[j]) == ranks.end()) break;
        else ++len; }
      if(len > r) r = len; }
    return r; }

  // Returns max size flush
  int flush(){
    int r = 0;
    umap<char, int>::iterator it;
    for(it = suits.begin(); it != suits.end(); ++it)
      if(it->second > r) r = it->second;
    return r; }

  bool full_house(){
    bool found = false;
    umap<char, int>::iterator it, it2;
    for(it = ranks.begin(); it != ranks.end(); ++it){
      if(it->second >= 3){
        found = true;
        break; } }
    if(!found) return false;
    for(it2 = ranks.begin(); it2 != ranks.end(); ++it2){
      if(it == it2) continue;
      if(it->second >= 2) return true; }
    return false; }

  // Returns max length straight flush
  int straight_flush(){

  }
};

int main(){
  Deck deck;
  Hand hand;
  hand.add(Card('A', 'S'));
  hand.add(Card('K', 'S'));
  hand.add(Card('K', 'C'));
  hand.add(Card('J', 'S'));
  hand.add(Card('J', 'C'));
  printf("%d\n", hand.kind(2));
  return 0; }
