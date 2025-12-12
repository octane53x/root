// ABSTRACT OBJECT

// update does not call move as one could expect, because the container (scene)
// needs to move objects in rooted order, information the object does not have

#ifndef OBJECT_HH
#define OBJECT_HH

#include "../core/system.hh"
#include "point.hh"
#include "viewport.hh"

struct image;

// Object with position, inherited by something with an image to draw
// Held by a 2D or 3D scene, which updates and draws the objects
struct object : virtual system {

  // Holds movement data, used to automatically move an object
  struct movement {

    // Style of movement
    enum mov_pattern : uchar {
      // If the root moves, the object moves equally
      ROOT,
      // Follow a predetermined path loop
      PATH,
      // Circle around the root
      ORBIT,
      // Function called during movement so that roots can be handled
      CUSTOM
    } pat;

    // Current position index in path
    int path_pos;
    // Movement velocity
    double vel; // pixels/sec
    // Progress toward next path index
    double path_prog;
    // Axis of orbit
    uvec axis;
    // The predetermined path of movement
    vec<point> path;
    // Object to which this object's position is rooted
    object* root;

    movement(mov_pattern p); };

  // Coordinate position in the environment
  point pos;
  // The change in position last time the object moved
  point last_move;
  // Fill color
  color fill;
  // Movement data, optional to not take up memory unless needed
  movement* mov;

  object();

  virtual void validate(const str& func);
  virtual void draw(image* canvas, const viewport& view) = 0;

  point move(const double ms); };

// Set default member state
object::object(): fill(DEFAULT_COLOR), mov(NULL) {
  type = "object"; }

// Construct with movement pattern
object::movement::movement(mov_pattern p):
    pat(p), vel(0.0), path_pos(0), path_prog(0.0), root(NULL) {}

// Ensure valid state
void object::validate(const str& func){
  system::validate(func);
  if(mov != NULL &&
      (mov->pat == movement::ROOT || mov->pat == movement::ORBIT))
    assert(mov->root != NULL, func, "movement.root not set"); }

// Move the object along its movement pattern
// Called by: scene.move_rec
point object::move(const double ms){
  point move = point(0, 0);
  if(mov == NULL || mov->pat == movement::ROOT) return move;
  if(mov->pat == movement::CUSTOM) return move; //! impl
  double dist = mov->vel * (ms / 1000.0);
  if(mov->pat == movement::PATH){
    while(dist > 0.0){
      point p = mov->path[mov->path_pos];
      double step = p.dist(point(0, 0));
      double next = step - mov->path_prog;
      if(dist < next){
        move += p * (dist / step);
        mov->path_prog += dist;
        dist = 0.0;
      }else{
        move += p * (next / step);
        dist -= next;
        mov->path_prog = 0.0;
        ++mov->path_pos;
        if(mov->path_pos == mov->path.size()) mov->path_pos = 0; } }
  }else if(mov->pat == movement::ORBIT){
    double r = pos.dist(mov->root->pos);
    double deg = dist / r;
    point p = pos;
    p.rotate(mov->root->pos, uvec(0, 0, -1), deg);
    move = p - pos; }
  pos += move;
  validate("object.move");
  return move; }

#endif
