#include "symbolTableDef.h"

SymbolTable* createSymbolTable(ASTNode* asTree,errorHead* symTableErrorListHead);

symbolTableEntry* createSymbolTableEntry(Token* token,int isID,int width,SYMBOL_NAME type);
symbolTableEntry* insertIDorFunID(SymbolTable* symTable, Token* token,SYMBOL_NAME type);
SymbolTable* createNewSymbolTable(int size);
symbolTableEntry* findIdorFuncName(char* findId,SymbolTable* symTable);
void populateSymbolTable(ASTNode* asTree,SymbolTable* symTable,errorHead* symTableErrorListHead);
ASTNode* getListOfStmts(ASTNode* asTree);

void processDeclarationStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processAssignmentStmtSingleVar(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processAssignmentStmtListVar(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processIfStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processIOStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processFunCallStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead);
void processFunctionDef(ASTNode* stmt, SymbolTable* symTable,errorHead* symTableErrorListHead);
void processBooleanExpr(ASTNode* expr, SymbolTable* symTable,errorHead* symTableErrorListHead);
void processArithmeticExprST(ASTNode* expr, SymbolTable* symTable,errorHead* symTableErrorListHead);

symbolTableEntry* checkIdExistence(char* tokenLexeme, SymbolTable* symTable);