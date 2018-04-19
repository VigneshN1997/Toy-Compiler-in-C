// ID: 2015A7PS0355P
// Name: Vignesh N
#include "typeChecking.c"

SymbolTable* createSymbolTable(ASTNode* asTree,errorHead* symTableErrorListHead,errorHead* typeCheckingErrorsHead);

symbolTableEntry* createSymbolTableEntry(Token* token,int isID,int width,SYMBOL_NAME type);
symbolTableEntry* insertIDorFunID(SymbolTable* symTable, Token* token,SYMBOL_NAME type);
SymbolTable* createNewSymbolTable(int size);
void populateSymbolTable(ASTNode* asTree,SymbolTable* symTable,errorHead* symTableErrorListHead, errorHead* typeCheckingErrorsHead);
ASTNode* getListOfStmts(ASTNode* asTree);

void printSymbolTable(SymbolTable* symTable);
void printSymbolTableRecursive(SymbolTable* symTable);

void processDeclarationStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processAssignmentStmtSingleVar(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processAssignmentStmtListVar(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processIfStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead,errorHead* typeCheckingErrorsHead);
void processIOStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processFunCallStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processFunctionDef(ASTNode* stmt, SymbolTable* symTable,errorHead* symTableErrorListHead,errorHead* typeCheckingErrorsHead);
void processBooleanExpr(ASTNode* expr, SymbolTable* symTable,errorHead* symTableErrorListHead);
void processArithmeticExprST(ASTNode* expr, SymbolTable* symTable,errorHead* symTableErrorListHead);

symbolTableEntry* checkIdExistence(char* tokenLexeme, SymbolTable* symTable);