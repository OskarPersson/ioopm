#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char* removeWhitespace(char* str){  
  //Remove leading whitespace
  while(isspace(*str)) str++;

  //Remove trailing whitespace
  int i = strlen(str);

  for(int j = i-1; j>=0; j--){
	if (isspace(str[j])){
	  i--;
	}else{
	  break;
	}
  }

  char* strdup = malloc(sizeof(char)*i+1);
  
  strncpy(strdup, str, i);
  
  return strdup;
}

char** separateString(char* str, char* separator){
  char* strdup = malloc(strlen(str)+1);
  strcpy(strdup, str);
  char* token = strtok(str, separator);

  int i = 0;
  while(token != NULL){
    i++;
	token = strtok(NULL, separator);
  }

  char** arr = malloc(sizeof(char*)*i);

  i = 0;
  token = strtok(strdup, separator);
  while(token != NULL){
    arr[i] = removeWhitespace(token);
    token = strtok(NULL, separator);
    i++;
  }
  return arr;
};

int countRowsInFile(FILE* file){
  int ctr = 0;
  while(!feof(file)){
	ctr = (fgetc(file) == '\n') ? ctr+1 : ctr;
  }
  rewind(file); //go to the start of the file
  return ctr;
}

//parses each line to an array of strings based on the commas,
//saves all lines in an array and returns it
char*** parsefile(FILE* file, int rows){
  char*** arr = malloc(sizeof(char**)*rows);
  char buffer[1024];
  int ctr = 0;
  while( fgets(buffer, 1024, file) ) {
	arr[ctr] = malloc(sizeof(arr[ctr]) * 4);
	arr[ctr] = separateString(buffer, ",");
	ctr++;
  }
  return arr;
}
