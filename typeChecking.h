// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "symbolTableDef.h"

void doTypeChecking(ASTNode* asTree, SymbolTable* symTable,errorHead* typeCheckingErrorsHead);

void typeCheckAssignmentStmtSingleVar(ASTNode* assgnStmt,SymbolTable* symTable, errorHead* typeCheckingErrorsHead);
void typeCheckAssignmentStmtListVar(ASTNode* assgnStmt,SymbolTable* symTable, errorHead* typeCheckingErrorsHead);
void typeCheckFunCallStmt(ASTNode* lhs,int numLHSVars,ASTNode* funCallStmt,SymbolTable* symTable,errorHead* typeCheckingErrorsHead);
void typeCheckFunction(ASTNode* func, SymbolTable* symTable,errorHead* typeCheckingErrorsHead);
void typeCheckBooleanExpr(ASTNode* boolExpr,SymbolTable* symTable,errorHead* typeCheckingErrorsHead);
void typeCheckArithmeticExpr(ASTNode* arithmeticExpr,SymbolTable* symTable,errorHead* typeCheckingErrorsHead);
void typeCheckIOStmt(ASTNode* ioStmt,SymbolTable* symTable,errorHead* typeCheckingErrorsHead);

void checkOuputParametersAssignment(ASTNode* funcStmts,ASTNode* opParameterList,int* assigned,int level,SymbolTable* funcSymTable,SymbolTable* currSymTable);
int recursionPresent(ASTNode* funCallStmt, SymbolTable* symTable);

int* extractStringSize(ASTNode* stringNode);
int* extractMatrixSize(ASTNode* matrixNode,errorHead* typeCheckingErrorsHead);

symbolTableEntry* findIdorFuncName(char* findId,SymbolTable* symTable);