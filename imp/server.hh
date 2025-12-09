// IMPACT SERVER

#ifndef IMP_SERVER
#define IMP_SERVER

#include "entity/unit/bot.hh"

const int
    // Number of network nodes held as the pool of targets for requests
    NUM_CONTACTS = 20,
    // Number of network nodes each request is sent to
    NUM_REQUEST_TARGETS = 3,
    // Number of times a node can fail to reply in time before being banned
    NUM_WARNINGS = 5;

const double
    // Time allowed without response until requests are sent to different nodes
    REPLY_PATIENCE = 0.1; // seconds

// The connection to the network that acts as a server, sending game updates
struct Server : virtual system {

  // A network request
  struct Request {
    // ID used to match replies
    llu id;
    // When the request was sent
    clock_t time;
    // Request message
    str msg;
    // IPs the request was sent to
    vec<str> targets; };

  // Unique user id, attached to a single login and profile
  llu user_id; // 0 if logged out
  // Request id to match replies
  llu next_request_id;
  // Message replies from the network, used to update the game
  queue<str> replies;
  // Pool of active network nodes to which requests can be sent
  // If requests fail to all, the user's network connection has gone down
  umap<str, int> contacts; // key = ip, val = port
  // Network nodes that failed to reply
  umap<str, int> warning;
  // Nodes that failed so much they will no longer be accepted to the pool
  uset<str> banned;
  // Request log, entries cleared once they are replied to
  umap<llu, Request> request_log;
  // Player data being constantly updated
  Bot* player;

  Server();

  virtual void validate(const str& func);
  virtual void update(const double ms);

  str request(const str& msg);
  void login(const str& user, const str& pass);
  void process_msg(const str& msg); };

// Set default member state
Server::Server(): user_id(0), next_request_id(1) {}

// Ensure valid state
void Server::validate(const str& func){
  system::validate(func);
  player->validate(func);
  assert(!(user_id == 0 && !replies.empty()),
      "Server logged out with pending messages"); }

// Process all network replies and keep contacts updated
// Called by: Impact.update
void Server::update(const double ms){
  system::update(ms);
  // Ping all contacts to ensure connectivity
  for(pair<str, int> contact : contacts)
    request(str("PING:")+contact.first+str(":")+to_string(contact.second));
  // Fill contacts
  if(contacts.size() < NUM_CONTACTS)
    request(str("CONTACTS:")+to_string(NUM_CONTACTS-contacts.size()));
  // Handle requests exceeding patience interval
  clock_t now = clock();
  umap<llu, Request>::iterator it;
  for(it = request_log.begin(); it != request_log.end()){
    Request req = it->second;
    double sec = (double)(now - req.time) / CLOCKS_PER_SEC;
    if(sec <= REPLY_PATIENCE){ ++it; continue; }
    // Delete request from log
    it = request_log.erase(it);
    // Resend request
    request(req.msg);
    // Increase warning or ban node
    for(str ip : req.targets){
      umap<str, int>::iterator it2 = warning.find(ip);
      if(it2 == warning.end()){
        warning[ip] = 1;
      }else if(it2->second == NUM_WARNINGS-1){
        warning.erase(it2);
        banned.insert(ip);
      }else
        ++it2->second; } }
  // Process replies
  while(!replies.empty()){
    process_msg(replies.front());
    replies.pop(); }
  validate("Server.update"); }

// Send a request to the network
// Called by: Server.*
void Server::request(const str& msg){
  Request r;
  r.id = next_request_id++;
  r.msg = msg;
  int found = 0;
  for(int i = 0; i < contacts.size(); ++i){
    r.targets.pb(contacts[i]);
    ++found;
    if(found == NUM_REQUEST_TARGETS) break; }
  r.time = clock();
  //! net/send
  request_log[r.id] = r;
  validate("Server.request"); }

// Send a login request
// Called by: Impact.init
void Server::login(const str& user, const str& pass){
  request(str("LOGIN:")+user+str(":")+pass);
  validate("Server.login"); }

// Process a reply from the network
// Called by: update
void Server::process_msg(const str& msg){
  //! Handle by msg
  //! Remove matching request from request_log
  validate("Server.process_msg"); }

#endif
