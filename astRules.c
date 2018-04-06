// struct ASTNode
// {
// 	SYMBOL_NAME op;
// 	struct ASTNode* parent;
// 	struct ASTNode* nextSibling;
// 	struct ASTNode* children;
// 	void* ptrToSymTableEntry;
// 	Token* token;
// };


ASTNode* createASTNode(SYMBOL_NAME op,Token* token,ASTNode* children)
{
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->op = op;
	node->parent = NULL;
	node->nextSibling = NULL;
	node->children = children;
	node->ptrToSymTableEntry = NULL;
	node->token = token;
	return node;
}

ASTNode* concat(ASTNode* singleNode,ASTNode* restList)
{
	singleNode->nextSibling = restList;
	return singleNode;
}

treeNode* getIthChildOfParseTreeNode(treeNode* pTreeNode, int i)
{
	treeNode* temp = pTreeNode->children;
	int childNum = 0;
	while(childNum != i)
	{
		temp = temp->nextSibling;
		childNum++;
	}
	return temp;
}

void applyRuleFor_mainFunction(treeNode* node)
{
	treeNode* mainTokenNode = node->children;
	treeNode* stmtsNode = getIthChildOfParseTreeNode(node,3);
	node->ptrToASTNode = createNewASTNode(MAIN,mainTokenNode->nodeVal->token,stmtsNode->ptrToASTNode);
}


void applyRuleFor_stmtsAndFunctionDefs(treeNode* node)
{
	treeNode* stmtOrFunctionDef = node->children;
	treeNode* moreStmtsOrFunctionDefs = stmtOrFunctionDef->nextSibling;
	node->ptrToASTNode = concat(stmtOrFunctionDef->ptrToASTNode,moreStmtsOrFunctionDefs->ptrToASTNode);
}

void applyRuleFor_moreStmtsorFunctionDefs1(treeNode* node)
{
	treeNode* stmtsAndFunctionDefs = node->children;
	node->ptrToASTNode = stmtsAndFunctionDefs->ptrToASTNode;
}

void applyRuleFor_moreStmtsorFunctionDefs2(treeNode* node)
{
	node->ptrToASTNode = NULL;
}

void applyRuleFor_stmtOrFunctionDef1(treeNode* node)
{
	treeNode* stmt = node->children;
	node->ptrToASTNode = stmt->ptrToASTNode;
}

void applyRuleFor_stmtOrFunctionDef2(treeNode* node)
{
	treeNode* functionDef = node->children;
	node->ptrToASTNode = functionDef->ptrToASTNode;
}

void applyRuleFor_stmt(treeNode* node)
{
	treeNode* child = node->children;
	node->ptrToASTNode = child->ptrToASTNode;	
}

void applyRuleFor_functionDef(treeNode* node)
{
	treeNode* parameterListOut = node->children->nextSibling->nextSibling;
	treeNode* funIdNode = parameterListOut->nextSibling->nextSibling->nextSibling;
	treeNode* parameterListIn = funIdNode->nextSibling->nextSibling;
	treeNode* stmtsAndFunctionDefs = parameterListIn->nextSibling->nextSibling;

	ASTNode* parameterListOutAST = createASTNode(PARAMETER_LIST,NULL,parameterListOut->ptrToASTNode);
	ASTNode* parameterListInAST = createASTNode(PARAMETER_LIST,NULL,parameterListIn->ptrToASTNode);
	ASTNode* stmtsAndFunctionDefsAST = createASTNode(STMTS_AND_FUNCTION_DEFS,NULL,stmtsAndFunctionDefs->ptrToASTNode);
	
	ASTNode* children = concat(parameterListOutAST,stmtsAndFunctionDefsAST);
	children = concat(parameterListIn,children);

	node->ptrToASTNode = createASTNode(FUNCTION,funIdNode->nodeVal->token,children);
}

