#include "ASTCreation.h"

ASTNode* createASTNode(SYMBOL_NAME op,Token* token,ASTNode* children)
{
	ASTNode* tnode = (ASTNode*)malloc(sizeof(ASTNode));
	tnode->op = op;
	tnode->parent = NULL;
	tnode->nextSibling = NULL;
	tnode->children = children;
	tnode->ptrToSymTableEntry = NULL;
	tnode->token = token;
	tnode->widthInfo = NULL;
	return tnode;
}

ASTNode* concat(ASTNode* singleNode,ASTNode* restList)
{
	singleNode->nextSibling = restList;
	return singleNode;
}

void applyRuleFor_mainFunction(treeNode* tnode)
{
	treeNode* mainTokenNode = tnode->children;
	treeNode* stmtsNode = mainTokenNode->nextSibling->nextSibling->nextSibling;
	tnode->ptrToASTNode = createASTNode(MAIN,mainTokenNode->nodeVal->token,(ASTNode*)stmtsNode->ptrToASTNode);
}

void applyRuleFor_stmtsAndFunctionDefs(treeNode* tnode)
{
	treeNode* stmtOrFunctionDef = tnode->children;
	treeNode* moreStmtsOrFunctionDefs = stmtOrFunctionDef->nextSibling;
	// printf("stmtsAndFunctionDefs\n");
	tnode->ptrToASTNode = concat((ASTNode*)stmtOrFunctionDef->ptrToASTNode,(ASTNode*)moreStmtsOrFunctionDefs->ptrToASTNode);
}

void applyRuleFor_moreStmtsorFunctionDefs1(treeNode* tnode)
{
	treeNode* stmtsAndFunctionDefs = tnode->children;
	tnode->ptrToASTNode = stmtsAndFunctionDefs->ptrToASTNode;
}

void applyRuleFor_moreStmtsorFunctionDefs2(treeNode* tnode)
{
	tnode->ptrToASTNode = NULL;
}

void applyRuleFor_stmtOrFunctionDef1(treeNode* tnode)
{
	treeNode* stmt = tnode->children;
	tnode->ptrToASTNode = stmt->ptrToASTNode;
}

void applyRuleFor_stmtOrFunctionDef2(treeNode* tnode)
{
	treeNode* functionDef = tnode->children;
	tnode->ptrToASTNode = functionDef->ptrToASTNode;
}

void applyRuleFor_stmt(treeNode* tnode)
{
	treeNode* child = tnode->children;
	tnode->ptrToASTNode = child->ptrToASTNode;	
}

void applyRuleFor_functionDef(treeNode* tnode) // updated
{
	treeNode* parameterListOut = tnode->children->nextSibling->nextSibling;
	treeNode* funIdNode = parameterListOut->nextSibling->nextSibling->nextSibling;
	treeNode* parameterListIn = funIdNode->nextSibling->nextSibling;
	treeNode* stmtsAndFunctionDefs = parameterListIn->nextSibling->nextSibling;

	ASTNode* parameterListOutAST = createASTNode(PARAMETER_LIST,NULL,(ASTNode*)parameterListOut->ptrToASTNode);
	ASTNode* parameterListInAST = createASTNode(PARAMETER_LIST,NULL,(ASTNode*)parameterListIn->ptrToASTNode);
	ASTNode* stmtsAndFunctionDefsAST = createASTNode(STMTS_AND_FUNCTION_DEFS,NULL,stmtsAndFunctionDefs->ptrToASTNode); // check if this tnode creation is needed?
	// printf("functionDef*1\n");
	ASTNode* children = concat(parameterListOutAST,stmtsAndFunctionDefsAST);
	// printf("functionDef*2\n");
	children = concat(parameterListInAST,children);

	tnode->ptrToASTNode = createASTNode(FUNCTION,funIdNode->nodeVal->token,children);
}

