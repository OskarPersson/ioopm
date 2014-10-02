#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define TEST(ID, test) test_count++; \
                       if(test) pass_count++; \
                       else printf("Failed test " #ID ": " #test "\n");

void runTests(){
  int test_count = 0;
  int pass_count = 0;

  struct node* node = mkNode(4);
  
  TEST(1, getValue(node) == 4);

  printf("Passed %d out of %d tests\n", pass_count, test_count);
}

int main(){
  runTests();
}
