#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "parse.h"

Edge **mkEdges(char ***strarr, int *n){
  Edge **edgeArr = calloc(*n, sizeof(Edge*));
  int edgeCounter = 0;
  for (int i=0; i<*n; i++){
	//Is it a well-formed row?
	if (strarr[i][3] == NULL){
	  continue;
	}
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
      edgeArr[edgeCounter++] = newEdge;
    }
  }
  *n = edgeCounter;
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
  if (argc < 2){
    puts("Usage: ./main network_file.txt");
  }else{
    char *networkFileName = argv[1];
    FILE *networkFile = fopen(networkFileName, "r");
	char *startFileName = "start.txt";
    FILE *startFile = fopen(startFileName, "r");
    if (networkFile && startFile){
      int networkRows = countRowsInFile(networkFile);
	  int startRows = countRowsInFile(startFile);
      char ***networkArr = parsefile(networkFile, networkRows);
	  char ***startArr = parsefile(startFile, startRows);
      Edge **edgeArr = mkEdges(networkArr, &networkRows);
	  Graph *graph = mkGraph(edgeArr, networkRows);

      fclose(networkFile);
	  fclose(startFile);

      Node *start = NULL;
      Node *end = NULL;
	  char *startStr = calloc(128, sizeof(char));
	  char *endStr = calloc(128, sizeof(char));
	  
	  while (strcmp(startStr, "") == 0 || start == NULL){
		printf("Från: ");
		fgets(startStr, 128, stdin);
		//remove trailing new-line
		if ((strlen(startStr) > 0) && (startStr[strlen(startStr)-1] == '\n')){
		  startStr[strlen(startStr)-1] = '\0';
		}
		start = findNode(startStr, graph);
		if (start == NULL){
		  printf("Hållplatsen \"%s\" existerar inte\n", startStr);
		}
	  }

	  while (strcmp(endStr, "") == 0 || end == NULL){
		printf("Till: ");
		fgets(endStr, 128, stdin);
		//remove trailing new-line
		if ((strlen(endStr) > 0) && (endStr[strlen(endStr)-1] == '\n')){
		  endStr[strlen(endStr)-1] = '\0';
		}
		end = findNode(endStr, graph);
		if (end == NULL){
		  printf("Hållplatsen \"%s\" existerar inte\n", endStr);
		}
	  }

	  fastestPath(graph, start, end);

	  for (int i = 0; i < networkRows; i++){
		for (int j = 0; j < 4; j++) {
		  free(networkArr[i][j]);
		}
		free(networkArr[i]);
	  }
	  free(networkArr);
	  rmGraph(graph);
    }
  }
  return 0;
}
