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

enum SYMBOL_NAME
{
	MAIN_FUNCTION,STMTS_AND_FUNCTION_DEFS,LF1,STMT_OR_FUNCTION_DEF,STMT,
	FUNCTION_DEF,DECLARATION_STMT,ASSIGNMENT_STMT,CONDITIONAL_STMT,OTHER_STMTS,
	LF5,IO_STMT,FUN_CALL_STMT,PARAMETER_LIST,TYPE,
	REMAINING_LIST,VAR_LIST,LEFT_HAND_SIDE,RIGHT_HAND_SIDE,ARITHMETIC_EXPR,
	ARITHMETIC_EXPR1,BOOLEAN_EXPR,OP1,TERM,TERM1,
	FACTOR,OP2,VAR,LF2,MATRIX_ASSIGN,SIZE_ASSIGN,ROWS,ROW,LF3,LF4,
	RELATIONAL_OP,MORE_IDS,FUNCTION_CALL,VAR_LIST1,LOGICAL_OP1,LEFT_HAND_SIDE1,
	RIGHT_HAND_SIDE1, EPSILON, ASSIGNOP, FUNID, ID, NUM, RNUM,
	STR, END, INT, REAL, STRING, MATRIX,
	MAIN, SQO, SQC, OP, CL, SEMICOLON, COMMA,
	IF, ELSE, ENDIF, READ, PRINT, FUNCTION,
	PLUS, MINUS, MUL, DIV, SIZE, AND, OR,
	NOT, LT, LE, EQ, GT, GE, NE
};

/*enum SYMBOL_NAME
{
	ASSIGNOP, FUNID, ID, NUM, RNUM,
	STR, END, INT, REAL, STRING, MATRIX,
	MAIN, SQO, SQC, OP, CL, SEMICOLON, COMMA,
	IF, ELSE, ENDIF, READ, PRINT, FUNCTION,
	PLUS, MINUS, MUL, DIV, SIZE, AND, OR,
	NOT, LT, LE, EQ, GT, GE, NE
};*/

typedef struct {
	enum SYMBOL_NAME t_name;
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
	enum SYMBOL_NAME t_name;
	char* lexeme;
	int l;
	long int line_no;
	union Value value;
};

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

typedef enum SYMBOL_NAME SYMBOL_NAME;
typedef union Value Value;
typedef struct Lexeme Lexeme;
typedef struct Token Token;
typedef struct grammar_var grammar_var;
typedef struct node_data node_data;
typedef enum DFA_STATE DFA_STATE;
