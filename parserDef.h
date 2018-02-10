#include "lexer.c"

enum SYMBOL_TYPE
{	
	T, // terminal
	NT // non-terminal
};

struct grammar_var
{
	char* sym_str;
	enum SYMBOL_NAME sym_name;
	enum SYMBOL_TYPE sym_type;
};

// rhs rhs_node
struct rhs_node
{
	enum SYMBOL_NAME sym_name;
	enum SYMBOL_NAME lhs_sym;
	enum SYMBOL_TYPE sym_type;
	struct rhs_node* next;
};

struct rhs_occurrences
{
	struct rhs_node* ptr_to_rhs_node;
	struct rhs_occurrences* next;
};

struct lhs
{
	enum SYMBOL_NAME sym_name;
	struct rhs_node* first_sym;
	struct rhs_occurrences* rhs_occur; 
};

typedef enum SYMBOL_TYPE SYMBOL_TYPE;
typedef enum SYMBOL_NAME SYMBOL_NAME;
typedef struct grammar_var grammar_var;
typedef struct rhs_node NODE;
typedef struct rhs_occurrences rhs_occurrences;
typedef struct lhs LHS; 

grammar_var grammar_var_mapping[] = {
	{"mainFunction",MAIN_FUNCTION,NT},
	{"stmtsAndFunctionDefs",STMTS_AND_FUNCTION_DEFS,NT},
	{"LF1",LF1,NT},
	{"stmtOrFunctionDef",STMT_OR_FUNCTION_DEF,NT},
	{"stmt",STMT,NT},
	{"functionDef",FUNCTION_DEF,NT},
	{"parameter_list",PARAMETER_LIST,NT},
	{"type",TYPE,NT},
	{"remainingList",REMAINING_LIST,NT},
	{"declarationStmt",DECLARATION_STMT,NT},
	{"assignmentStmt",ASSIGNMENT_STMT,NT},
	{"leftHandSide",LEFT_HAND_SIDE,NT},
	{"leftHandSide1",LEFT_HAND_SIDE1,NT},
	{"rightHandSide1",RIGHT_HAND_SIDE1,NT},
	{"rightHandSide",RIGHT_HAND_SIDE,NT},
	{"conditionalStmt",CONDITIONAL_STMT,NT},
	{"LF5",LF5,NT},
	{"otherStmts",OTHER_STMTS,NT},
	{"ioStmt",IO_STMT,NT},
	{"arithmeticExpression",ARITHMETIC_EXPR,NT},
	{"arithmeticExpression1",ARITHMETIC_EXPR1,NT},
	{"OP1",OP1,NT},
	{"term",TERM,NT},
	{"term1",TERM1,NT},
	{"OP2",OP2,NT},
	{"factor",FACTOR,NT},
	{"var",VAR,NT},
	{"LF2",LF2,NT},
	{"matrixAssign",MATRIX_ASSIGN,NT},
	{"rows",ROWS,NT},
	{"LF3",LF3,NT},
	{"row",ROW,NT},
	{"LF4",LF4,NT},
	{"sizeAssign",SIZE_ASSIGN,NT},
	{"booleanExpression",BOOLEAN_EXPR,NT},
	{"logicalOp1",LOGICAL_OP1,NT},
	{"relationalOp",RELATIONAL_OP,NT},
	{"var_list",VAR_LIST,NT},
	{"more_ids",MORE_IDS,NT},
	{"funCallStmt",FUN_CALL_STMT,NT},
	{"functionCall",FUNCTION_CALL,NT},
	{"var_list1",VAR_LIST1,NT},
	{"ASSIGNOP",ASSIGNOP,T},
	{"FUNID",FUNID,T},
	{"ID",ID,T},
	{"NUM",NUM,T},
	{"RNUM",RNUM,T},
	{"STR",STR,T},
	{"END",END,T},
	{"INT",INT,T},
	{"REAL",REAL,T},
	{"STRING",STRING,T},
	{"MATRIX",MATRIX,T},
	{"MAIN",MAIN,T},
	{"SQO",SQO,T},
	{"SQC",SQC,T},
	{"OP",OP,T},
	{"CL",CL,T},
	{"SEMICOLON",SEMICOLON,T},
	{"COMMA",COMMA,T},
	{"IF",IF,T},
	{"ELSE",ELSE,T},
	{"ENDIF",ENDIF,T},
	{"READ",READ,T},
	{"PRINT",PRINT,T},
	{"FUNCTION",FUNCTION,T},
	{"PLUS",PLUS,T},
	{"MINUS",MINUS,T},
	{"MUL",MUL,T},
	{"DIV",DIV,T},
	{"SIZE",SIZE,T},
	{"AND",AND,T},
	{"OR",OR,T},
	{"NOT",NOT,T},
	{"LT",LT,T},
	{"LE",LE,T},
	{"EQ",EQ,T},
	{"GT",GT,T},
	{"GE",GE,T},
	{"NE",NE,T},
	{"EPSILON",EPSILON,T}
};