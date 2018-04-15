#include "intCodeGen.h"

void createIntermediateCode(ASTNode* asTree,SymbolTable* symTable, int* tempVarNum,int* labelVarNum)
{
	ASTNode* stmts = getListOfStmts(asTree);
	while(stmts != NULL)
	{
		if(stmts->op == ASSIGNOP && stmts->children->op == LHS_SINGLE_VAR)
		{
			generateCodeFor_AssignmentStmtSingleVar(stmts,symTable,tempVarNum);
		}
		else if(stmts->op == IF)
		{
			generateCodeFor_IfStmt(stmts,symTable,tempVarNum,labelVarNum);
		}
		stmts = stmts->nextSibling;
	}
}

void appendAllStatements(ASTNode* mainNode)
{
	ASTNode* stmts = mainNode->children;
	mainNode->code = NULL;
	while(stmts != NULL)
	{
		mainNode->code = appendCodes((codeNode*)mainNode->code,(codeNode*)stmts->code);
		stmts = stmts->nextSibling;
	}
}


void generateCodeFor_AssignmentStmtSingleVar(ASTNode* assignStmt, SymbolTable* symTable, int* tempVarNum)
{
	ASTNode* lhs = assignStmt->children->children;
	ASTNode* rhs = assignStmt->children->nextSibling;
	if(rhs->op == SIZE)
	{

	}
	else if(rhs->op == FUNID)
	{
	
	}
	else
	{
		generateCodeFor_ArithmeticExpr(rhs,symTable,tempVarNum);
		// assign to lhs
		codeNode* line = generateThreeAddrCode(lhs,ASSIGN_OP,rhs->tempVar,NULL);
		assignStmt->code = appendCodes((codeNode*)rhs->code,line);
	}
}

void generateCodeFor_IfStmt(ASTNode* ifStmt,SymbolTable* symTable,int* tempVarNum,int* labelVarNum)
{
	ASTNode* boolExpr = ifStmt->children;
	ASTNode* elsePart = ifStmt->children->nextSibling->nextSibling->children;
	ifStmt->nextLabel = newLabelVar(labelVarNum);
	
	ifStmt->children->nextSibling->nextLabel = ifStmt->nextLabel;
	ifStmt->children->nextSibling->nextSibling->nextLabel = NULL;

	boolExpr->attrBool = (boolCodeAttr*)malloc(sizeof(boolCodeAttr));
	((boolCodeAttr*)boolExpr->attrBool)->trueLabel = newLabelVar(labelVarNum);
	((boolCodeAttr*)boolExpr->attrBool)->falseLabel = ifStmt->nextLabel;
	if(elsePart != NULL)
	{
		((boolCodeAttr*)boolExpr->attrBool)->falseLabel = newLabelVar(labelVarNum);
		ifStmt->children->nextSibling->nextSibling->nextLabel = ifStmt->nextLabel;
	}
	generateCodeFor_BoolExpr(boolExpr,labelVarNum);
	createIntermediateCode(ifStmt,symTable,tempVarNum,labelVarNum);
	appendAllStatements(ifStmt->children->nextSibling);
	createIntermediateCode(elsePart,symTable,tempVarNum,labelVarNum);
	appendAllStatements(elsePart);

	codeNode* ifStmtsNext = generateThreeAddrCode(ifStmt->nextLabel,LABEL,NULL,NULL);
	codeNode* condnTrue = generateThreeAddrCode(((boolCodeAttr*)boolExpr->attrBool)->trueLabel,LABEL,NULL,NULL);
	codeNode* condnFalse = generateThreeAddrCode(((boolCodeAttr*)boolExpr->attrBool)->falseLabel,LABEL,NULL,NULL);
	if(elsePart != NULL)
	{
		codeNode* tempCode = appendCodes((codeNode*)elsePart->code,ifStmtsNext);
		tempCode = appendCodes(condnFalse,tempCode);
		codeNode* gotoLine = generateThreeAddrCode(ifStmt->nextLabel,GOTO,NULL,NULL);
		tempCode = appendCodes(gotoLine,tempCode);
		tempCode = appendCodes((codeNode*)ifStmt->children->nextSibling->code,tempCode);
		tempCode = appendCodes(condnTrue,tempCode);
		ifStmt->code = appendCodes(boolExpr->code,tempCode);
	}
	else
	{
		codeNode* tempCode = appendCodes((codeNode*)ifStmt->children->nextSibling->code,ifStmtsNext);
		tempCode = appendCodes(condnTrue,tempCode);
		ifStmt->code = appendCodes(boolExpr->code,tempCode);
	}
}