void applyRuleFor_parameter_list(treeNode node)
{
	treeNode* type = node->children;
	treeNode* idNode = type->nextSibling;
	treeNode* remainingList = idNode->nextSibling;
	
	ASTNode* typeNodeAST = type->ptrToASTNode;
	ASTNode* idNodeAST = createASTNode(ID,token,NULL);
	ASTNode* children = concat(typeNodeAST,idNodeAST);
	ASTNode* parNodeAST = createASTNode(idNode->nodeVal->token->t_name,NULL,children);
	// ASTNode* typeNode = createASTNode(typeNode->nodeVal->token->t_name,NULL);
	node->ptrToASTNode = concat(parNodeAST,remainingList->ptrToASTNode);
}

void applyRuleFor_type(treeNode* node)
{
	node->ptrToASTNode = createASTNode(node->children->nodeVal->token->t_name,NULL,NULL); // check NULL
}

void applyRuleFor_remainingList1(treeNode* node)
{
	treeNode* parameterList = node->children->nextSibling;
	node->ptrToASTNode = parameterList->ptrToASTNode;
}

void applyRuleFor_remainingList2(treeNode* node)
{
	node->ptrToASTNode = NULL;
}

void applyRuleFor_declarationStmt(treeNode* node)
{
	treeNode* type = node->children;
	treeNode* var_list = type->nextSibling;
	ASTNode* children = concat(type->ptrToASTNode,var_list->ptrToASTNode);
	node->ptrToASTNode = createASTNode(DECLARATION_STMT,NULL,children);
}

void applyRuleFor_var_list(treeNode* node)
{
	treeNode* idNode = node->children;
	treeNode* more_ids = idNode->nextSibling;
	ASTNode* idNodeAST = createASTNode(ID,idNode->nodeVal->token,NULL);
	node->ptrToASTNode = concat(idNodeAST,more_ids->ptrToASTNode);
}

void applyRuleFor_more_ids1(treeNode* node)
{
	treeNode* var_list = node->children->nextSibling;
	node->ptrToASTNode = var_list->ptrToASTNode;
}

void applyRuleFor_more_ids2(treeNode* node)
{
	node->ptrToASTNode = NULL;
}

void applyRuleFor_assignmentStmt_type1(treeNode* node)
{
	treeNode* leftHandSide_singleVar = node->children;
	treeNode* assignOp = leftHandSide_singleVar->nextSibling; // for line number
	treeNode* rightHandSide_type1 = assignOp->nextSibling;

	ASTNode* varsAST = createASTNode(LHS_SINGLE_VAR,NULL,leftHandSide_singleVar->ptrToASTNode);
	ASTNode* children = concat(varsAST,rightHandSide_type1);

	node->ptrToASTNode = createASTNode(ASSIGN_OP,assignOp->nodeVal->token,children);
}

void applyRuleFor_assignmentStmt_type2(treeNode* node)
{
	treeNode* leftHandSide_listVar = node->children;
	treeNode* assignOp = leftHandSide_listVar->nextSibling; // for line number
	treeNode* rightHandSide_type2 = assignOp->nextSibling;

	ASTNode* varsAST = createASTNode(LHS_LIST_VAR,NULL,leftHandSide_listVar->ptrToASTNode);
	ASTNode* children = concat(varsAST,rightHandSide_type2);

	node->ptrToASTNode = createASTNode(ASSIGN_OP,assignOp->nodeVal->token,children);
}

void applyRuleFor_leftHandSide_singleVar(treeNode* node)
{
	treeNode* idNode = node->children;
	node->ptrToASTNode = createASTNode(ID,idNode->nodeVal->token,NULL);
}

void applyRuleFor_leftHandSide_listVar(treeNode* node)
{
	treeNode* var_list = node->children->nextSibling;
	node->ptrToASTNode = var_list->ptrToASTNode;
}

void applyRuleFor_rightHandSide_type(treeNode* node)
{
	treeNode* child = node->children;
	node->ptrToASTNode = child->ptrToASTNode;
}

void applyRuleFor_sizeExpression(treeNode* node)
{
	treeNode* sizeNode = node->children;
	treeNode* idNode = sizeNode->nextSibling;
	ASTNode* child = createASTNode(ID,idNode->nodeVal->token,NULL);
	node->ptrToASTNode = createASTNode(SIZE,sizeNode->nodeVal->token,child);
}

