#include "astRules.c"

ASTNode* createASTFromParseTree(ParseTree ptree)
{
	traverseParseTree(ptree);
	ASTNode* mainNodeAST = ptree->ptrToASTNode;
	freeParseTree(ptree);
	setParentPointers(mainNodeAST);
	return mainNodeAST;
}

void traverseParseTree(ParseTree ptree)
{
	if(ptree->children == NULL)
	{
		return;
	}

	// this node has both inherited and synthesised attributes
	if(ptree->nodeVal->token == NULL && ptree->nodeVal->variable->sym_name == ARITHMETIC_EXPR)
	{
		processArithmeticExpr(ptree);
		return;
	}
	else
	{
		treeNode* child = ptree->children;
		while(child != NULL)
		{
			if(child->nodeVal->token == NULL && child->nodeVal->variable->sym_name != EPSILON)
			{
				traverseParseTree(child);
				// processNonTerminalRule(child);
			}
			child = child->nextSibling;
		}
		processNonTerminalRule(ptree);
	}
}

void processArithmeticExpr(treeNode* node)
{
	if(node->children == NULL)
	{
		return;
	}

	treeNode* child = node->children;
	while(child != NULL)
	{
		if(child->nodeVal->token == NULL && child->nodeVal->variable->sym_name == ARITHMETIC_EXPR_LF)
		{
			child->inh_addr = node->children->ptrToASTNode;
		}
		else if(child->nodeVal->token == NULL && child->nodeVal->variable->sym_name == ARITHMETIC_TERM_LF)
		{	
			child->inh_addr = node->children->ptrToASTNode;
		}
		if(child->nodeVal->token == NULL && child->nodeVal->variable->sym_name != EPSILON)
		{
			processArithmeticExpr(child);
		}
		child = child->nextSibling;
	}
	processNonTerminalRule(node);
}

void processNonTerminalRule(ParseTree ptree)
{
	int ruleNumber = ptree->ptrToRule->ruleNumber;
	switch(ruleNumber)
	{
		case 0:
			applyRuleFor_mainFunction(ptree);
			break;
		case 1:
			applyRuleFor_stmtsAndFunctionDefs(ptree);
			break;
		case 2:
			applyRuleFor_moreStmtsorFunctionDefs1(ptree);
			break;
		case 3:
			applyRuleFor_moreStmtsorFunctionDefs2(ptree);
			break;
		case 4:
			applyRuleFor_stmtOrFunctionDef1(ptree);
			break;
		case 5:
			applyRuleFor_stmtOrFunctionDef2(ptree);
			break;
		case 6:
			applyRuleFor_stmt(ptree);
			break;
		case 7:
			applyRuleFor_stmt(ptree);
			break;
		case 8:
			applyRuleFor_stmt(ptree);
			break;
		case 9:
			applyRuleFor_stmt(ptree);
			break;
		case 10:
			applyRuleFor_stmt(ptree);
			break;
		case 11:
			applyRuleFor_stmt(ptree);
			break;
		case 12:
			applyRuleFor_functionDef(ptree);
			break;
		case 13:
			applyRuleFor_parameter_list(ptree);
			break;
		case 14:
			applyRuleFor_type(ptree);
			break;
		case 15:
			applyRuleFor_type(ptree);
			break;
		case 16:
			applyRuleFor_type(ptree);
			break;
		case 17:
			applyRuleFor_type(ptree);
			break;
		case 18:
			applyRuleFor_remainingList1(ptree);
			break;
		case 19:
			applyRuleFor_remainingList2(ptree);
			break;
		case 20:
			applyRuleFor_declarationStmt(ptree);
			break;
		case 21:
			applyRuleFor_var_list(ptree);
			break;
		case 22:
			applyRuleFor_more_ids1(ptree);
			break;
		case 23:
			applyRuleFor_more_ids2(ptree);
			break;
		case 24:
			applyRuleFor_assignmentStmt_type1(ptree);
			break;
		case 25:
			applyRuleFor_assignmentStmt_type2(ptree);
			break;
		case 26:
			applyRuleFor_leftHandSide_singleVar(ptree);
			break;
		case 27:
			applyRuleFor_leftHandSide_listVar(ptree);
			break;
		case 28:
			applyRuleFor_rightHandSide_type(ptree);
			break;
		case 29:
			applyRuleFor_rightHandSide_type(ptree);
			break;
		case 30:
			applyRuleFor_rightHandSide_type(ptree);
			break;
		case 31:
			applyRuleFor_rightHandSide_type(ptree);
			break;
		case 32:
			applyRuleFor_rightHandSide_type(ptree);
			break;
		case 33:
			applyRuleFor_sizeExpression(ptree);
			break;
		case 34:
			applyRuleFor_ifStmt(ptree);
			break;
		case 35:
			applyRuleFor_elseFactor1(ptree);
			break;
		case 36:
			applyRuleFor_elseFactor2(ptree);
			break;
		case 37:
			applyRuleFor_otherStmts1(ptree);
			break;
		case 38:
			applyRuleFor_otherStmts2(ptree);
			break;
		case 39:
			applyRuleFor_ioStmt(ptree);
			break;
		case 40:
			applyRuleFor_ioStmt(ptree);
			break;
		case 41:
			applyRuleFor_funCallStmt(ptree);
			break;
		case 42:
			applyRuleFor_inputParameterList1(ptree);
			break;
		case 43:
			applyRuleFor_inputParameterList2(ptree);
			break;
		case 44:
			applyRuleFor_listVar1(ptree);
			break;
		case 45:
			applyRuleFor_listVar2(ptree);
			break;
		case 46:
			applyRuleFor_arithmeticExpression(ptree);
			break;
		case 47:
			applyRuleFor_arithmeticExpressionLF1(ptree);
			break;
		case 48:
			applyRuleFor_arithmeticExpressionLF2(ptree);
			break;
		case 49:
			applyRuleFor_arithmeticTerm(ptree);
			break;
		case 50:
			applyRuleFor_arithmeticTermLF1(ptree);
			break;
		case 51:
			applyRuleFor_arithmeticTermLF2(ptree);
			break;
		case 52:
			applyRuleFor_factor1(ptree);
			break;
		case 53:
			applyRuleFor_factor2(ptree);
			break;
		case 54:
			applyRuleFor_operator(ptree);
			break;
		case 55:
			applyRuleFor_operator(ptree);
			break;
		case 56:
			applyRuleFor_operator(ptree);
			break;
		case 57:
			applyRuleFor_operator(ptree);
			break;
		case 58:
			applyRuleFor_booleanExpression1(ptree);
			break;
		case 59:
			applyRuleFor_booleanExpression2(ptree);
			break;
		case 60:
			applyRuleFor_booleanExpression3(ptree);
			break;
		case 61:
			applyRuleFor_constrainedVars(ptree);
			break;
		case 62:
			applyRuleFor_constrainedVars(ptree);
			break;
		case 63:
			applyRuleFor_constrainedVars(ptree);
			break;
		case 64:
			applyRuleFor_varIsMatrixElement(ptree);
			break;
		case 65:
			applyRuleFor_var1(ptree);
			break;
		case 66:
			applyRuleFor_var1(ptree);
			break;
		case 67:
			applyRuleFor_var1(ptree);
			break;
		case 68:
			applyRuleFor_varMatrix(ptree);
			break;
		case 69:
			applyRuleFor_matrix(ptree);
			break;
		case 70:
			applyRuleFor_rows(ptree);
			break;
		case 71:
			applyRuleFor_rowsLF1(ptree);
			break;
		case 72:
			applyRuleFor_rowsLF2(ptree);
			break;
		case 73:
			applyRuleFor_row(ptree);
			break;
		case 74:
			applyRuleFor_rowLF1(ptree);
			break;
		case 75:
			applyRuleFor_rowLF2(ptree);
			break;
		case 76:
			applyRuleFor_isMatrixElement1(ptree);
			break;
		case 77:
			applyRuleFor_isMatrixElement2(ptree);
			break;
		case 78:
			applyRuleFor_logicalOp(ptree);
			break;
		case 79:
			applyRuleFor_logicalOp(ptree);
			break;
		case 80:
			applyRuleFor_relationalOp(ptree);
			break;
		case 81:
			applyRuleFor_relationalOp(ptree);
			break;
		case 82:
			applyRuleFor_relationalOp(ptree);
			break;
		case 83:
			applyRuleFor_relationalOp(ptree);
			break;
		case 84:
			applyRuleFor_relationalOp(ptree);
			break;
		case 85:
			applyRuleFor_relationalOp(ptree);
			break;
	}
}

