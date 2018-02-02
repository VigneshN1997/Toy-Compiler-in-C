#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
enum SYMBOL_TYPE
{	
	T, // terminal
	NT // non-terminal
};

struct stack_symbol
{
	enum SYMBOL_TYPE sym_type;
}

typedef enum TOKEN_NAME TOKEN_NAME;
typedef struct SYMBOL_TYPE SYMBOL_TYPE;
typedef struct stack_symbol stack_symbol;