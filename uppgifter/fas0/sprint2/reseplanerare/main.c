#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "parse.h"

Edge **mkEdges(char ***strarr, int n){
  Edge **edgeArr = calloc(n, sizeof(Edge*));
  for (int i=0; i<n; i++){
    Node *fstNode = NULL;
    Node *sndNode = NULL;
    int newBusValue = atoi(strarr[i][0]); //busslinje
    if (i > 0){
      for (int j = 0; j <= i; j++) {
		if (edgeArr[j] != NULL){
		  Edge *curEdge = edgeArr[j];
		  char *firstValue = (char*)getNodeValue(getEdgeFirst(curEdge));
		  char *secondValue = (char*)getNodeValue(getEdgeSecond(curEdge));
		  int busValue = *(int*)getEdgeValue(curEdge);
		  if (((strcmp(firstValue, strarr[i][1]) == 0 && strcmp(secondValue, strarr[i][2]) == 0) ||
			   (strcmp(firstValue, strarr[i][2]) == 0 && strcmp(secondValue, strarr[i][1]) == 0))){ //all same
			if (busValue != newBusValue){ //different bus
			  if (strcmp(firstValue, strarr[i][1]) == 0 && strcmp(secondValue, strarr[i][2]) == 0){
				fstNode = getEdgeFirst(curEdge);
				sndNode = getEdgeSecond(curEdge);
			  }else{
				fstNode = getEdgeSecond(curEdge);
				sndNode = getEdgeFirst(curEdge);
			  }
			}else{
			  fstNode = NULL;
			  sndNode = NULL;
			}
		  }else if (strcmp(firstValue, strarr[i][1]) == 0){ //first, first
			fstNode = getEdgeFirst(curEdge);
		  }else if (strcmp(firstValue, strarr[i][2]) == 0){ //first, second
			sndNode = getEdgeFirst(curEdge);
		  }else if (strcmp(secondValue, strarr[i][1]) == 0){ //second, first
			fstNode = getEdgeSecond(curEdge);
		  }else if (strcmp(secondValue, strarr[i][2]) == 0){ //second, second
			sndNode = getEdgeSecond(curEdge);
		  }
		}else{
		  if (fstNode == NULL){
			char *fstString = malloc(sizeof(char)*strlen(strarr[i][1])+1);
			strcpy(fstString, strarr[i][1]);
			fstNode = mkNode(fstString);
		  }
		  if (sndNode == NULL){
			char *sndString = malloc(sizeof(char)*strlen(strarr[i][2])+1);
			strcpy(sndString, strarr[i][2]);
			sndNode = mkNode(sndString);
		  }
		}
      }
    }else{
	  char *fstString = malloc(sizeof(char)*strlen(strarr[i][1])+1);
	  strcpy(fstString, strarr[i][1]);
	  char *sndString = malloc(sizeof(char)*strlen(strarr[i][2])+1);
	  strcpy(sndString, strarr[i][2]);
      fstNode = mkNode(fstString);
      sndNode = mkNode(sndString);
    }

    int *edgeValue = malloc(sizeof(int));
    *edgeValue = newBusValue;
	
    if (fstNode != NULL && sndNode != NULL){
      int cost = atoi(strarr[i][3]);
      Edge *newEdge = mkEdge(fstNode, sndNode, cost, edgeValue);
      edgeArr[i] = newEdge;
    }
    else{
      edgeArr[i] = NULL;
    }
  }
  return edgeArr;
}

Node *findNode(char *str, Graph *graph){
  Edge **edges = graphEdges(graph);
  for (int i = 0; i<graphSize(graph); i++){
    if (edges[i] != NULL){
      Node *fstNode = getEdgeFirst(edges[i]);
      Node *sndNode = getEdgeSecond(edges[i]);
      if (strcmp((char*)getNodeValue(fstNode), str) == 0){
		return fstNode;
      }else if(strcmp((char*)getNodeValue(sndNode), str) == 0){
		return sndNode;
      }
    }
  }
  return NULL;
}

int main(int argc, char *argv[]){
  if (argc < 4){
    puts("Usage: ./main network_file.txt start end");
  }else{
    char *networkFileName = argv[1];
    FILE *file = fopen(networkFileName, "r");
    if (file){
      int rows = countRowsInFile(file);
      char ***arr = parsefile(file, rows);
      Edge **edgeArr = mkEdges(arr, rows);
	  Graph *graph = mkGraph(edgeArr, rows);

      fclose(file);

      Node *start = NULL;
      Node *end = NULL;
      char *startStr = argv[2];
	  char *endStr = argv[3];
      start = findNode(startStr, graph);
      end = findNode(endStr, graph);

	  fastestPath(graph, start, end);

	  for (int i = 0; i < rows; i++){
		for (int j = 0; j < 4; j++) {
		  free(arr[i][j]);
		}
		free(arr[i]);
	  }
	  free(arr);
	  rmGraph(graph);
    }
  }
  return 0;
}
