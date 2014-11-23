#include <stdlib.h>
#include "graph.h"
#include <stdio.h>
#include <string.h>

struct graph{
  Edge **edges;
  int size;
};

struct node {
  void *value;
  int currentCost;
  Node *previous;
  Edge *previousEdge;
  Node *next;
  Edge *nextEdge;
};

struct edge {
  Node *node1;
  Node *node2;
  int cost;
  void *value;
};

Graph *mkGraph(Edge **e, int s){
  Graph *g = malloc(sizeof(Graph));
  g->edges = e;
  g->size = s;
  return g;
}

void rmGraph(Graph *graph){
  Edge **edges = graph->edges;
  for (int i = 0; i<graph->size; i++){
	if (edges[i] != NULL){
	  Node *firstNode = getEdgeFirst(edges[i]);
	  Node *secondNode = getEdgeSecond(edges[i]);
	  if (firstNode != NULL || secondNode != NULL){
		for (int j = i; j<graph->size; j++){
		  if (getEdgeFirst(edges[j]) != NULL){
			if (firstNode == getEdgeFirst(edges[j]) ||
				secondNode == getEdgeFirst(edges[j])){
			  free(getEdgeFirst(edges[j])->value);
			  setNodeValue(getEdgeFirst(edges[j]), NULL);
			  setEdgeFirst(edges[j], NULL);
			}
		  }

		  if (getEdgeSecond(edges[j]) != NULL){
			if (firstNode == getEdgeSecond(edges[j]) ||
				secondNode == getEdgeSecond(edges[j])){
			  free(getEdgeSecond(edges[j])->value);
			  setNodeValue(getEdgeSecond(edges[j]), NULL);
			  setEdgeSecond(edges[j], NULL);
			}
		  }
		}

		if (firstNode != NULL){
		  free(firstNode);
		}

		if (secondNode != NULL){
		  free(secondNode);
		}
	  }
	  free(edges[i]->value);
	  free(edges[i]);
	}
  }
  free(graph->edges);
  free(graph);
}

int graphSize(Graph *g){
  return g->size;
}

Edge **graphEdges(Graph *g){
  return g->edges;
}

Node *mkNode(void *p){
  Node *node = malloc(sizeof(Node));
  node->value = p;
  node->currentCost = -1;
  node->previous = NULL;
  node->previousEdge = NULL;
  node->next = NULL;
  node->nextEdge = NULL;
  return node;
}

Edge *mkEdge(Node *n1, Node *n2, int c, void *v){
  Edge *edge = malloc(sizeof(Edge));
  edge->node1 = n1;
  edge->node2 = n2;
  edge->cost = c;
  edge->value = v;
  return edge;
}

void rmNode(Node *n, Graph *graph){
  free(n->value);
  free(n);
  for (int i = 0; i < graph->size; i++){
	if (getEdgeFirst(graph->edges[i]) == n ||
		getEdgeSecond(graph->edges[i]) == n){
	  rmEdge(graph->edges[i], graph);
	  i--;
	}
  }
}

void rmEdge(Edge *e, Graph *graph){
  free(e->value);
  free(e);
  for (int i = 0; i < graph->size; i++){
	if (graph->edges[i] == e){
	  for(int j = i; j < graph->size-1; j++) graph->edges[j] = graph->edges[j + 1];
	  graph->size--;
	  graph->edges = realloc(graph->edges, graph->size  *sizeof(Edge*));	  
	}
  }
}

void *getNodeValue(Node *n){
  return n->value;
}

void setNodeValue(Node *n, void *p){
  n->value = p;
}

Node *getEdgeFirst(Edge *e){
  return e->node1;
};

Node *getEdgeSecond(Edge *e){
  return e->node2;
};

void setEdgeFirst(Edge *e, Node *n){
  e->node1 = n;
}

void setEdgeSecond(Edge *e, Node *n){
  e->node2 = n;
}

int getEdgeCost(Edge *e){
  return e->cost;
}

void *getEdgeValue(Edge *e){
  return e->value;
};