ASTNode* newLabelVar(int* labelVarNum)
{
	Token* tok = (Token*)malloc(sizeof(Token));
	tok->line_no = -1;
	tok->t_name = ID;
	tok->lexeme = (char*)malloc(5*sizeof(char));
	strcpy(tok->lexeme,"L");
	char* strTempLabelNum = (char*)malloc(3*sizeof(char));
	snprintf(strTempLabelNum,3,"%d",*labelVarNum);
	strcat(tok->lexeme,strTempLabelNum);
	ASTNode* labelVar = createASTNode(ID,tok,NULL);
	*labelVarNum = *labelVarNum + 1;
	return labelVar;
}

ASTNode* newTempVar(int* tempVarNum,SymbolTable* symTable, SYMBOL_NAME type)
{
	Token* tok = (Token*)malloc(sizeof(Token));
	tok->line_no = -1;
	tok->t_name = ID;
	tok->lexeme = (char*)malloc(5*sizeof(char));
	strcpy(tok->lexeme,"t");
	char* strTempVarNum = (char*)malloc(3*sizeof(char));
	snprintf(strTempVarNum,3,"%d",*tempVarNum);
	strcat(tok->lexeme,strTempVarNum);
	ASTNode* tempVar = createASTNode(ID,tok,NULL);
	tempVar->ptrToSymTableEntry = insertIDorFunID(symTable,tok,type);
	*tempVarNum = *tempVarNum + 1;
	return tempVar;
}

void generateCodeFor_BoolExpr(ASTNode* boolExpr, int* labelVarNum)
{
	if(boolExpr->op == ID || boolExpr->op == NUM || boolExpr->op == RNUM)
	{
		boolExpr->tempVar = boolExpr;
		boolExpr->code = NULL;
	}
	else
	{
		ASTNode* child1 = boolExpr->children;
		ASTNode* child2 = NULL;
		if(boolExpr->op == AND)
		{
			child2 = boolExpr->children->nextSibling;
			child1->attrBool = (boolCodeAttr*)malloc(sizeof(boolCodeAttr));
			child2->attrBool = (boolCodeAttr*)malloc(sizeof(boolCodeAttr));
			((boolCodeAttr*)child1->attrBool)->trueLabel = newLabelVar(labelVarNum);
			((boolCodeAttr*)child1->attrBool)->falseLabel = ((boolCodeAttr*)boolExpr->attrBool)->falseLabel;
			((boolCodeAttr*)child2->attrBool)->trueLabel = ((boolCodeAttr*)boolExpr->attrBool)->trueLabel;
			((boolCodeAttr*)child2->attrBool)->falseLabel = ((boolCodeAttr*)boolExpr->attrBool)->falseLabel;
			generateCodeFor_BoolExpr(child1,labelVarNum);
			generateCodeFor_BoolExpr(child2,labelVarNum);
			codeNode* child1True = generateThreeAddrCode(((boolCodeAttr*)child1->attrBool)->trueLabel,LABEL,NULL,NULL);
			codeNode* tempCode = appendCodes(child1True,(codeNode*)child2->code);
			boolExpr->code = appendCodes((codeNode*)child1->code,tempCode);
		}
		else if(boolExpr->op == OR)
		{
			child2 = boolExpr->children->nextSibling;
			child1->attrBool = (boolCodeAttr*)malloc(sizeof(boolCodeAttr));
			child2->attrBool = (boolCodeAttr*)malloc(sizeof(boolCodeAttr));
			((boolCodeAttr*)child1->attrBool)->trueLabel = ((boolCodeAttr*)boolExpr->attrBool)->trueLabel;
			((boolCodeAttr*)child1->attrBool)->falseLabel = newLabelVar(labelVarNum);
			((boolCodeAttr*)child2->attrBool)->trueLabel = ((boolCodeAttr*)boolExpr->attrBool)->trueLabel;
			((boolCodeAttr*)child2->attrBool)->falseLabel = ((boolCodeAttr*)boolExpr->attrBool)->falseLabel;
			generateCodeFor_BoolExpr(child1,labelVarNum);
			generateCodeFor_BoolExpr(child2,labelVarNum);
			codeNode* child1False = generateThreeAddrCode(((boolCodeAttr*)child1->attrBool)->falseLabel,LABEL,NULL,NULL);
			codeNode* tempCode = appendCodes(child1False,(codeNode*)child2->code);
			boolExpr->code = appendCodes((codeNode*)child1->code,tempCode);
		}
		else if(boolExpr->op == NOT)
		{
			child1->attrBool = (boolCodeAttr*)malloc(sizeof(boolCodeAttr));
			((boolCodeAttr*)child1->attrBool)->trueLabel = ((boolCodeAttr*)boolExpr->attrBool)->falseLabel;
			((boolCodeAttr*)child1->attrBool)->falseLabel = ((boolCodeAttr*)boolExpr->attrBool)->trueLabel;
			generateCodeFor_BoolExpr(child1,labelVarNum);
			boolExpr->code = child1->code;
		}
		else
		{
			child2 = boolExpr->children->nextSibling;
			generateCodeFor_BoolExpr(child1,labelVarNum);
			generateCodeFor_BoolExpr(child2,labelVarNum);
			codeNode* line1 = generateThreeAddrCode(((boolCodeAttr*)boolExpr->attrBool)->trueLabel,LT_OP,child1->tempVar,child2->tempVar);
			if(boolExpr->op == LT)
			{
				line1->code->op = LT_OP;
			}
			else if(boolExpr->op == LE)
			{
				line1->code->op = LE_OP;
			}
			else if(boolExpr->op == EQ)
			{
				line1->code->op = EQ_OP;
			}
			else if(boolExpr->op == GT)
			{
				line1->code->op = GT_OP;
			}
			else if(boolExpr->op == GE)
			{
				line1->code->op = GE_OP;
			}
			else if(boolExpr->op == NE)
			{
				line1->code->op = NE_OP;
			}
			codeNode* line2 = generateThreeAddrCode(((boolCodeAttr*)boolExpr->attrBool)->falseLabel,GOTO,NULL,NULL);
			boolExpr->code = appendCodes(line1,line2);
		}
	}
}