void applyRuleFor_ifStmt(treeNode* node)
{
	treeNode* ifNode = node->children;
	treeNode* booleanExpression = ifNode->nextSibling->nextSibling;
	treeNode* stmt = booleanExpression->nextSibling->nextSibling;
	treeNode* otherStmts = stmt->nextSibling;
	treeNode* elseFactor = otherStmts->nextSibling;

	ASTNode* allstmts = concat(stmt->ptrToASTNode,otherStmts->ptrToASTNode);
	ASTNode* stmtsAST = createASTNode(STMTS_AND_FUNCTION_DEFS,NULL,allstmts);
	ASTNode* elseFactorAST = createASTNode(ELSE,NULL,elseFactor->ptrToASTNode);

	ASTNode* children = concat(stmtsAST,elseFactorAST);
	children = concat(booleanExpression->ptrToASTNode,children);

	node->ptrToASTNode = createASTNode(IF,ifNode->nodeVal->token,children);
}

void applyRuleFor_elseFactor1(treeNode* node)
{
	treeNode* stmt = node->children->nextSibling;
	treeNode* otherStmts = stmt->nextSibling;

	node->ptrToASTNode = concat(stmt->ptrToASTNode,otherStmts->ptrToASTNode);
}

void applyRuleFor_elseFactor2(treeNode* node)
{
	node->ptrToASTNode = NULL;
}

void applyRuleFor_otherStmts1(treeNode* node)
{
	treeNode* stmt = node->children;
	treeNode* otherStmts = stmt->nextSibling;
	node->ptrToASTNode = concat(stmt->ptrToASTNode,otherStmts->ptrToASTNode);
}

void applyRuleFor_otherStmts2(treeNode* node)
{
	node->ptrToASTNode = NULL;
}

void applyRuleFor_ioStmt(treeNode* node)
{
	treeNode* opNode = node->children;
	treeNode* idNode = opNode->nextSibling->nextSibling;
	ASTNode* child = createASTNode(ID,idNode->nodeVal->token,NULL);
	node->ptrToASTNode = createASTNode(opNode->nodeVal->token->t_name,opNode->nodeVal->token,child);
}

void applyRuleFor_funCallStmt(treeNode* node)
{
	treeNode* funCallNode = node->children;
	treeNode* inputParameterList = funCallNode->nextSibling->nextSibling;
	node->ptrToASTNode = createASTNode(FUNID,funCallNode->nodeVal>token,inputParameterList->ptrToASTNode);
}

void applyRuleFor_inputParameterList1(treeNode* node)
{
	treeNode* var = node->children;
	treeNode* listVar = var->nextSibling;
	node->ptrToASTNode = concat(var->ptrToASTNode,listVar->ptrToASTNode);
}

void applyRuleFor_inputParameterList2(treeNode* node)
{
	node->ptrToASTNode = NULL;
}

void applyRuleFor_listVar1(treeNode* node)
{
	treeNode* inputParameterList = node->children->nextSibling;
	node->ptrToASTNode = inputParameterList->ptrToASTNode;
}

void applyRuleFor_listVar2(treeNode* node)
{
	node->ptrToASTNode = NULL;
}

// rules for arithmetic expression(47-52)


void applyRuleFor_factor1(treeNode* node)
{
	treeNode* var = node->children;
	node->ptrToASTNode = var->ptrToASTNode;
}

void applyRuleFor_factor2(treeNode* node)
{
	treeNode* arithmeticExpression = node->children->nextSibling;
	node->ptrToASTNode = arithmeticExpression->ptrToASTNode;
}

void applyRuleFor_operator(treeNode* node)
{
	treeNode* operator = node->children;
	ASTNode* opNodeAST = createASTNode(operator->nodeVal->token->t_name,operator->nodeVal->token,NULL);
	node->ptrToASTNode = opNodeAST;
}

void applyRuleFor_booleanExpression1(treeNode* node)
{
	treeNode* booleanExpression1 = node->children->nextSibling;
	treeNode* logicalOp = lhs->nextSibling->nextSibling;
	treeNode* booleanExpression2 = logicalOp->nextSibling->nextSibling;
	
}



