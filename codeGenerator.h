// ID: 2015A7PS0355P
// Name: Vignesh N
#include "intCodeGen.c"

void initializeFile(FILE* codeFile);
void writeExitCode(FILE* codeFile);
void generateCode(codeNode* codeLines,SymbolTable* symTable,FILE* codeFile);

void createCodeFor_Comparison(threeAddrCode* tac,FILE* codeFile, char* jmpVariant);
void createCodeFor_AssignOp(threeAddrCode* tac,FILE* codeFile);
void createCodeFor_Plus(threeAddrCode* tac,FILE* codeFile);
void createCodeFor_Minus(threeAddrCode* tac,FILE* codeFile);
void createCodeFor_Multiply(threeAddrCode* tac,FILE* codeFile);
void createCodeFor_Print(threeAddrCode* tac,FILE* codeFile);
void createCodeFor_Read(threeAddrCode* tac, FILE* codeFile);
void printInteger(int offset,FILE* codeFile);

void writeStringinCode(ASTNode* stringNode, FILE* codeFile, int offset);
void writeStringIDinCode(ASTNode* stringNode,FILE* codeFile, int offset_rhs, int offset_lhs);
void writeMatrixinCode(ASTNode* matrixNode, FILE* codeFile, int offset);
void writeMatrixIDinCode(ASTNode* matrixNode, FILE* codeFile, int offset_rhs, int offset_lhs);

void matrixOperation(ASTNode* matrixID1, ASTNode* matrixID2, int offsetID1, int offsetID2, int resOffset,FILE* codeFile,char* operation);
