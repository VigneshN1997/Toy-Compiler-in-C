#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 20

enum TOKEN_NAME
{
	ASSIGNOP, FUNID, ID, NUM, RNUM,
	STR, END, INT, REAL, STRING, MATRIX,
	MAIN, SQO, SQC, OP, CL, SEMICOLON, COMMA,
	IF, ELSE, ENDIF, READ, PRINT, FUNCTION,
	PLUS, MINUS, MUL, DIV, SIZE, AND, OR,
	NOT, LT, LE, EQ, GT, GE, NE
};

enum DFA_STATE
{
	START, ACCEPT_ID1,ACCEPT_ID2,ACCEPT_FUNID,ACCEPT_COMMENT,ACCEPT_NUM,
	ACCEPT_RNUM, ACCEPT_STR, ACCEPT_AND,ACCEPT_OR, ACCEPT_NOT, ACCEPT_LT,ACCEPT_LE,
	ACCEPT_ASSIGNOP, ACCEPT_EQ, ACCEPT_NE, ACCEPT_GT, ACCEPT_GE, ACCEPT_SQC, ACCEPT_SQO,
	ACCEPT_OP,ACCEPT_CL, ACCEPT_SC, ACCEPT_COMMA,ACCEPT_PLUS, ACCEPT_MINUS, ACCEPT_MUL,
	ACCEPT_DIV, ACCEPT_SIZE, INT_ID,INT_F1,INT_F2,INT_COM,INT_NUM,INT_RNUM1,INT_RNUM2,INT_STR1,
	INT_STR2, INT_A1,INT_A2, INT_A3, INT_A4, INT_OR1, INT_OR2,INT_N1,INT_N2,INT_N3,INT_LT,INT_EQ,INT_NE,
	INT_GT,ACCEPT_LINE, ERROR_STATE
};

union Value
{
	int int_value;
	double real_value;
};

struct Token
{
	enum TOKEN_NAME t_name;
	char* lexeme;
	long int line_no;
	union Value value;
};

struct Lexeme
{
	char* lexeme_name;
	int length;
};

typedef union Value Value;
typedef struct Token Token;
typedef enum DFA_STATE DFA_STATE;
typedef enum TOKEN_NAME TOKEN_NAME;
typedef struct Lexeme Lexeme;

void getTokens(FILE* fp);
Token* createToken(TOKEN_NAME t_name,long int line_number,Lexeme* lexeme);
char* getPreviousBufferEnd(int diff_buffer,int buffer_read_into,char* buffer1_end,char* buffer2_end);
Lexeme* getLexeme(char* lexemeBegin,char* forward,int diff_buffer, char* prvs_buff_end,char* curr_buff_start);