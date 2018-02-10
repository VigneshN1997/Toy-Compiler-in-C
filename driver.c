#include "lexer.c"
int main(int argc, char const *argv[])
{
	FILE* fp = fopen("testcase.txt","r");
	int diff_buffer = 0;
	int buffer_read_into = 1;
	size_t nread;
	int line_number = 1;
	char *lexemeBegin, *forward, *buffer1, *buffer2;
	buffer1 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	buffer2 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	buffer1[BUFFER_SIZE - 1] = '?'; 					// to indicate end of buffers
	buffer2[BUFFER_SIZE - 1] = '?';
	nread = fread(buffer1,sizeof(char),BUFFER_SIZE - 1,fp);
	if(nread < BUFFER_SIZE - 1)
	{
		// printf("%d\n",nread);
		buffer1[nread] = '!'; // to indicate end of file
	}
	lexemeBegin = buffer1;								// initialize the lexemeBegin pointer		
	forward = buffer1;
	char* buffer1_end = buffer1 + BUFFER_SIZE - 1;		
	char* buffer2_end = buffer2 + BUFFER_SIZE - 1;
	char* prvs_buff_end,*curr_buff_start;
	Lexeme* lexeme;
	curr_buff_start = buffer1;
	prvs_buff_end = buffer1_end;
	int flag = 0;
	Token* tok = NULL;
	HASH_TABLE* lookupTable = getLookupTable();
	// printLookupTable(lookupTable);
	while(1)
	{
		tok = getNextToken(&diff_buffer,&buffer_read_into,&lexemeBegin,&forward,&buffer1,&buffer2,fp,buffer1_end,buffer2_end,&prvs_buff_end,&curr_buff_start,&flag,&line_number,lookupTable);
		if(tok == NULL)
		{
			break;
		}
		if(tok->t_name == NUM)
		{
			printf("type:%d name:%s line_no:%ld lexeme_len:%d %d\n",tok->t_name,tok->lexeme,tok->line_no,tok->l,(tok->value).int_value);
		}
		else if(tok->t_name == RNUM)
		{
			printf("type:%d name:%s line_no:%ld lexeme_len:%d %d\n",tok->t_name,tok->lexeme,tok->line_no,tok->l,(tok->value).real_value);
		}
		else
		{
			printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,tok->l);
		}
	}
	return 0;
}