#include "structs.h"

Token** getTokens(FILE* fp)
{
	fp = fopen(fp,"r");
	char* buffer1 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	char* buffer2 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	buffer1[BUFFER_SIZE - 1] = '?';
	buffer2[BUFFER_SIZE - 1] = '?';
	size_t nread;
	int buffer_read_into = 1;
	nread = fread(buffer1,sizeof(char),BUFFER_SIZE - 1,fp);
	char* lexemeBegin,forward;
	while(1)
	{
		Token* t1 = getNextToken();
	}
}



Token* getNextToken(char* buf1,char* buf2,)
{

}