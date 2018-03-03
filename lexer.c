// ID: 2015A7PS0355P
// Name: Vignesh N
#include "lexer.h"

// This function removes comments from testcase file and puts the comment free code on console
void removeComments(char* testcaseFile)
{
	FILE* fpt = fopen(testcaseFile,"r");
	if(fpt == NULL)
	{
		printf("input source code not present\n");
		return;
	}
	FILE* fpc = stdout;
	size_t nread;
	char* buffer1 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	buffer1[BUFFER_SIZE - 1] = '?';
	nread = fread(buffer1,sizeof(char),BUFFER_SIZE - 1,fpt);
	char* end_file = NULL;
	if(nread < BUFFER_SIZE - 1)
	{
		// printf("%d\n",nread);
		buffer1[nread] = '!'; // to indicate end of file
		end_file = buffer1 + nread;
	}
	char* fwd = buffer1;
	char* char_read = (char*)malloc(sizeof(char));
	int flag = 0;
	DFA_STATE state = START;
	while(1)
	{
		*char_read = *fwd;
		fwd++;
		switch(*char_read)
		{
			case '?':
				nread = fread(buffer1,sizeof(char),BUFFER_SIZE - 1,fpt);
				fwd = buffer1;
				if(nread < BUFFER_SIZE - 1)
				{
					buffer1[nread] = '!'; // to indicate end of file
					end_file = buffer1 + nread;
				}
				break;
			case '!':
				if(fwd - 1 == end_file)
				{	
					flag = 1;
				}
				break;
		}
		if(flag == 1)
		{
			break;
		}
		if(*char_read == '?')
		{
			continue;
		}
		switch(state)
		{
			case START:
				if(*char_read == '#') state = ACCEPT_COMMENT;
				else
				{
					fwrite(char_read,1,1,fpc);
					// fwrite(char_read,1,1,stdout);
					state = START;
				} 
				break;
			case ACCEPT_COMMENT:
				if(*char_read == '\n')
				{ 
					state = START;
					fwrite(char_read,1,1,fpc);
					// fwrite(char_read,1,1,stdout);
				}
				else state = ACCEPT_COMMENT;
				break;
		}
	}
	free(buffer1);
	fclose(fpt);
}


