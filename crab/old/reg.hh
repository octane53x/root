// GLOBAL TYPE REGISTRY

#ifndef REG_HH
#define REG_HH

#include "incl.hh"

// Graph of all types and their inheritance
struct Registry {

  // Type pointing to parent types
  struct RegNode {
    Type type;
    vec<RegNode*> parents; };

  // Nodes accessed by type name
  umap<Type, RegNode> nodes;

  void add(const Type& type, const vec<Type>& parents);

} reg;

// Add a node to the registry
void Registry::add(const Type& type, const vec<Type>& parents){
  if(nodes.find(type) != nodes.end()) return;
  RegNode node;
  node.type = type;
  for(const Type& p : parents)
    node.parents.pb(&nodes[p]);
  nodes[type] = node; }

#endif
