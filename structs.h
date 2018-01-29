#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

enum TOKEN_NAME
{
	ASSIGNOP, FUNID, ID, NUM, RNUM,
	STR, END, INT, REAL, STRING, MATRIX,
	MAIN, SQO, SQC, OP, CL, SEMICOLON, COMMA,
	IF, ELSE, ENDIF, READ, PRINT, FUNCTION,
	PLUS, MINUS, MUL, DIV, SIZE, AND, OR,
	NOT, LT, LE, EQ, GT, GE, NE
};

struct Token
{
	enum TOKEN_NAME t_name;
	char* lexeme;
	int line_no;
	int int_value;
	double real_value;
};


typedef struct Token Token;
typedef enum TOKEN_NAME TOKEN_NAME;