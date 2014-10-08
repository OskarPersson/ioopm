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
      for (int j = 0; j < i; j++) {
	if (edgeArr[j] != NULL){
	  Edge* curEdge = edgeArr[j];
	  char* firstValue = (char*)getValue(getEdgeFirst(curEdge));
	  char* secondValue = (char*)getValue(getEdgeSecond(curEdge));
	  int busValue = ((int*)getEdgeValue(curEdge))[0];
	  if (((strcmp(firstValue, strarr[i][1]) == 0 && strcmp(secondValue, strarr[i][2]) == 0) ||
	       (strcmp(firstValue, strarr[i][2]) == 0 && strcmp(secondValue, strarr[i][1]) == 0))){ //all same
	    if (busValue != newBusValue){ //different bus
	      fstNode = mkNode(strarr[i][1]);
	      sndNode = mkNode(strarr[i][2]);
	      break;
	    }else{
	      fstNode = NULL;
	      sndNode = NULL;
	      break;
	    }
	  }else if (strcmp(firstValue, strarr[i][1]) == 0){ //first, first
	    fstNode = getEdgeFirst(curEdge);
	    sndNode = mkNode(strarr[i][2]);
	    break;
	  }else if (strcmp(firstValue, strarr[i][2]) == 0){ //first, second
	    fstNode = getEdgeFirst(curEdge);
	    sndNode = mkNode(strarr[i][1]);
	    break;
	  }else if (strcmp(secondValue, strarr[i][1]) == 0){ //second, first
	    fstNode = getEdgeSecond(curEdge);
	    sndNode = mkNode(strarr[i][2]);
	    break;
	  }else if (strcmp(secondValue, strarr[i][2]) == 0){ //second, second
	    fstNode = getEdgeSecond(curEdge);
	    sndNode = mkNode(strarr[i][1]);
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
	
    if (fstNode != NULL && sndNode != NULL){
      int* edgeValues = malloc(sizeof(int)*2);
      edgeValues[0] = newBusValue; //busslinje
      edgeValues[1] = atoi(strarr[i][3]); //minuter
	  
      Edge* newEdge = mkEdge(fstNode, sndNode, edgeValues);
      edgeArr[i] = newEdge;
    }
    else{
      edgeArr[i] = NULL;
    }
  }
  return edgeArr;
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
	  
      int index = 12;
      if (edgeArr[index] != NULL){
	printf("Från\t\t%s\n"
	       "Till\t\t%s\n"
	       "Linje\t\t%d\n"
	       "Minuter\t\t%d",
	       (char*)getValue(getEdgeFirst(edgeArr[index])),
	       (char*)getValue(getEdgeSecond(edgeArr[index])),
	       ((int*)getEdgeValue(edgeArr[index]))[0],
	       ((int*)getEdgeValue(edgeArr[index]))[1]);
      }else{
	printf("Edge at index %d does not exist", index);
      }
    }
  }
  return 0;
}

