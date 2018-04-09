#include "symbolTableCreation.c"

void doTypeChecking(ASTNode* asTree, SymbolTable* symTable,errorHead* typeCheckingErrorsHead);

void typeCheckAssignmentStmtSingleVar(ASTNode* assgnStmt,SymbolTable* symTable, errorHead* typeCheckingErrorsHead);
void typeCheckAssignmentStmtListVar(ASTNode* assgnStmt,SymbolTable* symTable, errorHead* typeCheckingErrorsHead);
void typeCheckIfStmt(ASTNode* ifStmt,SymbolTable* symTable,errorHead* typeCheckingErrorsHead);
void typeCheckFunCallStmt(ASTNode* lhs,int numLHSVars,ASTNode* funCallStmt,SymbolTable* symTable,errorHead* typeCheckingErrorsHead);
void typeCheckFunction(ASTNode* func, SymbolTable* symTable,errorHead* typeCheckingErrorsHead);
void typeCheckBooleanExpr(ASTNode* boolExpr,SymbolTable* symTable,errorHead* typeCheckingErrorsHead);
void typeCheckArithmeticExpr(ASTNode* arithmeticExpr,SymbolTable* symTable,errorHead* typeCheckingErrorsHead);


void checkOuputParametersAssignment(ASTNode* funcStmts,ASTNode* opParameterList,int* assigned,int level,SymbolTable* funcSymTable,SymbolTable* currSymTable);
int recursionPresent(ASTNode* funCallStmt, SymbolTable* symTable);

int* extractStringSize(ASTNode* stringNode);
int* extractMatrixSize(ASTNode* matrixNode,errorHead* typeCheckingErrorsHead);