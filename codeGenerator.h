#include "intCodeGen.c"

void initializeFile(FILE* codeFile);
void generateCode(codeNode* codeLines,SymbolTable* symTable,FILE* codeFile);

void createCodeFor_Comparison(threeAddrCode* tac,FILE* codeFile, char* jmpVariant);
void createCodeFor_AssignOp(threeAddrCode* tac,FILE* codeFile);
void createCodeFor_Plus(threeAddrCode* tac,FILE* codeFile);
void createCodeFor_Minus(threeAddrCode* tac,FILE* codeFile);
void createCodeFor_Multiply(threeAddrCode* tac,FILE* codeFile);
