// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "ASTDef.h"

ASTNode* createASTFromParseTree(ParseTree ptree);
void traverseParseTree(ParseTree ptree);
void processArithmeticExpr(treeNode* node);
void processNonTerminalRule(ParseTree ptree);
void setParentPointers(ASTNode* asTree);

void freeParseTree(ParseTree ptree);
void printAST(ASTNode* ast, int* numNodes);		// testing
void doInOrderTraversalAST(ASTNode* ast,FILE* fp,int* numNodes); // testing

ASTNode* createASTNode(SYMBOL_NAME op,Token* token,ASTNode* children);
ASTNode* concat(ASTNode* singleNode,ASTNode* restList);


void applyRuleFor_mainFunction(treeNode* tnode);
void applyRuleFor_stmtsAndFunctionDefs(treeNode* tnode);
void applyRuleFor_moreStmtsorFunctionDefs1(treeNode* tnode);
void applyRuleFor_moreStmtsorFunctionDefs2(treeNode* tnode);
void applyRuleFor_stmtOrFunctionDef1(treeNode* tnode);
void applyRuleFor_stmtOrFunctionDef2(treeNode* tnode);
void applyRuleFor_stmt(treeNode* tnode);
void applyRuleFor_functionDef(treeNode* tnode);
void applyRuleFor_parameter_list(treeNode* tnode);
void applyRuleFor_type(treeNode* tnode);
void applyRuleFor_remainingList1(treeNode* tnode);
void applyRuleFor_remainingList2(treeNode* tnode);
void applyRuleFor_declarationStmt(treeNode* tnode);
void applyRuleFor_var_list(treeNode* tnode);
void applyRuleFor_more_ids1(treeNode* tnode);
void applyRuleFor_more_ids2(treeNode* tnode);
void applyRuleFor_assignmentStmt_type1(treeNode* tnode);
void applyRuleFor_assignmentStmt_type2(treeNode* tnode);
void applyRuleFor_leftHandSide_singleVar(treeNode* tnode);
void applyRuleFor_leftHandSide_listVar(treeNode* tnode);
void applyRuleFor_rightHandSide_type(treeNode* tnode);
void applyRuleFor_sizeExpression(treeNode* tnode);
void applyRuleFor_ifStmt(treeNode* tnode);
void applyRuleFor_elseFactor1(treeNode* tnode);
void applyRuleFor_elseFactor2(treeNode* tnode);
void applyRuleFor_otherStmts1(treeNode* tnode);
void applyRuleFor_otherStmts2(treeNode* tnode);
void applyRuleFor_ioStmt(treeNode* tnode);
void applyRuleFor_funCallStmt(treeNode* tnode);
void applyRuleFor_inputParameterList1(treeNode* tnode);
void applyRuleFor_inputParameterList2(treeNode* tnode);
void applyRuleFor_listVar1(treeNode* tnode);
void applyRuleFor_listVar2(treeNode* tnode);
void applyRuleFor_arithmeticExpression(treeNode* tnode);
void applyRuleFor_arithmeticExpressionLF1(treeNode* tnode);
void applyRuleFor_arithmeticExpressionLF2(treeNode* tnode);
void applyRuleFor_arithmeticTerm(treeNode* tnode);
void applyRuleFor_arithmeticTermLF1(treeNode* tnode);
void applyRuleFor_arithmeticTermLF2(treeNode* tnode);
void applyRuleFor_factor1(treeNode* tnode);
void applyRuleFor_factor2(treeNode* tnode);
void applyRuleFor_operator(treeNode* tnode);
void applyRuleFor_booleanExpression1(treeNode* tnode);
void applyRuleFor_booleanExpression2(treeNode* tnode);
void applyRuleFor_booleanExpression3(treeNode* tnode);
void applyRuleFor_constrainedVars(treeNode* tnode);
void applyRuleFor_logicalOp(treeNode* tnode);
void applyRuleFor_relationalOp(treeNode* tnode);
void applyRuleFor_var1(treeNode* tnode);
void applyRuleFor_varMatrix(treeNode* tnode);
void applyRuleFor_varIsMatrixElement(treeNode* tnode);
void applyRuleFor_isMatrixElement1(treeNode* tnode);
void applyRuleFor_isMatrixElement2(treeNode* tnode);
void applyRuleFor_matrix(treeNode* tnode);
void applyRuleFor_rows(treeNode* tnode);
void applyRuleFor_rowsLF1(treeNode* tnode);
void applyRuleFor_rowsLF2(treeNode* tnode);
void applyRuleFor_row(treeNode* tnode);
void applyRuleFor_rowLF1(treeNode* tnode);
void applyRuleFor_rowLF2(treeNode* tnode);




// work left
// - parent pointer in AST