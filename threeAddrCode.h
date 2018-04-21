// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "symbolTableCreation.c"

enum OPERATOR
{
	PLUS_OP,MINUS_OP,MUL_OP,DIV_OP,ASSIGN_OP,LABEL,
	LT_OP,LE_OP,EQ_OP,GT_OP,GE_OP,NE_OP,GOTO,
	READ_OP,PRINT_OP,ADDR_OP,VALUE_AT_OP
};

struct threeAddrCode
{
	ASTNode* resVar;
	enum OPERATOR op;
	ASTNode* var1;
	ASTNode* var2;
};

struct operator
{
	enum OPERATOR op;
	char* op_str;
};

struct codeNode
{
	struct threeAddrCode* code;
	struct codeNode* nextLine;
};

struct boolCodeAttr
{
	ASTNode* trueLabel;
	ASTNode* falseLabel;
};

typedef struct threeAddrCode threeAddrCode;
typedef struct codeNode codeNode;
typedef struct boolCodeAttr boolCodeAttr;
typedef enum OPERATOR OPERATOR;
typedef struct operator operator;

operator operator_mapping[] = {
	{PLUS_OP,"+"},
	{MINUS_OP,"-"},
	{MUL_OP,"*"},
	{DIV_OP,"/"},
	{ASSIGN_OP,"="},
	{LABEL,"label"},
	{LT_OP,"<"},
	{LE_OP,"<="},
	{EQ_OP,"=="},
	{GT_OP,">"},
	{GE_OP,">="},
	{NE_OP,"!="},
	{GOTO,"goto:"},
	{READ_OP,"read:"},
	{PRINT_OP,"print:"},
	{ADDR_OP,"addr_of:"},
	{VALUE_AT_OP,"value_at:"}
};