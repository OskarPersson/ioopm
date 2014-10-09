#include <stdlib.h>
#include "graph.h"
#include <stdio.h>

struct node {
  void* value;
  int currentCost;
};

struct edge {
  Node* node1;
  Node* node2;
  int cost;
  void* value;
};

Node* mkNode(void* p){
  Node* node = malloc(sizeof(Node));
  node -> value = p;
  node -> currentCost = NULL;
  return node;
}

Edge* mkEdge(Node* n1, Node* n2, int c, void* v){
  Edge* edge = malloc(sizeof(Edge));
  edge -> node1 = n1;
  edge -> node2 = n2;
  edge -> cost = c;
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

int getEdgeCost(Edge* e){
  return e->cost;
}

void* getEdgeValue(Edge* e){
  return e->value;
};

Edge** nodeEdges(Edge** edges, int n, Node* node){
  Edge** result = malloc(sizeof(Edge*)*n);
  int count = 0;
  for (int i = 0; i<n; i++){
    if (edges[i] != NULL){
      
      Node* fstNode = edges[i] -> node1;
      Node* sndNode = edges[i] -> node2;
      
      if (node == fstNode || node == sndNode){
	result[count] = edges[i];
	Node* otherNode = (node == fstNode) ? sndNode : fstNode;
	otherNode -> currentCost = edges[i] -> cost;
	printf("othernode: %s\n", (char*)getValue(otherNode));
	count++;
      }
    }
  }
  result = realloc(result, sizeof(Edge*)*count);
  return result;
}

Edge** fastestPath(Edge** edges, int n, Node* start, Node* end){
  start->currentCost = 0; //mark start as visited
  Edge** edgesToNode = nodeEdges(edges, n, start);
  
  return edgesToNode;//path;
}
