#include <stdlib.h>
#include "graph.h"

struct node {
  void* value;
};

struct edge {
  void* node1;
  void* node2;
  void* value;
};

Node* mkNode(void* p){
  Node* node = malloc(sizeof(Node));
  node -> value = p;
  return node;
}

Edge* mkEdge(Node* n1, Node* n2, void* v){
  //Edge myEdge = {.node1 = n1, .node2 = n2, .minutes = m};
  Edge* edge = malloc(sizeof(Edge));
  edge -> node1 = n1;
  edge -> node2 = n2;
  edge -> value = v;
  return edge;
}

void* getValue(Node* n){
  return n -> value;
}

Node* getEdgeFirst(Edge* e){
  return e->node1;
};
Node* getEdgeSecond(Edge* e){
  return e->node2;
};
void* getEdgeValue(Edge* e){
  return e->value;
};
