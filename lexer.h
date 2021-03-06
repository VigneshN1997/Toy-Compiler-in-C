// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "hashtable.c"

void removeComments(char* testcaseFile);

Token* nextToken(int* diff_buffer, int* buffer_read_into, char** lexemeBegin,char** forward,char** buffer1,char** buffer2,FILE* fp,char* buffer1_end,char* buffer2_end,char** prvs_buff_end,char** curr_buff_start,int* flag,int* line_number,HASH_TABLE* lookupTable,char* ef);

void retract(char** forward);
Token* processInput(SYMBOL_NAME t_name,char** lexemeBegin,char** forward,int* diff_buffer,int buffer_read_into,char* buffer1_end,char* buffer2_end,char** prvs_buff_end,char* curr_buff_start,int line_number,HASH_TABLE* lookupTable);
char* getPreviousBufferEnd(int diff_buffer,int buffer_read_into,char* buffer1_end,char* buffer2_end);
Lexeme* getLexeme(char* lexemeBegin,char* forward,int diff_buffer, char* prvs_buff_end,char* curr_buff_start);
Token* createToken(SYMBOL_NAME t_name,long int line_number,Lexeme* lexeme);
Token* getErrorToken(char** lexemeBegin,char** forward,int* diff_buffer,int buffer_read_into,char* buffer1_end,char* buffer2_end,char** prvs_buff_end,char* curr_buff_start,int line_number,int error_no);