void generateCodeFor_ArithmeticExpr(ASTNode* arithmeticExpr,SymbolTable* symTable,int* tempVarNum)
{
	if(arithmeticExpr->op == ID)
	{
		if(arithmeticExpr->children != NULL)
		{

		}
		else
		{
			if(arithmeticExpr->type == MATRIX)
			{

			}	
			else if(arithmeticExpr->type == STRING)
			{

			}
			else
			{
				arithmeticExpr->tempVar = arithmeticExpr;
				arithmeticExpr->code = NULL;
			}		
		}
	}
	else if(arithmeticExpr->op == NUM)
	{
		arithmeticExpr->tempVar = arithmeticExpr;
		arithmeticExpr->code = NULL;
	}
	else if(arithmeticExpr->op == RNUM)
	{
		arithmeticExpr->tempVar = arithmeticExpr;
		arithmeticExpr->code = NULL;
	}
	else if(arithmeticExpr->op == STR)
	{

	}
	else if(arithmeticExpr->op == MATRIX)
	{

	}
	else
	{
		ASTNode* child = arithmeticExpr->children;
		while(child != NULL)
		{
			generateCodeFor_ArithmeticExpr(child,symTable,tempVarNum);
			child = child->nextSibling;
		}
		SYMBOL_NAME child1Type = arithmeticExpr->children->type;
		SYMBOL_NAME child2Type = arithmeticExpr->children->nextSibling->type;
		int* child1Width = arithmeticExpr->children->widthInfo;
		int* child2Width = arithmeticExpr->children->nextSibling->widthInfo;
		if(arithmeticExpr->op == PLUS)
		{
			if(child1Type == INT && child2Type == INT)
			{
				arithmeticExpr->tempVar = newTempVar(tempVarNum,symTable,INT);
				codeNode* line = generateThreeAddrCode(arithmeticExpr->tempVar,PLUS_OP,arithmeticExpr->children->tempVar,arithmeticExpr->children->nextSibling->tempVar);
				codeNode* tempCode = appendCodes((codeNode*)arithmeticExpr->children->nextSibling->code,line);
				arithmeticExpr->code = appendCodes(arithmeticExpr->children->code,tempCode);
			}
			else if(child1Type == REAL && child2Type == REAL)
			{
				arithmeticExpr->tempVar = newTempVar(tempVarNum,symTable,REAL);
				codeNode* line = generateThreeAddrCode(arithmeticExpr->tempVar,PLUS_OP,arithmeticExpr->children->tempVar,arithmeticExpr->children->nextSibling->tempVar);
				codeNode* tempCode = appendCodes((codeNode*)arithmeticExpr->children->nextSibling->code,line);
				arithmeticExpr->code = appendCodes(arithmeticExpr->children->code,tempCode);
			}
			else if(child1Type == STRING && child2Type == STRING)
			{
					
			}
			else if(child1Type == MATRIX && child2Type == MATRIX)
			{
				
			}
		}
		else if(arithmeticExpr->op == MINUS)
		{
			if(child1Type == INT && child2Type == INT)
			{
				arithmeticExpr->tempVar = newTempVar(tempVarNum,symTable,INT);
				codeNode* line = generateThreeAddrCode(arithmeticExpr->tempVar,MINUS_OP,arithmeticExpr->children->tempVar,arithmeticExpr->children->nextSibling->tempVar);
				codeNode* tempCode = appendCodes((codeNode*)arithmeticExpr->children->nextSibling->code,line);
				arithmeticExpr->code = appendCodes(arithmeticExpr->children->code,tempCode);
			}
			else if(child1Type == REAL && child2Type == REAL)
			{
				arithmeticExpr->tempVar = newTempVar(tempVarNum,symTable,REAL);
				codeNode* line = generateThreeAddrCode(arithmeticExpr->tempVar,MINUS_OP,arithmeticExpr->children->tempVar,arithmeticExpr->children->nextSibling->tempVar);
				codeNode* tempCode = appendCodes((codeNode*)arithmeticExpr->children->nextSibling->code,line);
				arithmeticExpr->code = appendCodes(arithmeticExpr->children->code,tempCode);
			}
			else if(child1Type == MATRIX && child2Type == MATRIX)
			{

			}
		}
		else if(arithmeticExpr->op == MUL)
		{
			if(child1Type == INT && child2Type == INT)
			{
				arithmeticExpr->tempVar = newTempVar(tempVarNum,symTable,INT);
				codeNode* line = generateThreeAddrCode(arithmeticExpr->tempVar,MUL_OP,arithmeticExpr->children->tempVar,arithmeticExpr->children->nextSibling->tempVar);
				codeNode* tempCode = appendCodes((codeNode*)arithmeticExpr->children->nextSibling->code,line);
				arithmeticExpr->code = appendCodes(arithmeticExpr->children->code,tempCode);
			}
			else if(child1Type == REAL && child2Type == REAL)
			{
				arithmeticExpr->tempVar = newTempVar(tempVarNum,symTable,REAL);
				codeNode* line = generateThreeAddrCode(arithmeticExpr->tempVar,MUL_OP,arithmeticExpr->children->tempVar,arithmeticExpr->children->nextSibling->tempVar);
				codeNode* tempCode = appendCodes((codeNode*)arithmeticExpr->children->nextSibling->code,line);
				arithmeticExpr->code = appendCodes(arithmeticExpr->children->code,tempCode);
			}
		}
		else if(arithmeticExpr->op == DIV)
		{
			if((child1Type == INT || child1Type == REAL) && (child2Type == INT || child2Type == REAL))
			{
				arithmeticExpr->tempVar = newTempVar(tempVarNum,symTable,REAL);
				codeNode* line = generateThreeAddrCode(arithmeticExpr->tempVar,DIV_OP,arithmeticExpr->children->tempVar,arithmeticExpr->children->nextSibling->tempVar);
				codeNode* tempCode = appendCodes((codeNode*)arithmeticExpr->children->nextSibling->code,line);
				arithmeticExpr->code = appendCodes(arithmeticExpr->children->code,tempCode);
			}
		}
	}
}

