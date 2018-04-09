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
};

typedef struct ASTNode ASTNode;