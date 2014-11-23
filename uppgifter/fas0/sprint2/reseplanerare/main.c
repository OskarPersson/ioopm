/** 
 *  @file	main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "graph.h"
#include "parse.h"

/**@brief checks if a string only contains digits
 * @param str the string to look in
 * @return 1 if the string only contains digits, else false
 */
int isNumeric (char *str){
  while(*str){
	if (!isdigit(*str)){
	  return 0;
	}else{
	  str++;
	}
  }
  return 1;
}

/**@brief creates an array of edges
 * @param strarr an array of string arrays where the first element in the string array is the bus, the second is the source, the third the destination and the fourth the cost (time)
 * @param n the number of edges to create
 * @return an array of edges where each edge is connected to two nodes, has a cost and a bus
 */
Edge **mkEdges(char ***strarr, int *n){
  Edge **edgeArr = calloc(*n, sizeof(Edge*));
  int edgeCounter = 0;
  for (int i=0; i<*n; i++){
	//Is it a well-formed row?
	
	if (strarr[i][3] == NULL || 
		!(isNumeric(strarr[i][0]) && !isNumeric(strarr[i][1]) &&
		  !isNumeric(strarr[i][2]) && isNumeric(strarr[i][3]))){
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
  edgeArr = realloc(edgeArr, sizeof(Edge*) * edgeCounter);
  *n = edgeCounter;
  return edgeArr;
}

/**@brief searches for a node with the given string as value
 * @param str the value that the node have
 * @param graph the graph to search in
 * @return the node with the given string as value
 */
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

/**@brief starts the application which gets the fastest path from one node to another
 * @param argc number of arguments given at start
 * @param argv arguments given at start
 * @return 0
 */
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

	  free(startStr);
	  free(endStr);

	  int min = fastestPathOneLine(graph, start, end);
	  
	  if (min > 0){
		printPath(start, end);
		
		char *givenTime = malloc(sizeof(char)*6);
		printf("När vill du åka? ");
		fgets(givenTime, 6, stdin);
		
		int givenHour = atoi(strtok(givenTime, ":"));
		int givenMin = atoi(strtok(NULL, ":"));
		
		int line = *(int*)getEdgeValue(nextEdge(start));
		char *first = NULL;
		char *second = NULL;
		for (int i = 0; i < startRows; i++){
		  if (atoi(startArr[i][0]) == line){
			if (first == NULL){
			  first = startArr[i][1];
			}else if(strcmp(first, startArr[i][1]) != 0){
			  second = startArr[i][1];
			  break;
			}
		  }
		}

		Node *firstNode = findNode(first, graph);
		Node *secondNode = findNode(second, graph);
		int firstMin = fastestPathOneLine(graph, start, firstNode);
		int secondMin = fastestPathOneLine(graph, start, secondNode);

		Node *bestNode;

		if (firstMin < secondMin){
		  bestNode = firstNode;
		}else{
		  bestNode = secondNode;
		}
		
		int extraMin = firstMin < secondMin ? firstMin : secondMin;
		
		puts("\nMöjliga avgångar: \n");
		for (int i = 0; i < startRows; i++){
		  if (atoi(startArr[i][0]) == line && strcmp(startArr[i][1], (char*)getNodeValue(bestNode)) == 0){
			int h = atoi(strtok(startArr[i][2], ":"));
			int m = atoi(strtok(NULL, ":"));
			
			m = m + extraMin;
			if (m > 60){
			  h++;
			  m = m - 60;
			}
			
			if ((givenHour == h && givenMin <= m) || givenHour < h ){
			  printf("%02d:%02d\n", h, m);
			}
		  }
		}
		free(givenTime);
	  }else{
		printf("Det går inte att åka från %s till %s utan byte\n", (char*)getNodeValue(start), (char*)getNodeValue(end));
	  }
	  

	  for (int i = 0; i < networkRows; i++){
		for (int j = 0; j < 4; j++) {
		  free(networkArr[i][j]);
		}
		free(networkArr[i]);
	  }
	  free(networkArr);

	  for (int i = 0; i < startRows; i++){
		for (int j = 0; j < 3; j++) {
		  free(startArr[i][j]);
		}
		free(startArr[i]);
	  }
	  free(startArr);

	  rmGraph(graph);
    }
  }
  return 0;
}