codeNode* generateThreeAddrCode(ASTNode* resVar, OPERATOR op,ASTNode* var1, ASTNode* var2)
{
	threeAddrCode* tac = (threeAddrCode*)malloc(sizeof(threeAddrCode));
	tac->resVar = resVar;
	tac->op = op;
	tac->var1 = var1;
	tac->var2 = var2;
	codeNode* cNode = (codeNode*)malloc(sizeof(codeNode));
	cNode->code = tac;
	cNode->nextLine = NULL;
	return cNode;
}

codeNode* appendCodes(codeNode* c1, codeNode* c2)
{
	if(c1 == NULL)
	{
		return c2;
	}
	codeNode* temp = c1;
	while(temp->nextLine != NULL)
	{
		temp = temp->nextLine;
	}
	temp->nextLine = c2;
	return c1;
}

void printIntCode(codeNode* c)
{
	FILE* fp = fopen("code.txt","w");
	codeNode* temp = c;
	while(temp != NULL)
	{
		if(temp->code->op == ASSIGN_OP)
		{
			fprintf(fp,"%s = %s\n",temp->code->resVar->token->lexeme,temp->code->var1->token->lexeme);
		}
		else if(temp->code->op == PLUS_OP || temp->code->op == MINUS_OP || temp->code->op == MUL_OP || temp->code->op == DIV_OP)
		{
			fprintf(fp,"%s = %s %s %s\n",temp->code->resVar->token->lexeme,temp->code->var1->token->lexeme,operator_mapping[temp->code->op].op_str,temp->code->var2->token->lexeme);
		}
		else if(temp->code->op == LABEL)
		{
			fprintf(fp, "%s:\n",temp->code->resVar->token->lexeme);
		}
		else if(temp->code->op == LT_OP || temp->code->op == LE_OP || temp->code->op == EQ_OP || temp->code->op == GT_OP || temp->code->op == GE_OP || temp->code->op == NE_OP)
		{
			fprintf(fp, "if (%s %s %s) goto: %s\n",temp->code->var1->token->lexeme,operator_mapping[temp->code->op].op_str,temp->code->var2->token->lexeme,temp->code->resVar->token->lexeme);
		}
		else if(temp->code->op == GOTO)
		{
			fprintf(fp, "goto:%s\n",temp->code->resVar->token->lexeme);
		}
		temp = temp->nextLine;
	}
	fclose(fp);
}