// This function returns the next lexical token or an error
Token* nextToken(int* diff_buffer, int* buffer_read_into, char** lexemeBegin,char** forward,char** buffer1,char** buffer2,FILE* fp,char* buffer1_end,char* buffer2_end,char** prvs_buff_end,char** curr_buff_start,int* flag,int* line_number,HASH_TABLE* lookupTable,char* ef)
{
	Token* tok = NULL;
	size_t nread;
	char char_read;
	char* end_file = ef;
	DFA_STATE state = START;
	while(1)
	{
		char_read = **forward;							// read a character from buffer
		if(*forward == (char*)(curr_buff_start - 1))
		{
			char_read = '.';			
		}
		*forward = *forward + 1;
		switch(char_read)								// sentinals check
		{
			case '?':
				if(*forward - 1  == buffer1_end || *forward - 1  == buffer2_end)
				{								
					if(*buffer_read_into == 1)
					{
						nread = fread(*buffer2,sizeof(char),BUFFER_SIZE - 1,fp);
						if(nread < BUFFER_SIZE - 1)
						{
							(*buffer2)[nread] = '!'; // to indicate end of file
							end_file = *buffer2 + nread;
						}
						*diff_buffer = 1;
						*forward = *buffer2;
						*curr_buff_start = *buffer2;
						*buffer_read_into = 2;
					}
					else
					{
						nread = fread(*buffer1,sizeof(char),BUFFER_SIZE - 1,fp);
						if(nread < BUFFER_SIZE - 1)
						{
							(*buffer1)[nread] = '!'; // to indicate end of file
							end_file = *buffer1 + nread;
						}
						*diff_buffer = 1;
						*forward = *buffer1;
						*curr_buff_start = *buffer1;
						*buffer_read_into = 1; 
					}
				}
				break;
			case '!':
				if(*forward - 1 == end_file)
				{
					*flag = 1;
				}
				break;
		}
		if(char_read == '?' && *forward == *curr_buff_start)
		{
			continue;
		}
		switch(state)
		{
			// START state
			case START:
				if(char_read == '\t' || char_read == ' ' || char_read == '\r') 
				{
					*lexemeBegin = *forward;
					*diff_buffer = 0;
					state = START;
				}
				else if(char_read == '\n')
				{
					state = START;
					*lexemeBegin = *forward;
					*diff_buffer = 0;
					*line_number += 1;
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
				else
				{
					if(*flag != 1)
					{
						tok = (Token*)malloc(sizeof(Token));
						tok->lexeme = (char*)malloc(sizeof(char));
						strcpy(tok->lexeme,&char_read);
						tok->l = 1;
						tok->line_no = *line_number;
						tok->err = (Error*)malloc(sizeof(Error));
						tok->err->error_no = 2;
						*lexemeBegin = *forward;
						return tok;
					}
					else
					{
						return NULL;
					}
				}
				break;
			
			// COMMENT
			case ACCEPT_COMMENT:
				if(char_read != '\n') state = ACCEPT_COMMENT;
				else
				{
					state = START;
					retract(forward);
				}
				break;


			case ACCEPT_ID1:
				if((char_read >= 'a' && char_read <= 'z') || (char_read >= 'A' && char_read <= 'Z')) state = ACCEPT_ID1;
				else if(char_read >= '0' && char_read <= '9') state = ACCEPT_ID2;
				else 
				{
					tok = processInput(ID,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
					if((tok->l) - 1 > 20)
					{
						tok->err = (Error*)malloc(sizeof(Error));
						tok->err->error_no = 1;
					}
					return tok;
				}
				break;
			
			case ACCEPT_ID2:
				tok = processInput(ID,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				if((tok->l) - 1 > 20)
				{
					tok->err = (Error*)malloc(sizeof(Error));
					tok->err->error_no = 1;
				}
				return tok;
				break;

			case ACCEPT_NUM:
				if(char_read >= '0' && char_read <= '9') state = ACCEPT_NUM;
				else if(char_read == '.') state = INT_RNUM1;
				else
				{
					tok = processInput(NUM,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
					// printf("type:%d name:%s line_no:%ld lexeme_len:%d %d\n",tok->t_name,tok->lexeme,tok->line_no,lexeme->length,(tok->value).int_value);
					return tok;
				}
				break;

			case INT_RNUM1:
				if(char_read >= '0' && char_read <= '9') state = INT_RNUM2;
				else if(char_read == 'a' || char_read == 'o' || char_read == 'n')
				{
					retract(forward);
					tok = processInput(NUM,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
					return tok;
				}
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;
				}
				break;

			case INT_RNUM2:
				if(char_read >= '0' && char_read <= '9') state = ACCEPT_RNUM;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);	
					return tok;	
				}
				break;

			case ACCEPT_RNUM:
				tok = processInput(RNUM,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;

			case INT_STR1:
				if((char_read >= 'a' && char_read <= 'z') || char_read == ' ') state = INT_STR2;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;

			case INT_STR2:
				if((char_read >= 'a' && char_read <= 'z') || char_read == ' ') state = INT_STR2;
				else if(char_read == '"') state = ACCEPT_STR;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;
			
			case ACCEPT_STR:
				tok = processInput(STR,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				if((tok->l) - 1 > 20)
				{
					tok->err = (Error*)malloc(sizeof(Error));
					tok->err->error_no = 1;
				}
				return tok;
				break;

			case INT_F1:
				if((char_read >= 'a' && char_read <= 'z') || (char_read >= 'A' && char_read <= 'Z')) state = ACCEPT_FUNID;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;

			case ACCEPT_FUNID:
				if((char_read >= 'a' && char_read <= 'z') || (char_read >= 'A' && char_read <= 'Z') || (char_read >= '0' && char_read <= '9')) state = ACCEPT_FUNID;
				else
				{
					tok = processInput(FUNID,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
					if((tok->l) - 1 > 20)
					{
						tok->err = (Error*)malloc(sizeof(Error));
						tok->err->error_no = 1;
					}
					return tok;
				}
				break;

			case ACCEPT_LT:
				if(char_read == '=') state = ACCEPT_LE;
				else
				{
					tok = processInput(LT,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
					return tok;
				}
				break;
			case ACCEPT_LE:
				tok = processInput(LE,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;

			case ACCEPT_GT:
				if(char_read == '=') state = ACCEPT_GE;
				else
				{
					tok = processInput(GT,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
					return tok;
				}
				break;

			case ACCEPT_GE:
				tok = processInput(GE,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;
			
			case ACCEPT_ASSIGNOP:
				if(char_read == '/') state = INT_NE;
				else if(char_read == '=') state = ACCEPT_EQ;
				else
				{
					tok = processInput(ASSIGNOP,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
					return tok;
				}
				break;

			case INT_NE:
				if(char_read == '=') state = ACCEPT_NE;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;

			case ACCEPT_EQ:
				tok = processInput(EQ,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;

			case ACCEPT_NE:
				tok = processInput(NE,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;

			case ACCEPT_SQO:
				tok = processInput(SQO,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;
			
			case ACCEPT_SQC:
				tok = processInput(SQC,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;

			case ACCEPT_OP:
				tok = processInput(OP,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;

			case ACCEPT_CL:
				tok = processInput(CL,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;
			case ACCEPT_SC:
				tok = processInput(SEMICOLON,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;
			case ACCEPT_COMMA:
				tok = processInput(COMMA,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;
			case ACCEPT_PLUS:
				tok = processInput(PLUS,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;
			case ACCEPT_MINUS:
				tok = processInput(MINUS,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;
			case ACCEPT_MUL:
				tok = processInput(MUL,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;
			case ACCEPT_DIV:
				tok = processInput(DIV,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;
			case ACCEPT_SIZE:
				tok = processInput(SIZE,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;

			case INT_A1:
				if(char_read == 'a') state = INT_A2;
				else if(char_read == 'o') state = INT_OR1;
				else if(char_read == 'n') state = INT_N1;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;

			case INT_A2:
				if(char_read == 'n') state = INT_A3;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;
			
			case INT_A3:
				if(char_read == 'd') state = INT_A4;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;

			case INT_A4:
				if(char_read == '.') state = ACCEPT_AND;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;

			case ACCEPT_AND:
				tok = processInput(AND,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;

			case INT_OR1:
				if(char_read == 'r') state = INT_OR2;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;
			case INT_OR2:
				if(char_read == '.') state = ACCEPT_OR;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;
			
			case ACCEPT_OR:
				tok = processInput(OR,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;

			case INT_N1:
				if(char_read == 'o') state = INT_N2;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;

			case INT_N2:
				if(char_read == 't') state = INT_N3;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;

			case INT_N3:
				if(char_read == '.') state = ACCEPT_NOT;
				else
				{
					tok = getErrorToken(lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,3);
					return tok;	
				}
				break;

			case ACCEPT_NOT:
				tok = processInput(NOT,lexemeBegin,forward,diff_buffer,*buffer_read_into,buffer1_end,buffer2_end,prvs_buff_end,*curr_buff_start,*line_number,lookupTable);
				return tok;
				break;
		}
		if(*flag == 1)
		{
			break;
		}
	}
	return NULL;
}

// This function return a error token (if error is found) -> this token will not be added to the token list
Token* getErrorToken(char** lexemeBegin,char** forward,int* diff_buffer,int buffer_read_into,char* buffer1_end,char* buffer2_end,char** prvs_buff_end,char* curr_buff_start,int line_number,int error_no)
{
	*prvs_buff_end = getPreviousBufferEnd(*diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
	Lexeme* lexeme = getLexeme(*lexemeBegin,*forward,*diff_buffer,*prvs_buff_end,curr_buff_start);
	retract(forward);
	Token* tok = (Token*)malloc(sizeof(Token));
	tok->lexeme = lexeme->lexeme_name;
	tok->l = lexeme->length;
	tok->line_no = line_number;
	tok->err = (Error*)malloc(sizeof(Error));
	tok->err->error_no = error_no;
	*lexemeBegin = *forward;
	*diff_buffer = 0;
	return tok;	
}

// retract forward pointer
void retract(char** forward)
{
	*forward = *forward - 1;
}

// This function processes the lexeme from lexemeBegin to forward and returns the token thus formed 
Token* processInput(SYMBOL_NAME t_name,char** lexemeBegin,char** forward,int* diff_buffer,int buffer_read_into,char* buffer1_end,char* buffer2_end,char** prvs_buff_end,char* curr_buff_start,int line_number,HASH_TABLE* lookupTable)
{
	retract(forward); // retraction
	*prvs_buff_end = getPreviousBufferEnd(*diff_buffer,buffer_read_into,buffer1_end,buffer2_end);
	Lexeme* lexeme = getLexeme(*lexemeBegin,*forward,*diff_buffer,*prvs_buff_end,curr_buff_start);
	if(t_name == ID || t_name == FUNID)
	{
		keyword* kw = findKeyword(lookupTable,lexeme->lexeme_name);
		if(kw != NULL)
		{
			t_name = kw->t_name;
		}
	}
	Token* tok = createToken(t_name,line_number,lexeme);
	*lexemeBegin = *forward;
	*diff_buffer = 0;
	return tok;
}

// this function is used for buffer management (when a lexeme straddles accross buffers)
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

// this function is used to return the lexeme string between lexemeBegin and forward
Lexeme* getLexeme(char* lexemeBegin,char* forward,int diff_buffer, char* prvs_buff_end,char* curr_buff_start)
{
	Lexeme* lexeme = (Lexeme*)malloc(sizeof(Lexeme));
	if(lexeme == NULL)
	{
		printf("couldnt allocate memory for lexeme\n");
		return NULL;
	}
	int len_lexeme;
	int i;
	if(diff_buffer == 0)
	{
		lexeme->length = (int)(forward-lexemeBegin);
		lexeme->length++;
		char lexeme_name[lexeme->length];
		i = 0;
		for(char *c = lexemeBegin; c != forward; c++)
		{
			lexeme_name[i] = *c;
			i++;
		}
		lexeme->lexeme_name = (char*)malloc(lexeme->length * sizeof(char));
		strcpy(lexeme->lexeme_name,lexeme_name);
		// lexeme->lexeme_name = lexeme_name;
	}
	else
	{
		lexeme->length = (int)(prvs_buff_end - lexemeBegin) + (int)(forward - curr_buff_start);
		lexeme->length++;
		char lexeme_name[lexeme->length];
		i = 0;
		for(char* c = lexemeBegin; c != prvs_buff_end; c++)
		{
			lexeme_name[i] = *c;
			i++;
		}
		for(char* c = curr_buff_start; c != forward; c++)
		{
			lexeme_name[i] = *c;
			i++;
		}
		lexeme->lexeme_name = (char*)malloc(lexeme->length * sizeof(char));
		strcpy(lexeme->lexeme_name,lexeme_name);
	}
	lexeme->lexeme_name[lexeme->length - 1] = '\0';
	return lexeme;
}

// This function creates a new token and assigns the different parameters to struct token variable
Token* createToken(SYMBOL_NAME t_name,long int line_number,Lexeme* lexeme)
{
	Token* tok = (Token*)malloc(sizeof(Token));
	if(tok == NULL)
	{
		printf("memory not allcated for token\n");
		return NULL;
	}
	tok->t_name = t_name;
	tok->line_no = line_number;
	tok->lexeme = lexeme->lexeme_name;
	int len_lexeme = lexeme->length;
	tok->l = len_lexeme;
	tok->err = NULL;
	int i;
	if(t_name == NUM)
	{
		int val = 0;
		for(i = 0; i < len_lexeme - 1; i ++)
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
		while(i < len_lexeme - 1)
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