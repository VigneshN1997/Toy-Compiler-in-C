#include "structs.h"
int line_number;



void getTokens(FILE* fp)
{
	int lexeme_size;
	size_t nread;
	int buffer_read_into = 1;
	char* lexemeBegin,*forward,*buffer1,*buffer2;
	char char_read;
	buffer1 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	buffer2 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	buffer1[BUFFER_SIZE - 1] = '?'; // to indicate end of buffers
	buffer2[BUFFER_SIZE - 1] = '?';
	nread = fread(buffer1,sizeof(char),BUFFER_SIZE - 1,fp); // do check of correct read
	if(nread < BUFFER_SIZE - 1)
	{
		buffer1[nread] = '!'; // to indicate end of file
	}
	lexemeBegin = buffer1;
	forward = buffer1;
	DFA_STATE state = START;
	int i;
	// for(i = 0; i < BUFFER_SIZE; i++)
	// {
	// 	if(buffer1[i] == '!')
	// 	{
	// 		printf("hey\n");
	// 	}
	// 	else if(buffer1[i] == '\n')
	// 	{
	// 		printf("newline\n");
	// 	}
	// 	else
	// 	{
	// 		printf("%c\n",buffer1[i]);
	// 	}
	// }
	int flag = 0;
	while(1)
	{
		while(1)
		{
			char_read = *forward++;
			switch(char_read)
			{
				case '?':
					if(buffer_read_into == 1)
					{
						nread = fread(buffer2,sizeof(char),BUFFER_SIZE - 1,fp);
						if(nread < BUFFER_SIZE - 1)
						{
							buffer2[nread] = '!'; // to indicate end of file
						}	
						buffer_read_into = 2; 
					}
					else
					{
						nread = fread(buffer1,sizeof(char),BUFFER_SIZE - 1,fp);
						if(nread < BUFFER_SIZE - 1)
						{
							buffer1[nread] = '!'; // to indicate end of file
						}
						buffer_read_into = 1; 
					}
					break;
				case '!':
					flag = 1;
					break;
			}
			if(flag == 1)
			{
				break;
			}
			switch(state)
			{
				case START:
					if(char_read == '\t' || char_read == ' ') state = START;
					else if((char_read >= 'a' && char_read <= 'z') || (char_read >= 'A' && char_read <= 'Z')) state = ACCEPT_ID1;
					else if(char_read == '_') state = INT_F1;
					else if(char_read == '#') state = INT_COM;
					else if(char_read >= '0' && char_read <= '9') state = INT_NUM;
					else if(char_read == '"') state = INT_STR1;
					else if(char_read == '.') state = INT_A1;
					//else if(char_read == '\n')
					else if(char_read == '<') state = INT_LT;
					else if(char_read == '=') state = INT_EQ;
					else if(char_read == '>') state = INT_GT;
					else if(char_read == '[') state = ACCEPT_SQO;
					else if(char_read == ']') state = ACCEPT_SQC;
					else if(char_read == '(') state = ACCEPT_OP;
					else if(char_read == ')') state = ACCEPT_CL;
					else if(char_read == ';') state = ACCEPT_SC;
					else if(char_read == ',') state = ACCEPT_COMMA;
					else if(char_read == '+') state = ACCEPT_PLUS;
					else if(char_read == '-') state = ACCEPT_MINUS;
					else if(char_read == '*') state = ACCEPT_MUL;
					else if(char_read == '/') state = ACCEPT_DIV;
					else if(char_read == '@') state = ACCEPT_SIZE;
					break;
				case ACCEPT_ID1:
					if((char_read >= 'a' && char_read <= 'z') || (char_read >= 'A' && char_read <= 'Z')) state = ACCEPT_ID1;
					else if(char_read >= '0' && char_read <= '9') state = ACCEPT_ID2;
					else 
					{
						state = START;

					}
					else
					break;
			}
		}
		if(flag == 1)
		{
			break;
		}
	}
}



// Token* getNextToken(char* buf1,char* buf2,)
// {

// }

int main(int argc, char const *argv[])
{
	FILE* fp = fopen("testcase.txt","r");
	// getTokens(fp);
	return 0;
}