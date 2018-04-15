#include "threeAddrCode.h"

void createIntermediateCode(ASTNode* asTree,SymbolTable* symTable, int* tempVarNum,int* labelVarNum);
void appendAllStatements(ASTNode* mainNode);

void generateCodeFor_AssignmentStmtSingleVar(ASTNode* assignStmt, SymbolTable* symTable, int* tempVarNum);
void generateCodeFor_IfStmt(ASTNode* ifStmt,SymbolTable* symTable,int* tempVarNum,int* labelVarNum);
void generateCodeFor_BoolExpr(ASTNode* boolExpr, int* labelVarNum);
void generateCodeFor_ArithmeticExpr(ASTNode* arithmeticExpr,SymbolTable* symTable,int* tempVarNum);

ASTNode* newLabelVar(int* labelVarNum);
ASTNode* newTempVar(int* tempVarNum,SymbolTable* symTable, SYMBOL_NAME type);
codeNode* generateThreeAddrCode(ASTNode* resVar, OPERATOR op,ASTNode* var1, ASTNode* var2);
codeNode* appendCodes(codeNode* c1, codeNode* c2);


void printIntCode(codeNode* c);