void applyRuleFor_parameter_list(treeNode* tnode)
{
	treeNode* type = tnode->children;
	treeNode* idNode = type->nextSibling;
	treeNode* remainingList = idNode->nextSibling;
	
	ASTNode* typeNodeAST = type->ptrToASTNode;
	ASTNode* idNodeAST = createASTNode(ID,idNode->nodeVal->token,NULL);
	// printf("parameterList*1\n");
	// ASTNode* children = concat(typeNodeAST,idNodeAST);
	typeNodeAST->children = idNodeAST;
	// ASTNode* parNodeAST = createASTNode(type->nodeVal->variable->sym_name,NULL,children);
	// ASTNode* typeNode = createASTNode(typeNode->nodeVal->token->t_name,NULL);
	// printf("parameterList*2\n");
	tnode->ptrToASTNode = concat(typeNodeAST,(ASTNode*)remainingList->ptrToASTNode);
}

void applyRuleFor_type(treeNode* tnode)
{
	tnode->ptrToASTNode = createASTNode(tnode->children->nodeVal->token->t_name,tnode->children->nodeVal->token,NULL); // check NULL
}

void applyRuleFor_remainingList1(treeNode* tnode)
{
	treeNode* parameterList = tnode->children->nextSibling;
	tnode->ptrToASTNode = parameterList->ptrToASTNode;
}

void applyRuleFor_remainingList2(treeNode* tnode)
{
	tnode->ptrToASTNode = NULL;
}

void applyRuleFor_declarationStmt(treeNode* tnode)
{
	treeNode* type = tnode->children;
	treeNode* var_list = type->nextSibling;
	// printf("DECLARATION_STMT\n");
	ASTNode* children = concat((ASTNode*)type->ptrToASTNode,(ASTNode*)var_list->ptrToASTNode);
	tnode->ptrToASTNode = createASTNode(DECLARATION_STMT,NULL,children);
}

void applyRuleFor_var_list(treeNode* tnode)
{
	treeNode* idNode = tnode->children;
	treeNode* more_ids = idNode->nextSibling;
	ASTNode* idNodeAST = createASTNode(ID,idNode->nodeVal->token,NULL);
	// printf("var_list\n");
	tnode->ptrToASTNode = concat((ASTNode*)idNodeAST,(ASTNode*)more_ids->ptrToASTNode);
}

void applyRuleFor_more_ids1(treeNode* tnode)
{
	treeNode* var_list = tnode->children->nextSibling;
	tnode->ptrToASTNode = var_list->ptrToASTNode;
}

void applyRuleFor_more_ids2(treeNode* tnode)
{
	tnode->ptrToASTNode = NULL;
}

void applyRuleFor_assignmentStmt_type1(treeNode* tnode)
{
	treeNode* leftHandSide_singleVar = tnode->children;
	treeNode* assignOp = leftHandSide_singleVar->nextSibling; // for line number
	treeNode* rightHandSide_type1 = assignOp->nextSibling;

	ASTNode* varsAST = createASTNode(LHS_SINGLE_VAR,NULL,(ASTNode*)leftHandSide_singleVar->ptrToASTNode);
	// printf("assignmentStmt_type1\n");
	ASTNode* children = concat(varsAST,(ASTNode*)rightHandSide_type1->ptrToASTNode);

	tnode->ptrToASTNode = createASTNode(ASSIGNOP,assignOp->nodeVal->token,children);
}

void applyRuleFor_assignmentStmt_type2(treeNode* tnode)
{
	treeNode* leftHandSide_listVar = tnode->children;
	treeNode* assignOp = leftHandSide_listVar->nextSibling; // for line number
	treeNode* rightHandSide_type2 = assignOp->nextSibling;

	ASTNode* varsAST = createASTNode(LHS_LIST_VAR,NULL,(ASTNode*)leftHandSide_listVar->ptrToASTNode);
	// printf("assignmentStmt_type1\n");
	ASTNode* children = concat(varsAST,(ASTNode*)rightHandSide_type2->ptrToASTNode);

	tnode->ptrToASTNode = createASTNode(ASSIGNOP,assignOp->nodeVal->token,children);
}

void applyRuleFor_leftHandSide_singleVar(treeNode* tnode)
{
	treeNode* idNode = tnode->children;
	tnode->ptrToASTNode = createASTNode(ID,idNode->nodeVal->token,NULL);
}

void applyRuleFor_leftHandSide_listVar(treeNode* tnode)
{
	treeNode* var_list = tnode->children->nextSibling;
	tnode->ptrToASTNode = var_list->ptrToASTNode;
}

