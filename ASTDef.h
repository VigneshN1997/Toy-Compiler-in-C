// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "parser.c"

struct ASTNode
{
	SYMBOL_NAME op;
	struct ASTNode* parent;
	struct ASTNode* nextSibling;
	struct ASTNode* children;
	void* ptrToSymTableEntry;
	SYMBOL_NAME type;
	int* widthInfo; // for matrices and strings
	Token* token;
	struct ASTNode* tempVar; // for intermediate code generation
	void* code;
	void* attrBool;				// true false attributes in boolean expression
	struct ASTNode* nextLabel; // for labels in if stmts
};

typedef struct ASTNode ASTNode;