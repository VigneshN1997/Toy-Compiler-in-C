#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 100
#define table_size 24 // lookup table

enum DFA_STATE
{
	START, ACCEPT_ID1,ACCEPT_ID2,ACCEPT_FUNID,ACCEPT_COMMENT,ACCEPT_NUM,
	ACCEPT_RNUM, ACCEPT_STR, ACCEPT_AND,ACCEPT_OR, ACCEPT_NOT, ACCEPT_LT,ACCEPT_LE,
	ACCEPT_ASSIGNOP, ACCEPT_EQ, ACCEPT_NE, ACCEPT_GT, ACCEPT_GE, ACCEPT_SQC, ACCEPT_SQO,
	ACCEPT_OP,ACCEPT_CL, ACCEPT_SC, ACCEPT_COMMA,ACCEPT_PLUS, ACCEPT_MINUS, ACCEPT_MUL,
	ACCEPT_DIV, ACCEPT_SIZE,INT_F1,INT_RNUM1,INT_RNUM2,INT_STR1, INT_STR2, INT_A1,INT_A2, 
	INT_A3, INT_A4, INT_OR1, INT_OR2,INT_N1,INT_N2,INT_N3,INT_NE,ERROR_STATE
};

enum TOKEN_NAME
{
	ASSIGNOP, FUNID, ID, NUM, RNUM,
	STR, END, INT, REAL, STRING, MATRIX,
	MAIN, SQO, SQC, OP, CL, SEMICOLON, COMMA,
	IF, ELSE, ENDIF, READ, PRINT, FUNCTION,
	PLUS, MINUS, MUL, DIV, SIZE, AND, OR,
	NOT, LT, LE, EQ, GT, GE, NE
};

typedef struct {
	enum TOKEN_NAME t_name;
	char* name;
}keyword;

keyword keyword_mapping [] = {
	{END,"end"},
	{INT,"int"},
	{REAL,"real"},
	{STRING,"string"},
	{MATRIX,"matrix"},
	{MAIN,"_main"},
	{IF,"if"},
	{ELSE,"else"},
	{ENDIF,"endif"},
	{READ,"read"},
	{PRINT,"print"},
	{FUNCTION,"function"}
};

union Value
{
	int int_value;
	double real_value;
};


struct Lexeme
{
	char* lexeme_name;
	int length;
};

struct Token
{
	enum TOKEN_NAME t_name;
	char* lexeme;
	int l;
	long int line_no;
	union Value value;
};

// enum SYMBOL_NAME
// {
// 	A,B,C
// };

// enum SYMBOL_TYPE
// {	
// 	T, // terminal
// 	NT // non-terminal
// };

// struct grammar_var
// {
// 	enum SYMBOL_TYPE sym_type;
// 	enum SYMBOL_NAME sym_name;
// };

// struct node_data
// {
// 	struct Token* token;			// lexical token
// 	struct grammar_var* variable;	// grammar variable
// };

typedef enum TOKEN_NAME TOKEN_NAME;
typedef enum SYMBOL_TYPE SYMBOL_TYPE;
typedef enum SYMBOL_NAME SYMBOL_NAME;
typedef union Value Value;
typedef struct Lexeme Lexeme;
typedef struct Token Token;
typedef struct grammar_var grammar_var;
typedef struct node_data node_data;
typedef enum DFA_STATE DFA_STATE;
