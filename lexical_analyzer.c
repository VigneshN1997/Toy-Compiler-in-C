#include "structs.h"
long int line_number = 1;



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
	Token* tok;
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
					forward = buffer2;
					buffer_read_into = 2; 
				}
				else
				{
					nread = fread(buffer1,sizeof(char),BUFFER_SIZE - 1,fp);
					if(nread < BUFFER_SIZE - 1)
					{
						buffer1[nread] = '!'; // to indicate end of file
					}
					forward = buffer1;
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
		if(flag == 1)
		{
			break;
		}
		switch(state)
		{
			// START state
			case START:
				if(char_read == '\t' || char_read == ' ') 
				{
					lexemeBegin = forward;
					state = START;
				}
				else if(char_read == '\n')
				{
					state = START;
					lexemeBegin = forward;
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
					tok = createToken(ID,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
					printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
					lexemeBegin = forward;
				}
				break;
			
			case ACCEPT_ID2:
				state = START;
				forward--; // retraction
				tok = createToken(ID,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;

			case ACCEPT_NUM:
				if(char_read >= '0' && char_read <= '9') state = ACCEPT_NUM;
				else if(char_read == '.') state = INT_RNUM1;
				else
				{
					state = START;
					forward--; // retraction
					tok = createToken(NUM,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
					printf("%d %s %ld %d\n",tok->t_name,tok->lexeme,tok->line_no,(tok->value).int_value);
					lexemeBegin = forward;
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
				tok = createToken(RNUM,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld %lf\n",tok->t_name,tok->lexeme,tok->line_no,(tok->value).real_value);
				lexemeBegin = forward;
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
				tok = createToken(STR,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
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
					tok = createToken(FUNID,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
					printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
					lexemeBegin = forward;
				}
				break;

			case ACCEPT_LT:
				if(char_read == '=') state = ACCEPT_LE;
				else
				{
					state = START;
					forward--; // retraction
					tok = createToken(LT,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
					printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
					lexemeBegin = forward;
				}
				break;
			case ACCEPT_LE:
				state = START;
				forward--; // retraction
				tok = createToken(LE,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;

			case ACCEPT_GT:
				if(char_read == '=') state = ACCEPT_GE;
				else
				{
					state = START;
					forward--; // retraction
					tok = createToken(GT,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
					printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
					lexemeBegin = forward;
				}
				break;

			case ACCEPT_GE:
				state = START;
				forward--; // retraction
				tok = createToken(GE,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;
			
			case ACCEPT_ASSIGNOP:
				if(char_read == '/') state = INT_NE;
				else if(char_read == '=') state = ACCEPT_EQ;
				else
				{
					state = START;
					forward--; // retraction
					tok = createToken(ASSIGNOP,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
					printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
					lexemeBegin = forward;
				}
				break;

			case INT_NE:
				if(char_read == '=') state = ACCEPT_NE;
				else state = ERROR_STATE;
				break;

			case ACCEPT_EQ:
				state = START;
				forward--; // retraction
				tok = createToken(EQ,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;

			case ACCEPT_NE:
				state = START;
				forward--; // retraction
				tok = createToken(NE,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;

			case ACCEPT_SQO:
				state = START;
				forward--; // retraction
				tok = createToken(SQO,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;
			
			case ACCEPT_SQC:
				state = START;
				forward--; // retraction
				tok = createToken(SQC,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;

			case ACCEPT_OP:
				state = START;
				forward--; // retraction
				tok = createToken(OP,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;

			case ACCEPT_CL:
				state = START;
				forward--; // retraction
				tok = createToken(CL,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;
			case ACCEPT_SC:
				state = START;
				forward--; // retraction
				tok = createToken(SEMICOLON,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;
			case ACCEPT_COMMA:
				state = START;
				forward--; // retraction
				tok = createToken(COMMA,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;
			case ACCEPT_PLUS:
				state = START;
				forward--; // retraction
				tok = createToken(PLUS,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;
			case ACCEPT_MINUS:
				state = START;
				forward--; // retraction
				tok = createToken(MINUS,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;
			case ACCEPT_MUL:
				state = START;
				forward--; // retraction
				tok = createToken(MUL,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;
			case ACCEPT_DIV:
				state = START;
				forward--; // retraction
				tok = createToken(DIV,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;
			case ACCEPT_SIZE:
				state = START;
				forward--; // retraction
				tok = createToken(SIZE,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
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
				tok = createToken(AND,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
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
				tok = createToken(OR,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
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
				tok = createToken(NOT,line_number,lexemeBegin,forward); // third argument is 0 because token is not a NUM/RNUM
				printf("%d %s %ld\n",tok->t_name,tok->lexeme,tok->line_no);
				lexemeBegin = forward;
				break;

			case ERROR_STATE:
				printf("error\n");
				break;
		}
	}
}

Token* createToken(TOKEN_NAME t_name,long int line_number,char* lexemeBegin,char* forward)
{
	Token* tok = (Token*)malloc(sizeof(Token));
	tok->t_name = t_name;
	tok->line_no = line_number;
	int len_lexeme = (int)(forward-lexemeBegin);
	tok->lexeme = (char*)malloc(len_lexeme * sizeof(char));
	int i;
	for(i = 0; i < len_lexeme; i++)
	{
		tok->lexeme[i] = *lexemeBegin;
		lexemeBegin++;
	}
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