void applyRuleFor_rightHandSide_type(treeNode* tnode)
{
	treeNode* child = tnode->children;
	tnode->ptrToASTNode = child->ptrToASTNode;
}

void applyRuleFor_sizeExpression(treeNode* tnode)
{
	treeNode* sizeNode = tnode->children;
	treeNode* idNode = sizeNode->nextSibling;
	ASTNode* child = createASTNode(ID,idNode->nodeVal->token,NULL);
	tnode->ptrToASTNode = createASTNode(SIZE,sizeNode->nodeVal->token,child);
}

void applyRuleFor_ifStmt(treeNode* tnode)
{
	treeNode* ifNode = tnode->children;
	treeNode* booleanExpression = ifNode->nextSibling->nextSibling;
	treeNode* stmt = booleanExpression->nextSibling->nextSibling;
	treeNode* otherStmts = stmt->nextSibling;
	treeNode* elseFactor = otherStmts->nextSibling;
	// printf("ifStmt*1\n");
	ASTNode* allstmts = concat((ASTNode*)stmt->ptrToASTNode,(ASTNode*)otherStmts->ptrToASTNode);
	ASTNode* stmtsAST = createASTNode(STMTS_AND_FUNCTION_DEFS,NULL,allstmts);
	ASTNode* elseFactorAST = createASTNode(ELSE,NULL,(ASTNode*)elseFactor->ptrToASTNode);
	// printf("ifStmt*2\n");
	ASTNode* children = concat(stmtsAST,elseFactorAST);
	// printf("ifStmt*3\n");
	children = concat((ASTNode*)booleanExpression->ptrToASTNode,children);

	tnode->ptrToASTNode = createASTNode(IF,ifNode->nodeVal->token,children);
}

void applyRuleFor_elseFactor1(treeNode* tnode)
{
	treeNode* stmt = tnode->children->nextSibling;
	treeNode* otherStmts = stmt->nextSibling;
	// printf("elseFactor\n");
	tnode->ptrToASTNode = concat((ASTNode*)stmt->ptrToASTNode,(ASTNode*)otherStmts->ptrToASTNode);
}

void applyRuleFor_elseFactor2(treeNode* tnode)
{
	tnode->ptrToASTNode = NULL;
}

void applyRuleFor_otherStmts1(treeNode* tnode)
{
	treeNode* stmt = tnode->children;
	treeNode* otherStmts = stmt->nextSibling;
	// printf("otherStmts\n");
	tnode->ptrToASTNode = concat((ASTNode*)stmt->ptrToASTNode,(ASTNode*)otherStmts->ptrToASTNode);
}

void applyRuleFor_otherStmts2(treeNode* tnode)
{
	tnode->ptrToASTNode = NULL;
}

void applyRuleFor_ioStmt(treeNode* tnode)
{
	treeNode* opNode = tnode->children;
	treeNode* idNode = opNode->nextSibling->nextSibling;
	ASTNode* child = createASTNode(ID,idNode->nodeVal->token,NULL);
	tnode->ptrToASTNode = createASTNode(opNode->nodeVal->token->t_name,opNode->nodeVal->token,child);
}

void applyRuleFor_funCallStmt(treeNode* tnode)
{
	treeNode* funCallNode = tnode->children;
	treeNode* inputParameterList = funCallNode->nextSibling->nextSibling;
	tnode->ptrToASTNode = createASTNode(FUNID,funCallNode->nodeVal->token,(ASTNode*)inputParameterList->ptrToASTNode);
}

void applyRuleFor_inputParameterList1(treeNode* tnode)
{
	treeNode* var = tnode->children;
	treeNode* listVar = var->nextSibling;
	// printf("inputParameterList\n");
	tnode->ptrToASTNode = concat((ASTNode*)var->ptrToASTNode,(ASTNode*)listVar->ptrToASTNode);
}

void applyRuleFor_inputParameterList2(treeNode* tnode)
{
	tnode->ptrToASTNode = NULL;
}

void applyRuleFor_listVar1(treeNode* tnode)
{
	treeNode* inputParameterList = tnode->children->nextSibling;
	tnode->ptrToASTNode = inputParameterList->ptrToASTNode;
}

