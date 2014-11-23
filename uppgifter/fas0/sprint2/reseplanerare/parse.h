/** 
 *  @file	parse.h
 */

/**@brief remove leading and trailing whitespace
 *@param str the string to remove whitespace from
 *@return the given string with no leading and trailing whitespace
 */
char *removeWhitespace(char*);
/**@brief splits the given string based on the delimiter
 *@param str the string to split
 *@param del the delimiter
 *@return an array of strings where every element is a part of the given string
 */
char **separateString(char*, char*);
/**@brief counts the rows in the given file
 *@param f the file to count in
 *@return the number of rows in the given file
 */
int countRowsInFile(FILE*);
/**@brief parses each line to an array of strings based on the ", " delimiter
 *@param f the file to parse
 *@param rows the number of rows in the file
 *@return an array of string arrays where each element is based on a string split by the ", " delimiter
 */
char ***parsefile(FILE*, int);
