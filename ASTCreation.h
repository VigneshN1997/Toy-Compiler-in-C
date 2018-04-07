#include "ASTDef.h"

ASTNode* createASTFromParseTree(ParseTree ptree);

ASTNode* createASTNode(SYMBOL_NAME op,Token* token,ASTNode* children);




// work left
// - parent pointer in AST