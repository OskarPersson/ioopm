#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "parse.h"

int init_suite_1(void){
  return 0;
}

int clean_suite_1(void){
  return 0;
}

int init_suite_2(void){
  return 0;
}

int clean_suite_2(void){
  return 0;
}


void testMKGRAPH(void){
  Node *node1 = mkNode(NULL);
  Node *node2 = mkNode(NULL);

  Edge *edge1 = mkEdge(node1, node2, 1, NULL);
  Edge *edge2 = mkEdge(node2, node1, 2, NULL);

  Edge **edgeArr = malloc(sizeof(Edge*)*2);
  edgeArr[0] = edge1;
  edgeArr[1] = edge2;

  Graph *graph = mkGraph(NULL, 0);
  CU_ASSERT(graphSize(graph) == 0);
  rmGraph(graph);
  graph = mkGraph(edgeArr, 2);
  CU_ASSERT(graphSize(graph) == 2);
  CU_ASSERT(graphEdges(graph)[1] == edge2);
  rmGraph(graph);
}

void testMKNODE(void){
  char *val = "node value";
  Node *node = mkNode(val);
  CU_ASSERT(getNodeValue(node) == val);
  int *val2 = malloc(sizeof(int));
  *val2 = 32;
  setNodeValue(node, val2); 
  CU_ASSERT(getNodeValue(node) == val2);
  CU_ASSERT(*(int*)getNodeValue(node) == 32);
  free(val2);
  free(node);
}

void testMKEDGE(void){
  char *val = "edge value";
  Node *node = mkNode(NULL);
  Node *node2 = mkNode(NULL);
  int cost = 3;
  Edge *edge = mkEdge(node, node2, cost, val);

  CU_ASSERT(getEdgeValue(edge) == val);
  int *val2 = malloc(sizeof(int));
  *val2 = 32;
  setEdgeValue(edge, val2); 
  CU_ASSERT(getEdgeValue(edge) == val2);
  CU_ASSERT(*(int*)getEdgeValue(edge) == 32);
  CU_ASSERT(getEdgeFirst(edge) == node);
  CU_ASSERT(getEdgeSecond(edge) == node2);
  CU_ASSERT(getEdgeCost(edge) == 3);
  free(val2);
  free(edge);
  free(node);
  free(node2);
}

void testRemoveWhitespace(void){
  char *both = "  foo    ";
  char *none = "foo";
  char *leading = "   foo";
  char *trailing = "foo        ";
  CU_ASSERT(strcmp(removeWhitespace(both), "foo") == 0);
  CU_ASSERT(strcmp(removeWhitespace(none), "foo") == 0);
  CU_ASSERT(strcmp(removeWhitespace(leading), "foo") == 0);
  CU_ASSERT(strcmp(removeWhitespace(trailing), "foo") == 0);
}

void testSeparateString(void){
  char str[] = "foo, bar, abc";
  char **strarr = separateString(str, ",");
  CU_ASSERT(strcmp(strarr[0], "foo") == 0);
  CU_ASSERT(strcmp(strarr[1], "bar") == 0);
  CU_ASSERT(strcmp(strarr[2], "abc") == 0);
  
  for (int i=0; i<3; i++){
	free(strarr[i]);
  }
  free(strarr);
  char str2[] = "foo";
  strarr = separateString(str2, ",");
  CU_ASSERT(strcmp(strarr[0], "foo") == 0);
  free(strarr[0]);
  free(strarr);

  char str3[] = "foo||bar";
  strarr = separateString(str3, "||");
  CU_ASSERT(strcmp(strarr[0], "foo") == 0);
  CU_ASSERT(strcmp(strarr[1], "bar") == 0);
  free(strarr[0]);
  free(strarr[1]);
  free(strarr);
}

void testFastestPath(void){
  int *val1 = malloc(sizeof(int));
  int *val2 = malloc(sizeof(int));
  int *val3 = malloc(sizeof(int));
  int *val4 = malloc(sizeof(int));
  *val1 = 3;
  *val2 = 4;
  *val3 = 5;
  *val4 = 4;

  Node *node1 = mkNode(val2);
  Node *node2 = mkNode(val4);
  Node *node3 = mkNode(val1);
  Node *node4 = mkNode(val3);

  Edge *edge1 = mkEdge(node1, node4, 9, val1);
  Edge *edge2 = mkEdge(node1, node2, 3, val2);
  Edge *edge3 = mkEdge(node2, node3, 4, val3);
  Edge *edge4 = mkEdge(node2, node4, 5, val2);

  Edge **edgeArr = malloc(sizeof(Edge*)*4);
  edgeArr[0] = edge1;
  edgeArr[1] = edge2;
  edgeArr[2] = edge3;
  edgeArr[3] = edge4;

  Graph *graph = mkGraph(edgeArr, 4);
  int min = fastestPathOneLine(graph, node1, node4);
  CU_ASSERT(min == 8);
  CU_ASSERT(prevNode(node4) == node2);
  CU_ASSERT(nextNode(node2) == node4);
  CU_ASSERT(prevEdge(node4) == edge4);
  CU_ASSERT(nextEdge(node2) == edge4);
}

int main(){
  CU_pSuite pSuite1 = NULL;
  CU_pSuite pSuite2 = NULL;

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* add suites to the registry */
  pSuite1 = CU_add_suite("Basic Functions Suite", init_suite_1, clean_suite_1);
  if (NULL == pSuite1){
	CU_cleanup_registry();
	return CU_get_error();
  }
  pSuite2 = CU_add_suite("Advanced Functions Suite", init_suite_2, clean_suite_2);
  if (NULL == pSuite2){
	CU_cleanup_registry();
	return CU_get_error();
  }

  /* add the tests to the suites */
  if (
	  (NULL == CU_add_test(pSuite1, "test of mkGraph()", testMKGRAPH)) ||
	  (NULL == CU_add_test(pSuite1, "test of mkNode()", testMKNODE)) ||
	  (NULL == CU_add_test(pSuite1, "test of mkEdge()", testMKEDGE)) ||
	  (NULL == CU_add_test(pSuite1, "test of removeWhitespace()", testRemoveWhitespace)) ||
	  (NULL == CU_add_test(pSuite1, "test of separateString()", testSeparateString))
	  ){
	CU_cleanup_registry();
	return CU_get_error();
  }

  if (
    (NULL == CU_add_test(pSuite2, "test of fastestPathOneLine()", testFastestPath))
	){
	CU_cleanup_registry();
	return CU_get_error();
    }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
