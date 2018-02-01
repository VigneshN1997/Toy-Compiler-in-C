#include "structs.h"
long int line_number = 1;

void getTokens(FILE* fp)
{
	int diff_buffer = 0; 								// if lexemeBegin and forward are in the same buffer or not
	size_t nread;										// number of characters read from file	
	int buffer_read_into = 1;							// buffer number into which characters are read
	char* lexemeBegin,*forward,*buffer1,*buffer2;
	char char_read;
	buffer1 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	buffer2 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	buffer1[BUFFER_SIZE - 1] = '?'; 					// to indicate end of buffers
	buffer2[BUFFER_SIZE - 1] = '?';
	nread = fread(buffer1,sizeof(char),BUFFER_SIZE - 1,fp);
	if(nread < BUFFER_SIZE - 1)
	{
		buffer1[nread] = '!'; // to indicate end of file
	}
	lexemeBegin = buffer1;								// initialize the lexemeBegin pointer		
	forward = buffer1;
	DFA_STATE state = START;							
	char* buffer1_end = buffer1 + BUFFER_SIZE - 1;		
	char* buffer2_end = buffer2 + BUFFER_SIZE - 1;
	char* prvs_buff_end,*curr_buff_start;
	Lexeme* lexeme;
	curr_buff_start = buffer1;
	int flag = 0;										// to terminate out of loop once eof is reached
	Token* tok;
	while(1)
	{
		char_read = *forward++;							// read a character from buffer
		switch(char_read)								// sentinals check
		{
			case '?':									
				if(buffer_read_into == 1)
				{
					nread = fread(buffer2,sizeof(char),BUFFER_SIZE - 1,fp);
					if(nread < BUFFER_SIZE - 1)
					{
						buffer2[nread] = '!'; // to indicate end of file
					}
					diff_buffer = 1;
					forward = buffer2;
					curr_buff_start = buffer2;
					buffer_read_into = 2;
				}
				else
				{
					nread = fread(buffer1,sizeof(char),BUFFER_SIZE - 1,fp);
					if(nread < BUFFER_SIZE - 1)
					{
						buffer1[nread] = '!'; // to indicate end of file
					}
					diff_buffer = 1;
					forward = buffer1;
					curr_buff_start = buffer1;
					buffer_read_into = 1; 
				}
				break;
			case '!':
				flag = 1;
				break;
		}
		if(char_read == '?')
		{
			continue;
		}
		switch(state)
		{
			// START state
			case START:
				if(char_read == '\t' || char_read == ' ') 
				{
					lexemeBegin = forward;
					diff_buffer = 0;
					state = START;
				}
				else if(char_read == '\n')
				{
					state = START;
					lexemeBegin = forward;
					diff_buffer = 0;
					line_number += 1;
				}
				else if((char_read >= 'a' && char_read <= 'z') || (char_read >= 'A' && char_read <= 'Z')) state = ACCEPT_ID1;
				else if(char_read == '_') state = INT_F1;
				else if(char_read == '#') state = ACCEPT_COMMENT;
				else if(char_read >= '0' && char_read <= '9') state = ACCEPT_NUM;
				else if(char_read == '"') state = INT_STR1;
				else if(char_read == '.') state = INT_A1;
				else if(char_read == '<') state = ACCEPT_LT;
				else if(char_read == '=') state = ACCEPT_ASSIGNOP;
				else if(char_read == '>') state = ACCEPT_GT;
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
				else state = ERROR_STATE;
				break;
			
			// COMMENT
			case ACCEPT_COMMENT:
				if(char_read != '\n') state = ACCEPT_COMMENT;
				else
				{
					state = START;
					forward--;
				}
				break;


			case ACCEPT_ID1:
				if((char_read >= 'a' && char_read <= 'z') || (char_read >= 'A' && char_read <= 'Z')) state = ACCEPT_ID1;
				else if(char_read >= '0' && char_read <= '9') state = ACCEPT_ID2;
				else 
				{
					state = START;
					forward--; // retraction
					prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
					lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
					tok = createToken(ID,line_number,lexeme);
					printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
					lexemeBegin = forward;
					diff_buffer = 0;
				}
				break;
			
			case ACCEPT_ID2:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(ID,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case ACCEPT_NUM:
				if(char_read >= '0' && char_read <= '9') state = ACCEPT_NUM;
				else if(char_read == '.') state = INT_RNUM1;
				else
				{
					state = START;
					forward--; // retraction
					prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
					lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
					tok = createToken(NUM,line_number,lexeme);
					printf("type:%d name:%s line_no:%ld lexeme_len:%d %d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length,(tok->value).int_value);
					lexemeBegin = forward;
					diff_buffer = 0;
				}
				break;

			case INT_RNUM1:
				if(char_read >= '0' && char_read <= '9') state = INT_RNUM2;
				else state = ERROR_STATE; // 
				break;

			case INT_RNUM2:
				if(char_read >= '0' && char_read <= '9') state = ACCEPT_RNUM;
				else state = ERROR_STATE; //
				break;

			case ACCEPT_RNUM:
			 	state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(RNUM,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d %lf\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length,(tok->value).real_value);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case INT_STR1:
				if(char_read >= 'a' && char_read <= 'z') state = INT_STR2;
				else state = ERROR_STATE;
				break;

			case INT_STR2:
				if(char_read >= 'a' && char_read <= 'z') state = INT_STR2;
				else if(char_read == '"') state = ACCEPT_STR;
				else state = ERROR_STATE;
				break;
			
			case ACCEPT_STR:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(STR,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case INT_F1:
				if(char_read >= 'a' && char_read <= 'z') state = ACCEPT_FUNID;
				else state = ERROR_STATE;
				break;

			case ACCEPT_FUNID:
				if((char_read >= 'a' && char_read <= 'z') || (char_read >= 'A' && char_read <= 'Z') || (char_read >= '0' && char_read <= '9')) state = ACCEPT_FUNID;
				else
				{
					state = START;
					forward--; // retraction
					prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
					lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
					tok = createToken(FUNID,line_number,lexeme);
					printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
					lexemeBegin = forward;
					diff_buffer = 0;
				}
				break;

			case ACCEPT_LT:
				if(char_read == '=') state = ACCEPT_LE;
				else
				{
					state = START;
					forward--; // retraction
					prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
					lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
					tok = createToken(LT,line_number,lexeme);
					printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
					lexemeBegin = forward;
					diff_buffer = 0;
				}
				break;
			case ACCEPT_LE:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(LE,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case ACCEPT_GT:
				if(char_read == '=') state = ACCEPT_GE;
				else
				{
					state = START;
					forward--; // retraction
					prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
					lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
					tok = createToken(GT,line_number,lexeme);
					printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
					lexemeBegin = forward;
					diff_buffer = 0;
				}
				break;

			case ACCEPT_GE:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(GE,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;
			
			case ACCEPT_ASSIGNOP:
				if(char_read == '/') state = INT_NE;
				else if(char_read == '=') state = ACCEPT_EQ;
				else
				{
					state = START;
					forward--; // retraction
					prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
					lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
					tok = createToken(ASSIGNOP,line_number,lexeme);
					printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
					lexemeBegin = forward;
					diff_buffer = 0;
				}
				break;

			case INT_NE:
				if(char_read == '=') state = ACCEPT_NE;
				else state = ERROR_STATE;
				break;

			case ACCEPT_EQ:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(EQ,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case ACCEPT_NE:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(NE,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case ACCEPT_SQO:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(SQO,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;
			
			case ACCEPT_SQC:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(SQC,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case ACCEPT_OP:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(OP,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case ACCEPT_CL:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(CL,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;
			case ACCEPT_SC:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(SEMICOLON,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;
			case ACCEPT_COMMA:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(COMMA,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;
			case ACCEPT_PLUS:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(PLUS,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;
			case ACCEPT_MINUS:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(MINUS,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;
			case ACCEPT_MUL:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(MUL,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;
			case ACCEPT_DIV:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(DIV,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;
			case ACCEPT_SIZE:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(SIZE,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case INT_A1:
				if(char_read == 'a') state = INT_A2;
				else if(char_read == 'o') state = INT_OR1;
				else if(char_read == 'n') state = INT_N1;
				else state = ERROR_STATE;
				break;

			case INT_A2:
				if(char_read == 'n') state = INT_A3;
				else state = ERROR_STATE;
				break;
			
			case INT_A3:
				if(char_read == 'd') state = INT_A4;
				else state = ERROR_STATE;
				break;

			case INT_A4:
				if(char_read == '.') state = ACCEPT_AND;
				else state = ERROR_STATE;
				break;

			case ACCEPT_AND:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(AND,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case INT_OR1:
				if(char_read == 'r') state = INT_OR2;
				else state = ERROR_STATE;
				break;
			case INT_OR2:
				if(char_read == '.') state = ACCEPT_OR;
				else state = ERROR_STATE;
				break;
			
			case ACCEPT_OR:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(OR,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case INT_N1:
				if(char_read == 'o') state = INT_N2;
				else state = ERROR_STATE;
				break;

			case INT_N2:
				if(char_read == 't') state = INT_N3;
				else state = ERROR_STATE;
				break;

			case INT_N3:
				if(char_read == '.') state = ACCEPT_NOT;
				else state = ERROR_STATE;
				break;

			case ACCEPT_NOT:
				state = START;
				forward--; // retraction
				prvs_buff_end = getPreviousBufferEnd(diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
				lexeme = getLexeme(lexemeBegin,forward,diff_buffer,prvs_buff_end,curr_buff_start);
				tok = createToken(NOT,line_number,lexeme);
				printf("type:%d name:%s line_no:%ld lexeme_len:%d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length);
				lexemeBegin = forward;
				diff_buffer = 0;
				break;

			case ERROR_STATE:
				printf("error\n");
				break;
		}
		if(flag == 1)
		{
			break;
		}
	}
}

// this function returns the pointer to end of the buffer which is needed to extract the lexeme name(if it is spread across 2 buffers)
char* getPreviousBufferEnd(int diff_buffer,int buffer_read_into,char* buffer1_end,char* buffer2_end)
{
	if(diff_buffer == 1)
	{
		if(buffer_read_into == 1)
		{
			return buffer2_end;
		}
		else
		{
			return buffer1_end;
		}
	}
	if(buffer_read_into == 1)
	{
		return buffer1_end;
	}
	return buffer2_end;
}

// this function returns the lexeme name from lexemeBegin to forward pointer
Lexeme* getLexeme(char* lexemeBegin,char* forward,int diff_buffer, char* prvs_buff_end,char* curr_buff_start)
{
	Lexeme* lexeme = (Lexeme*)malloc(sizeof(Lexeme));
	int len_lexeme;
	int i;
	if(diff_buffer == 0)
	{
		lexeme->length = (int)(forward-lexemeBegin);
		lexeme->lexeme_name = (char*)malloc(len_lexeme * sizeof(char));
		i = 0;
		for(char *c = lexemeBegin; c != forward; c++)
		{
			lexeme->lexeme_name[i] = *c;
			i++;
		}
	}
	else
	{
		lexeme->length = (int)(prvs_buff_end - lexemeBegin) + (int)(forward - curr_buff_start);
		lexeme->lexeme_name = (char*)malloc(len_lexeme * sizeof(char));
		i = 0;
		for(char* c = lexemeBegin; c != prvs_buff_end; c++)
		{
			lexeme->lexeme_name[i] = *c;
			i++;
		}
		for(char* c = curr_buff_start; c != forward; c++)
		{
			lexeme->lexeme_name[i] = *c;
			i++;
		}
	}
	return lexeme;
}

// this function is used to create a new token
Token* createToken(TOKEN_NAME t_name,long int line_number,Lexeme* lexeme)
{
	Token* tok = (Token*)malloc(sizeof(Token));
	tok->t_name = t_name;
	tok->line_no = line_number;
	tok->lexeme = lexeme->lexeme_name;
	int len_lexeme = lexeme->length;
	int i;
	if(t_name == NUM)
	{
		int val = 0;
		for(i = 0; i < len_lexeme; i ++)
		{
			val = val*10 + (tok->lexeme[i] - '0');
		}
		(tok->value).int_value = val;
	}
	else if(t_name == RNUM)
	{
		int int_part = 0;
		double frac_part = 0;
		i = 0;
		while(tok->lexeme[i] != '.')
		{
			int_part = int_part*10 + (tok->lexeme[i] - '0');
			i++;
		}
		i++;
		double p = 1;
		while(i < len_lexeme)
		{
			frac_part =  frac_part*10 + (tok->lexeme[i] - '0');
			p = p*10;
			i++;
		}
		frac_part = frac_part/p;
		(tok->value).real_value = int_part + frac_part;
	}
	return tok;
}


int main(int argc, char const *argv[])
{
	FILE* fp = fopen("testcase.txt","r");
	getTokens(fp);
	return 0;
}