#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 4096
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
	MAIN_FUNCTION,STMTS_AND_FUNCTION_DEFS,MORE_STMTS_OR_FUNCTION_DEFS,
	STMT_OR_FUNCTION_DEF,STMT,FUNCTION_DEF,PARAMETER_LIST,TYPE,REMAINING_LIST,
	DECLARATION_STMT,VAR_LIST,MORE_IDS,ASSIGNMENT_STMT_TYPE1,ASSIGNMENT_STMT_TYPE2,
	LHS_SINGLE_VAR,LHS_LIST_VAR,RHS_TYPE1,RHS_TYPE2,SIZE_EXPR,IF_STMT,ELSE_FACTOR,
	OTHER_STMTS,IO_STMT,FUN_CALL_STMT,INPUT_PARAMETER_LIST,LIST_VAR,ARITHMETIC_EXPR,
	ARITHMETIC_EXPR_LF,ARITHMETIC_TERM,ARITHMETIC_TERM_LF,FACTOR,OPERATOR_LOW_PREC,
	OPERATOR_HIGH_PREC,BOOL_EXPR, CONSTRAINED_VARS,VAR,MATRIX_DEF,ROWS,ROWS_LF,ROW,ROW_LF,
	IS_MATRIX_ELEM,LOGICAL_OP,RELATIONAL_OP,EPSILON, ASSIGNOP, FUNID, ID, NUM, RNUM,
	STR, END, INT, REAL, STRING, MATRIX,MAIN, SQO, SQC, OP, CL, SEMICOLON, COMMA,
	IF, ELSE, ENDIF, READ, PRINT, FUNCTION,PLUS, MINUS, MUL, DIV, SIZE, AND, OR,
	NOT, LT, LE, EQ, GT, GE, NE,$
};

enum SYMBOL_TYPE
{	
	T, // terminal
	NT // non-terminal
};

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
	int line_no;
	union Value value;
};

struct grammar_var
{
	char* sym_str;
	enum SYMBOL_NAME sym_name;
	enum SYMBOL_TYPE sym_type;
};

struct node_data
{
	struct Token* token;			// lexical token
	struct grammar_var* variable;	// grammar variable
	void* ptr_to_ParseTreeNode;
};


typedef enum SYMBOL_NAME SYMBOL_NAME;
typedef enum SYMBOL_TYPE SYMBOL_TYPE;
typedef union Value Value;
typedef struct Lexeme Lexeme;
typedef struct Token Token;
typedef struct grammar_var grammar_var;
typedef struct node_data node_data;
typedef enum DFA_STATE DFA_STATE;