void setEdgeValue(Edge *e, void *p){
  e->value = p;
}

Edge *nextEdge(Node *n){
  return n->nextEdge;
}
Edge *prevEdge(Node *n){
  return n->previousEdge;
}
Node *nextNode(Node *n){
  return n->next;
}
Node *prevNode(Node *n){
  return n->previous;
}

void nodeEdges(Graph *g, Node *node){
  int count = 0;
  int n = g->size;
  Edge **edges = g->edges;
  for (int i = 0; i<n; i++){
    if (edges[i] != NULL){
      Node *fstNode = edges[i]->node1;
      Node *sndNode = edges[i]->node2;
	  if (node == fstNode || node == sndNode){
		Node *otherNode = (node == fstNode) ? sndNode : fstNode;
		if (otherNode->currentCost == -1 || 
			(node->currentCost + edges[i]->cost) < otherNode->currentCost){
		  otherNode->currentCost = node->currentCost + edges[i]->cost;
		  otherNode->previous = node;
		  otherNode->previousEdge = edges[i];
		  nodeEdges(g, otherNode);
		}
		count++;
      }
    }
  }
}

void nodeEdgesOneLine(Graph *g, Node *node, int line){
  int count = 0;
  int n = g->size;
  Edge **edges = g->edges;
  for (int i = 0; i<n; i++){
    if (edges[i] != NULL){
	  if (line == *(int*)edges[i]->value){
		Node *fstNode = edges[i]->node1;
		Node *sndNode = edges[i]->node2;
		if (node == fstNode || node == sndNode){
		  Node *otherNode = (node == fstNode) ? sndNode : fstNode;
		  if (otherNode->currentCost == -1 || 
			  (node->currentCost + edges[i]->cost) < otherNode->currentCost){
			otherNode->currentCost = node->currentCost + edges[i]->cost;
			otherNode->previous = node;
			otherNode->previousEdge = edges[i];
			nodeEdgesOneLine(g, otherNode, line);
		  }
		  count++;
		}
	  }
    }
  }
}

void fastestPath(Graph *g, Node *start, Node *end){
  start->currentCost = 0; //mark start as visited;
  nodeEdges(g, start);
  Node *previousNode = end;
  Node *nextNode = end;
  Node *currentNode = end;
  
  while(previousNode != start){
	nextNode = previousNode;
    previousNode = previousNode->previous;
	previousNode->next = nextNode;
	previousNode->nextEdge = nextNode->previousEdge;
  }
  nextNode = start;
  while(nextNode != end){
	currentNode = nextNode;
    nextNode = nextNode->next;	
	printf("%d %s -> %s\n", *(int*)getEdgeValue(currentNode->nextEdge),
		   (char*) currentNode->value, (char*) nextNode->value);
  }
  
  printf("Minuter: %d\n", end->currentCost);
}

int fastestPathOneLine(Graph *g, Node *start, Node *end){
  start->currentCost = 0; //mark start as visited;

  for (int i = 0; i < g->size; i++) {
    if (g->edges[i]->node1 == start || g->edges[i]->node2 == start){
	  nodeEdgesOneLine(g, start, *(int*)g->edges[i]->value);
	}
  }

  if (end->previous != NULL){
	return end->currentCost;
  }else{
	return -1;
  }
}

void printPath(Node *start, Node *end){
  Node *previousNode = end;
  Node *nextNode = end;
  Node *currentNode = end;

  if (end->previous != NULL){
	while(previousNode != start){
	  nextNode = previousNode;
	  previousNode = previousNode->previous;
	  previousNode->next = nextNode;
	  previousNode->nextEdge = nextNode->previousEdge;
	}
	nextNode = start;
	while(nextNode != end){
	  currentNode = nextNode;
	  nextNode = nextNode->next;	
	  printf("%d %s -> %s\n", *(int*)getEdgeValue(currentNode->nextEdge),
			 (char*) currentNode->value, (char*) nextNode->value);
	}
	printf("Minuter: %d\n", end->currentCost);
  }
}