void applyRuleFor_listVar2(treeNode* tnode)
{
	tnode->ptrToASTNode = NULL;
}

// rules for arithmetic expression(47-52)
void applyRuleFor_arithmeticExpression(treeNode* tnode)
{
	treeNode* arithmeticExpressionLF = tnode->children->nextSibling;
	tnode->ptrToASTNode = arithmeticExpressionLF->ptrToASTNode;
}

void applyRuleFor_arithmeticExpressionLF1(treeNode* tnode)
{
	treeNode* operator_lowPrecedence = tnode->children;
	treeNode* arithmeticExpression = operator_lowPrecedence->nextSibling;
	// printf("arithmeticExpressionLF\n");
	ASTNode* children = concat((ASTNode*)tnode->inh_addr,(ASTNode*)arithmeticExpression->ptrToASTNode);
	((ASTNode*)operator_lowPrecedence->ptrToASTNode)->children = children;
	tnode->ptrToASTNode = operator_lowPrecedence->ptrToASTNode;
}

void applyRuleFor_arithmeticExpressionLF2(treeNode* tnode)
{
	tnode->ptrToASTNode = tnode->inh_addr;
}

void applyRuleFor_arithmeticTerm(treeNode* tnode)
{
	treeNode* arithmeticTermLF = tnode->children->nextSibling;
	tnode->ptrToASTNode = arithmeticTermLF->ptrToASTNode;
}

void applyRuleFor_arithmeticTermLF1(treeNode* tnode)
{
	treeNode* operator_highPrecedence = tnode->children;
	treeNode* arithmeticTerm = operator_highPrecedence->nextSibling;
	// printf("arithmeticTermLF\n");
	ASTNode* children = concat((ASTNode*)tnode->inh_addr,(ASTNode*)arithmeticTerm->ptrToASTNode);
	((ASTNode*)operator_highPrecedence->ptrToASTNode)->children = children;
	tnode->ptrToASTNode = operator_highPrecedence->ptrToASTNode;
}

void applyRuleFor_arithmeticTermLF2(treeNode* tnode)
{
	tnode->ptrToASTNode = tnode->inh_addr;
}


void applyRuleFor_factor1(treeNode* tnode)
{
	treeNode* arithmeticExpression = tnode->children->nextSibling;
	tnode->ptrToASTNode = arithmeticExpression->ptrToASTNode;
}

void applyRuleFor_factor2(treeNode* tnode)
{
	treeNode* var = tnode->children;
	tnode->ptrToASTNode = var->ptrToASTNode;
}

void applyRuleFor_operator(treeNode* tnode)
{
	treeNode* operator = tnode->children;
	ASTNode* opNodeAST = createASTNode(operator->nodeVal->token->t_name,operator->nodeVal->token,NULL);
	tnode->ptrToASTNode = opNodeAST;
}

void applyRuleFor_booleanExpression1(treeNode* tnode)
{
	treeNode* booleanExpression1 = tnode->children->nextSibling;
	treeNode* logicalOp = booleanExpression1->nextSibling->nextSibling;
	treeNode* booleanExpression2 = logicalOp->nextSibling->nextSibling;
	// printf("booleanExpression1\n");
	ASTNode* children = concat((ASTNode*)booleanExpression1->ptrToASTNode,(ASTNode*)booleanExpression2->ptrToASTNode);
	((ASTNode*)logicalOp->ptrToASTNode)->children = children;
	tnode->ptrToASTNode = logicalOp->ptrToASTNode;
}

void applyRuleFor_booleanExpression2(treeNode* tnode)
{
	treeNode* notOp = tnode->children;
	notOp->ptrToASTNode = createASTNode(NOT,notOp->nodeVal->token,NULL);
	treeNode* booleanExpression1 = notOp->nextSibling->nextSibling;
	((ASTNode*)notOp->ptrToASTNode)->children = booleanExpression1->ptrToASTNode;
	tnode->ptrToASTNode = notOp->ptrToASTNode;
}

