#include "parser.c"

struct ASTNode
{
	enum SYMBOL_NAME operation;
	struct ASTNode* parent;
	struct ASTNode* nextSibling;
	struct ASTNode* children;
	void* ptrToSymTableEntry;
};