void freeParseTree(ParseTree ptree)
{
	if(ptree->children == NULL)
	{
		free(ptree);
	}
	treeNode* child = ptree->children;
	while(child != NULL)
	{
		traverseParseTree(child);
		child = child->nextSibling;
	}
	free(ptree);
}

void printAST(ASTNode* ast)
{
	FILE* fp;
	fp = fopen("astfile.txt","w");
	fprintf(fp, "==============================================\n");
	fprintf(fp, "Operator |Lexeme Current Node   |Value|Parent  \n");
	fprintf(fp, "==============================================\n");

	doInOrderTraversalAST(ast,fp);
	fclose(fp);
}

void doInOrderTraversalAST(ASTNode* ast,FILE* fp)
{
	if(ast->children == NULL)
	{
		Token* tok = ast->token;
		if(tok == NULL)
		{
			fprintf(fp, "%d - - %d\n",(int)ast->op,(int)ast->parent->op);
		}
		else if(tok->t_name == NUM)
		{
			fprintf(fp,"%d %s %d %d\n",(int)ast->op,tok->lexeme,(tok->value).int_value,(int)ast->parent->op);
		}
		else if(tok->t_name == RNUM)
		{
			fprintf(fp,"%d %s %lf %d\n",(int)ast->op,tok->lexeme,(tok->value).real_value,(int)ast->parent->op);
		}
		else
		{
			fprintf(fp,"%d %s - %d\n",(int)ast->op,tok->lexeme,(int)ast->parent->op);
		}
	}
	else
	{
		ASTNode* leftMostChild = ast->children;
		doInOrderTraversalAST(leftMostChild,fp);
		Token* tok = ast->token;
		if(ast->parent != NULL)
		{	
			if(tok == NULL)
			{
				fprintf(fp, "%d - - %d\n",(int)ast->op,(int)ast->parent->op);
			}
			else
			{
				fprintf(fp,"%d %s - %d\n",(int)ast->op,tok->lexeme,(int)ast->parent->op);
			}
		}
		else
		{
			fprintf(fp,"%d %s - -\n",(int)ast->op,tok->lexeme);
		}
		ASTNode* otherChildren = leftMostChild->nextSibling;
		while(otherChildren != NULL)
		{
			doInOrderTraversalAST(otherChildren,fp);
			otherChildren = otherChildren->nextSibling;
		}
	}
}

void setParentPointers(ASTNode* asTree)
{
	if(asTree == NULL)
	{
		return;
	}
	ASTNode* child = asTree->children;
	while(child != NULL)
	{
		child->parent = asTree;
		setParentPointers(child);
		child = child->nextSibling;
	}
}