#include <stdio.h>
#include "graph.h"

int main(){
  char* val_1 = "firstNode";
  char* val_2 = "secondNode";
  Node* fstNode = mkNode(val_1);
  Node* sndNode = mkNode(val_2);
  int edgeVal = 8;
  Edge* myEdge = mkEdge(fstNode, sndNode, &edgeVal);
  printf("Value of first node: %s, value of second node: %s, minutes: %d\n", 
		 (char*) getValue(getEdgeFirst(myEdge)),
		 (char*) getValue(getEdgeSecond(myEdge)),
		 *((int*) getEdgeValue(myEdge)));
  return 0;
}
