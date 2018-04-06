#include "parser.c"

struct ASTNode
{
	SYMBOL_NAME op;
	struct ASTNode* parent;
	struct ASTNode* nextSibling;
	struct ASTNode* children;
	void* ptrToSymTableEntry;
	Token* token;
};

typedef struct ASTNode ASTNode;