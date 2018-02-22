#include "Set.c"
#define num_non_terminals 44
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
	grammar_var* sym;
	enum SYMBOL_NAME lhs_sym;
	struct rhs_node* next;
	struct rhs_node* previous;
};

struct rhs_head
{
	struct rhs_node* first_sym;
	struct rhs_node* last_sym;
	struct rhs_head* next_rule;
};

struct rhs_occurrences
{
	struct rhs_node* ptr_to_rhs_node;
	struct rhs_occurrences* next;
};

struct lhs
{
	enum SYMBOL_NAME sym_name;
	char* name;
	struct rhs_head* first_rule;
	struct rhs_head* last_rule;
	struct rhs_occurrences* rhs_occur; 
	int derives_epsilon;
	Set* first_set;
	Set* follow_set;
};

typedef enum SYMBOL_TYPE SYMBOL_TYPE;
typedef struct grammar_var grammar_var;
typedef struct rhs_node rhs_node;
typedef struct rhs_head rhs_head;
typedef struct rhs_occurrences rhs_occurrences;
typedef struct lhs LHS; 
typedef struct lhs* Grammar;
typedef struct rhs_head*** parsingTable;


grammar_var grammar_var_mapping[] = {
	{"mainFunction",MAIN_FUNCTION,NT},
	{"stmtsAndFunctionDefs",STMTS_AND_FUNCTION_DEFS,NT},
	{"moreStmtsorFunctionDefs",MORE_STMTS_OR_FUNCTION_DEFS,NT},
	{"stmtOrFunctionDef",STMT_OR_FUNCTION_DEF,NT},
	{"stmt",STMT,NT},
	{"functionDef",FUNCTION_DEF,NT},
	{"parameter_list",PARAMETER_LIST,NT},
	{"type",TYPE,NT},
	{"remainingList",REMAINING_LIST,NT},
	{"declarationStmt",DECLARATION_STMT,NT},
	{"var_list",VAR_LIST,NT},
	{"more_ids",MORE_IDS,NT},
	{"assignmentStmt_type1",ASSIGNMENT_STMT_TYPE1,NT},
	{"assignmentStmt_type2",ASSIGNMENT_STMT_TYPE2,NT},
	{"leftHandSide_singleVar",LHS_SINGLE_VAR,NT},
	{"leftHandSide_listVar",LHS_LIST_VAR,NT},
	{"rightHandSide_type1",RHS_TYPE1,NT},
	{"rightHandSide_type2",RHS_TYPE2,NT},
	{"sizeExpression",SIZE_EXPR,NT},
	{"ifStmt",IF_STMT,NT},
	{"elseFactor",ELSE_FACTOR,NT},
	{"otherStmts",OTHER_STMTS,NT},
	{"ioStmt",IO_STMT,NT},
	{"funCallStmt",FUN_CALL_STMT,NT},
	{"inputParameterList",INPUT_PARAMETER_LIST,NT},
	{"listVar",LIST_VAR,NT},
	{"arithmeticExpression",ARITHMETIC_EXPR,NT},
	{"arithmeticExprLF",ARITHMETIC_EXPR_LF,NT},
	{"arithmeticTerm",ARITHMETIC_TERM,NT},
	{"arithmeticTermLF",ARITHMETIC_TERM_LF,NT},
	{"factor",FACTOR,NT},
	{"operator_lowPrecedence",OPERATOR_LOW_PREC,NT},
	{"operator_highPrecedence",OPERATOR_HIGH_PREC,NT},
	{"booleanExpression",BOOL_EXPR,NT},
	{"constrainedVars",CONSTRAINED_VARS,NT},
	{"var",VAR,NT},
	{"matrix",MATRIX_DEF,NT},
	{"rows",ROWS,NT},
	{"rowsLF",ROWS_LF,NT},
	{"row",ROW,NT},
	{"rowLF",ROW_LF,NT},
	{"isMatrixElement",IS_MATRIX_ELEM,NT},
	{"logicalOp",LOGICAL_OP,NT},
	{"relationalOp",RELATIONAL_OP,NT},
	{"EPSILON",EPSILON,T},
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
	{"$",$,T}
};