void applyRuleFor_booleanExpression3(treeNode* tnode)
{
	treeNode* constrainedVars1 = tnode->children;
	treeNode* relationalOp = constrainedVars1->nextSibling;
	treeNode* constrainedVars2 = relationalOp->nextSibling;
	// printf("booleanExpression3\n");
	ASTNode* children = concat((ASTNode*)constrainedVars1->ptrToASTNode,(ASTNode*)constrainedVars2->ptrToASTNode);
	((ASTNode*)relationalOp->ptrToASTNode)->children = children;
	tnode->ptrToASTNode = relationalOp->ptrToASTNode;
}

void applyRuleFor_constrainedVars(treeNode* tnode)
{
	treeNode* child = tnode->children;
	tnode->ptrToASTNode = createASTNode(child->nodeVal->token->t_name,child->nodeVal->token,NULL);
}

void applyRuleFor_logicalOp(treeNode* tnode)
{
	treeNode* child = tnode->children;
	tnode->ptrToASTNode = createASTNode(child->nodeVal->token->t_name,child->nodeVal->token,NULL);
}

void applyRuleFor_relationalOp(treeNode* tnode)
{
	treeNode* child = tnode->children;
	tnode->ptrToASTNode = createASTNode(child->nodeVal->token->t_name,child->nodeVal->token,NULL);
}

void applyRuleFor_var1(treeNode* tnode)
{
	treeNode* child = tnode->children;
	tnode->ptrToASTNode = createASTNode(child->nodeVal->token->t_name,child->nodeVal->token,NULL);	
}

void applyRuleFor_varMatrix(treeNode* tnode)
{
	treeNode* matrix = tnode->children;
	tnode->ptrToASTNode = matrix->ptrToASTNode;
}

void applyRuleFor_varIsMatrixElement(treeNode* tnode)
{
	treeNode* idNode = tnode->children;
	treeNode* isMatrixElement = idNode->nextSibling;
	tnode->ptrToASTNode = createASTNode(ID,idNode->nodeVal->token,(ASTNode*)isMatrixElement->ptrToASTNode);
}

void applyRuleFor_isMatrixElement1(treeNode* tnode)
{
	treeNode* num1 = tnode->children->nextSibling;
	treeNode* num2 = num1->nextSibling->nextSibling;
	ASTNode* num1AST = createASTNode(NUM,num1->nodeVal->token,NULL);
	ASTNode* num2AST = createASTNode(NUM,num2->nodeVal->token,NULL);
	// printf("isMatrixElement\n");
	tnode->ptrToASTNode = concat(num1AST,num2AST);
}

void applyRuleFor_isMatrixElement2(treeNode* tnode)
{
	tnode->ptrToASTNode = NULL;
}

void applyRuleFor_matrix(treeNode* tnode)
{
	treeNode* rows = tnode->children->nextSibling;
	tnode->ptrToASTNode = createASTNode(MATRIX,NULL,(ASTNode*)rows->ptrToASTNode);
}

void applyRuleFor_rows(treeNode* tnode)
{
	treeNode* row = tnode->children;
	treeNode* rowsLF = row->nextSibling;
	
	ASTNode* rowAST = createASTNode(ROW,NULL,(ASTNode*)row->ptrToASTNode);
	// printf("rows\n");
	tnode->ptrToASTNode = concat(rowAST,(ASTNode*)rowsLF->ptrToASTNode);
}

void applyRuleFor_rowsLF1(treeNode* tnode)
{
	treeNode* rows = tnode->children->nextSibling;
	tnode->ptrToASTNode = rows->ptrToASTNode;
}

void applyRuleFor_rowsLF2(treeNode* tnode)
{
	tnode->ptrToASTNode = NULL;
}

void applyRuleFor_row(treeNode* tnode)
{
	treeNode* numNode = tnode->children;
	treeNode* rowLF = numNode->nextSibling;
	ASTNode* numNodeAST = createASTNode(NUM,numNode->nodeVal->token,NULL);
	// printf("row\n");
	tnode->ptrToASTNode = concat(numNodeAST,(ASTNode*)rowLF->ptrToASTNode);		
}

void applyRuleFor_rowLF1(treeNode* tnode)
{
	treeNode* row = tnode->children->nextSibling;
	tnode->ptrToASTNode = row->ptrToASTNode;
}

void applyRuleFor_rowLF2(treeNode* tnode)
{
	tnode->ptrToASTNode = NULL;
}