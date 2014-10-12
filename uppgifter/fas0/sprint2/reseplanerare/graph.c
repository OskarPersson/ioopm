#include <stdlib.h>
#include "graph.h"
#include <stdio.h>
#include <string.h>

struct node {
  void* value;
  int currentCost;
  Node* previous;
  Edge* previousEdge;
  Node* next;
  Edge* nextEdge;
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
  node -> currentCost = -1;
  node -> previous = NULL;
  node -> previousEdge = NULL;
  node -> next = NULL;
  node -> nextEdge = NULL;
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
		Node* otherNode = (node == fstNode) ? sndNode : fstNode;
		if (otherNode -> currentCost == -1 || 
			(node -> currentCost + edges[i] -> cost) < otherNode -> currentCost){
		  otherNode -> currentCost = node -> currentCost + edges[i] -> cost;
		  otherNode -> previous = node;
		  otherNode -> previousEdge = edges[i];
		  nodeEdges(edges, n, otherNode);
		}
		count++;
      }
    }
  }
  result = realloc(result, sizeof(Edge*)*count);
  return result;
}

Edge** fastestPath(Edge** edges, int n, Node* start, Node* end){
  start->currentCost = 0; //mark start as visited
  Edge** path = malloc(sizeof(edges));
  Edge** edgesToStart = nodeEdges(edges, n, start);
  Node* previousNode = end;
  Node* nextNode = end;
  Node* currentNode = end;
  
  while(previousNode != start){
	nextNode = previousNode;
    previousNode = previousNode->previous;
	previousNode -> next = nextNode;
	previousNode -> nextEdge = nextNode -> previousEdge;
  }
  nextNode = start;
  while(nextNode != end){
	currentNode = nextNode;
    nextNode = nextNode -> next;	
	printf("%d %s -> %s\n", *(int*)getEdgeValue(currentNode->nextEdge), (char*) currentNode->value, (char*) nextNode->value);
  }
  
  printf("Minutes: %d", end->currentCost);
  
  return path;
}
