#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "parse.h"

Edge** mkEdges(char*** strarr, int n){
  Edge** edgeArr = malloc(sizeof(Edge*)*n);
  for (int i=0; i<n; i++){
    Node* fstNode = NULL;
    Node* sndNode = NULL;
    int newBusValue = atoi(strarr[i][0]); //busslinje
    if (i > 0){
      for (int j = 0; j <= i; j++) {
	if (edgeArr[j] != NULL){
	  Edge* curEdge = edgeArr[j];
	  char* firstValue = (char*)getValue(getEdgeFirst(curEdge));
	  char* secondValue = (char*)getValue(getEdgeSecond(curEdge));
	  int busValue = *(int*)getEdgeValue(curEdge);
	  if (((strcmp(firstValue, strarr[i][1]) == 0 && strcmp(secondValue, strarr[i][2]) == 0) ||
	       (strcmp(firstValue, strarr[i][2]) == 0 && strcmp(secondValue, strarr[i][1]) == 0))){ //all same
	    if (busValue != newBusValue){ //different bus
	      fstNode = getEdgeFirst(curEdge);
	      sndNode = getEdgeSecond(curEdge);
	    }else{
	      fstNode = NULL;
	      sndNode = NULL;
	    }	 
	    break;
	  }else if (strcmp(firstValue, strarr[i][1]) == 0){ //first, first
	    fstNode = getEdgeFirst(curEdge);
	    sndNode = mkNode(strarr[i][2]);
	    break;
	  }else if (strcmp(firstValue, strarr[i][2]) == 0){ //first, second
	    fstNode = mkNode(strarr[i][1]);
	    sndNode = getEdgeFirst(curEdge);
	    break;
	  }else if (strcmp(secondValue, strarr[i][1]) == 0){ //second, first
	    fstNode = getEdgeSecond(curEdge);
	    sndNode = mkNode(strarr[i][2]);
	    break;
	  }else if (strcmp(secondValue, strarr[i][2]) == 0){ //second, second
	    fstNode = mkNode(strarr[i][1]);
	    sndNode = getEdgeSecond(curEdge);
	    break;
	  }
	}else{
	  fstNode = mkNode(strarr[i][1]);
	  sndNode = mkNode(strarr[i][2]);
	  break;
	}
      }
    }else{
      fstNode = mkNode(strarr[i][1]);
      sndNode = mkNode(strarr[i][2]);
    }

    int* edgeValue = malloc(sizeof(int));
    *edgeValue = newBusValue;
	
    if (fstNode != NULL && sndNode != NULL){
      int cost = atoi(strarr[i][3]);
      Edge* newEdge = mkEdge(fstNode, sndNode, cost, edgeValue);
      edgeArr[i] = newEdge;
    }
    else{
      edgeArr[i] = NULL;
    }
  }
  return edgeArr;
}

Node* findNode(char* str, Edge** edges, int n){
  for (int i = 0; i<n; i++){
    if (edges[i] != NULL){
      Node* fstNode = getEdgeFirst(edges[i]);
      Node* sndNode = getEdgeSecond(edges[i]);
      if (strcmp((char*)getValue(fstNode), str) == 0){
	return fstNode;
      }else if(strcmp((char*)getValue(sndNode), str) == 0){
	return sndNode;
      }
    }
  }
  return NULL;
}

int main(int argc, char *argv[]){
  if (argc < 2){
    puts("Usage: ./main network_file.txt");
  }else{
    char* networkFileName = argv[1];
    FILE* file = fopen(networkFileName, "r");
    if (file){
      int rows = countRowsInFile(file);
      char*** arr = parsefile(file, rows);
      Edge** edgeArr = mkEdges(arr, rows);

      fclose(file);
      /*
      int index = 12;
      if (edgeArr[index] != NULL){
	printf("Från\t\t%s\n"
	       "Till\t\t%s\n"
	       "Linje\t\t%d\n"
	       "Minuter\t\t%d",
	       (char*)getValue(getEdgeFirst(edgeArr[index])),
	       (char*)getValue(getEdgeSecond(edgeArr[index])),
	       (int)getEdgeCost(edgeArr[index]),
	       *(int*)getEdgeValue(edgeArr[index]));
      }else{
	printf("Edge at index %d does not exist", index);
      }*/
      Node* start = NULL;
      Node* end = NULL;
      char* startStr = "Centralstationen";
      char* endStr = "Centralstationen";
      start = findNode(startStr, edgeArr, rows);
      end = findNode(endStr, edgeArr, rows);
      //printf("start: %s\n", (char*)getValue(start));
      //printf("end: %s\n", (char*)getValue(end));
      fastestPath(edgeArr, rows, start, end);
    }
  }
  return 0;
}
