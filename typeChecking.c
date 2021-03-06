// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "typeChecking.h"
// type check assignment type 1 statement
void typeCheckAssignmentStmtSingleVar(ASTNode* assgnStmt,SymbolTable* symTable, errorHead* typeCheckingErrorsHead)
{
	ASTNode* lhs = assgnStmt->children->children;
	ASTNode* rhs = assgnStmt->children->nextSibling;
	if(lhs->ptrToSymTableEntry == NULL)
	{
		lhs->type = EPSILON; // error type
	}
	else
	{
		lhs->type = ((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->type;
	}
	if(rhs->op == SIZE)
	{
		ASTNode* sizeVar = rhs->children;
		if(((symbolTableEntry*)sizeVar->ptrToSymTableEntry) != NULL && ((symbolTableEntry*)sizeVar->ptrToSymTableEntry)->idInfoPtr->type != STRING)
		{
			// error
			insertError(typeCheckingErrorsHead,sizeVar->token,12);
		}
		else if(lhs->type != INT)
		{
			insertError(typeCheckingErrorsHead,lhs->token,6);
		}
	}
	else if(rhs->op == FUNID)
	{
		typeCheckFunCallStmt(lhs,1,rhs,symTable,typeCheckingErrorsHead);
	}
	else
	{
		typeCheckArithmeticExpr(rhs,symTable,typeCheckingErrorsHead);
		// check types of lhs and rhs
		if(lhs->type != rhs->type)
		{
			// check if any other error has to be given(check token to pass)
			insertError(typeCheckingErrorsHead,lhs->token,6);
		}
		// do matrix/string size assignment in symbolTable
		else
		{
			if(lhs->ptrToSymTableEntry != NULL)
			{
				if(((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->type == MATRIX && rhs->widthInfo != NULL && rhs->widthInfo[0] != 0)
				{
					if(((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->widthInfo[0] != 0)
					{
						if(((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->widthInfo[0] != rhs->widthInfo[0] || ((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->widthInfo[1] != rhs->widthInfo[1])
						{
							insertError(typeCheckingErrorsHead,lhs->token,18);
						}
					}
					else
					{
						((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->widthInfo[0] = rhs->widthInfo[0];
						((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->widthInfo[1] = rhs->widthInfo[1];
						if(((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->offset == -1)
						{
							((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->offset = ((symbolTableEntry*)lhs->ptrToSymTableEntry)->ptrToCurrSymTable->currOffset;	
							((symbolTableEntry*)lhs->ptrToSymTableEntry)->ptrToCurrSymTable->currOffset += (rhs->widthInfo[0]*rhs->widthInfo[1]*2);
						}
					}
				}
				else if(((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->type == STRING && rhs->widthInfo != NULL && rhs->widthInfo[0] != 0)
				{
					if(((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->widthInfo[0] != 0)
					{
						if(((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->widthInfo[0] != rhs->widthInfo[0])
						{
							insertError(typeCheckingErrorsHead,lhs->token,19);
						}
					}
					else
					{
						((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->widthInfo[0] = rhs->widthInfo[0];
						if(((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->offset == -1)
						{
							((symbolTableEntry*)lhs->ptrToSymTableEntry)->idInfoPtr->offset = ((symbolTableEntry*)lhs->ptrToSymTableEntry)->ptrToCurrSymTable->currOffset;	
							((symbolTableEntry*)lhs->ptrToSymTableEntry)->ptrToCurrSymTable->currOffset += rhs->widthInfo[0];
						}
					}
				}
			}
		}
		// if sizes are already present then throw an error
	}
}
// type check assignment type 2 statement
void typeCheckAssignmentStmtListVar(ASTNode* assgnStmt,SymbolTable* symTable, errorHead* typeCheckingErrorsHead)
{
	ASTNode* lhs = assgnStmt->children->children;
	ASTNode* rhs = assgnStmt->children->nextSibling;
	ASTNode* temp = lhs;
	int numLHSVars = 0;
	while(temp != NULL)
	{
		if(temp->ptrToSymTableEntry == NULL)
		{
			temp->type = EPSILON; // error type
		}
		else
		{
			temp->type = ((symbolTableEntry*)temp->ptrToSymTableEntry)->idInfoPtr->type;
		}
		numLHSVars++;
		temp = temp->nextSibling;
	}
	if(rhs->op == SIZE)
	{
		ASTNode* sizeVar = rhs->children;
		if(sizeVar->ptrToSymTableEntry != NULL)
		{
			SYMBOL_NAME sizeVarType = ((symbolTableEntry*)sizeVar->ptrToSymTableEntry)->idInfoPtr->type;
			if(sizeVarType != STRING && sizeVarType != MATRIX)
			{
				// error
				insertError(typeCheckingErrorsHead,sizeVar->token,12);
			}
			else if(sizeVarType == STRING)
			{
				if(numLHSVars != 1)
				{
					insertError(typeCheckingErrorsHead,sizeVar->token,13);
				}
				else if(lhs->type != INT)
				{
					insertError(typeCheckingErrorsHead,lhs->token,6);	
				}
			}
			else if(sizeVarType == MATRIX)
			{
				if(numLHSVars != 2)
				{
					insertError(typeCheckingErrorsHead,sizeVar->token,13);	
				}
				else if(lhs->type != INT || lhs->nextSibling->type != INT)
				{
					if(lhs->type != INT)
					{
						insertError(typeCheckingErrorsHead,lhs->token,6);
					}
					if(lhs->nextSibling->type != INT)
					{
						insertError(typeCheckingErrorsHead,lhs->nextSibling->token,6);
					}	
				}
			}	
		}
	}
	else if(rhs->op == FUNID)
	{
		typeCheckFunCallStmt(lhs,numLHSVars,rhs,symTable,typeCheckingErrorsHead);
	}
}
// type check function call statement
void typeCheckFunCallStmt(ASTNode* lhs,int numLHSVars,ASTNode* funCallStmt,SymbolTable* symTable,errorHead* typeCheckingErrorsHead)
{
	if(((symbolTableEntry*)funCallStmt->ptrToSymTableEntry) != NULL)
	{
		if(recursionPresent(funCallStmt,symTable))
		{
			insertError(typeCheckingErrorsHead,funCallStmt->token,11);
		}
		ASTNode* inputParameterList = NULL;
		int numParams = 0;
		ASTNode* temp = NULL;
		if(numLHSVars != ((symbolTableEntry*)funCallStmt->ptrToSymTableEntry)->funcInfoPtr->numOpParameters)
		{
			insertError(typeCheckingErrorsHead,funCallStmt->token,14);
		}
		else
		{
			temp = ((symbolTableEntry*)funCallStmt->ptrToSymTableEntry)->funcInfoPtr->opParameterList;
			ASTNode* lhsvar = lhs;
			while(lhsvar != NULL)
			{
				if(lhsvar->ptrToSymTableEntry != NULL)
				{
					if(temp->op != ((symbolTableEntry*)lhsvar->ptrToSymTableEntry)->idInfoPtr->type)
					{
						insertError(typeCheckingErrorsHead,lhsvar->token,20); // should I break here ?
					}
				}
				temp = temp->nextSibling;
				lhsvar = lhsvar->nextSibling;
			}
		}
		inputParameterList = funCallStmt->children;
		temp = inputParameterList;
		while(temp != NULL)
		{
			numParams++;
			temp = temp->nextSibling;
		}
		if(numParams != ((symbolTableEntry*)funCallStmt->ptrToSymTableEntry)->funcInfoPtr->numIpParameters)
		{
			insertError(typeCheckingErrorsHead,funCallStmt->token,9);		
		}
		else
		{
			temp = inputParameterList;
			ASTNode* formalParameterList = ((symbolTableEntry*)funCallStmt->ptrToSymTableEntry)->funcInfoPtr->ipParameterList;
			ASTNode* formalParam = formalParameterList;
			while(temp != NULL)
			{
				if(temp->ptrToSymTableEntry != NULL)
				{
					if(temp->op == ID && temp->children != NULL)
					{
						int dim1 = ((symbolTableEntry*)temp->ptrToSymTableEntry)->idInfoPtr->widthInfo[0];
						int dim2 = ((symbolTableEntry*)temp->ptrToSymTableEntry)->idInfoPtr->widthInfo[1];
						int ind1 = (temp->children->token->value).int_value;
						int ind2 = (temp->children->nextSibling->token->value).int_value;
						if(ind1 > dim1 || ind2 > dim2 || ind1 < 1 || ind2 < 1)
						{
							insertError(typeCheckingErrorsHead,temp->token,22);
						}
						if(formalParam->op != INT)
						{
							insertError(typeCheckingErrorsHead,temp->token,10);		
						}
					}
					else if(((symbolTableEntry*)temp->ptrToSymTableEntry)->idInfoPtr->type != formalParam->op)
					{
						insertError(typeCheckingErrorsHead,temp->token,10); // should I break here ?
					}
				}
				else if(temp->op == NUM && formalParam->op != INT)
				{
					insertError(typeCheckingErrorsHead,temp->token,10);
				}
				else if(temp->op == RNUM && formalParam->op != REAL)
				{
					insertError(typeCheckingErrorsHead,temp->token,10);
				}
				else if(temp->op == MATRIX && formalParam->op != MATRIX)
				{
					Token* tok = (Token*)malloc(sizeof(Token));
					tok->lexeme = (char*)malloc(7*sizeof(char));
					strcpy(tok->lexeme,"matrix");
					tok->lexeme[strlen(tok->lexeme)] = '\0';
					tok->t_name = MATRIX;
					tok->line_no = funCallStmt->token->line_no;
					insertError(typeCheckingErrorsHead,tok,10);
				}
				else if(temp->op == MATRIX)
				{
					temp->widthInfo = extractMatrixSize(temp,typeCheckingErrorsHead);
				}
				else if(temp->op == STR && formalParam->op != STRING)
				{
					insertError(typeCheckingErrorsHead,temp->token,10);
				}
				temp = temp->nextSibling;
				formalParam = formalParam->nextSibling;
			}
		}
	}
}
// check if recursion is present
int recursionPresent(ASTNode* funCallStmt, SymbolTable* symTable)
{
	SymbolTable* par = symTable;
	while(par != NULL)
	{
		symbolTableEntry* entry = findIdorFuncName(funCallStmt->token->lexeme,par);
		if(entry != NULL)
		{
			return 0;
		}
		else if(strcmp(par->scopeName,funCallStmt->token->lexeme) == 0)
		{
			return 1;
		}
		par = par->ptrToParentST;
	}
	return 0;
}
// type check function definition
void typeCheckFunction(ASTNode* func, SymbolTable* symTable,errorHead* typeCheckingErrorsHead)
{
	ASTNode* funcStmts = func->children->nextSibling->nextSibling->children;
	if((symbolTableEntry*)func->ptrToSymTableEntry != NULL) // DO WE NEED TO CHECK FOR OVERLOADED FUNCTIONS?
	{
		int numOpParameters = ((symbolTableEntry*)func->ptrToSymTableEntry)->funcInfoPtr->numOpParameters;
		int* assigned = (int*)malloc(numOpParameters*sizeof(int));
		for(int i = 0; i < numOpParameters; i++)
		{
			assigned[i] = 0;
		}
		checkOuputParametersAssignment(funcStmts,((symbolTableEntry*)func->ptrToSymTableEntry)->funcInfoPtr->opParameterList,assigned,0,symTable,symTable);
		ASTNode* opParam = func->children->nextSibling->children;
		for(int i = 0; i < numOpParameters; i++)
		{
			if(assigned[i] == 0)
			{
				insertError(typeCheckingErrorsHead,opParam->children->token,8);
			}
			opParam = opParam->nextSibling;
		}
	}
}
// check if all output parameters in a function definition are assigned a value
void checkOuputParametersAssignment(ASTNode* funcStmts,ASTNode* opParameterList,int* assigned,int level,SymbolTable* funcSymTable,SymbolTable* currSymTable)
{
	ASTNode* stmt = funcStmts;
	int index = 0;
	int found = 0;
	while(stmt != NULL)
	{
		if(stmt->op == ASSIGNOP || stmt->op == READ)
		{
			if(stmt->op == READ)
			{
				ASTNode* var = stmt->children;
				ASTNode* opParam = opParameterList;
				index = 0;
				found = 0;
				while(opParam != NULL)
				{
					if(strcmp(var->token->lexeme,opParam->children->token->lexeme) == 0)
					{
						found = 1;
						break;
					}
					index++;
					opParam = opParam->nextSibling;
				}
				if(found)
				{
					if(level != 0)
					{
						SymbolTable* par = currSymTable;
						int presentInInnerScope = 0;
						while(par != funcSymTable)
						{
							symbolTableEntry* entry = findIdorFuncName(var->token->lexeme,par);
							if(entry != NULL)
							{
								presentInInnerScope = 1;
								break;
							}
							par = par->ptrToParentST;
						}
						if(!presentInInnerScope)
						{
							assigned[index] = 1;
						}
					}
					else
					{
						assigned[index] = 1;
					}
				}
			}
			else
			{
				ASTNode* lhsVars = stmt->children->children;
				while(lhsVars != NULL)
				{
					ASTNode* opParam = opParameterList;
					index = 0;
					found = 0;
					while(opParam != NULL)
					{
						if(strcmp(lhsVars->token->lexeme,opParam->children->token->lexeme) == 0)
						{
							found = 1;
							break;
						}
						index++;
						opParam = opParam->nextSibling;
					}
					if(found)
					{
						if(level != 0)
						{
							SymbolTable* par = currSymTable;
							int presentInInnerScope = 0;
							while(par != funcSymTable)
							{
								symbolTableEntry* entry = findIdorFuncName(lhsVars->token->lexeme,par);
								if(entry != NULL)
								{
									presentInInnerScope = 1;
									break;
								}
								par = par->ptrToParentST;
							}
							if(!presentInInnerScope)
							{
								assigned[index] = 1;
							}
						}
						else
						{
							assigned[index] = 1;
						}
					}
					lhsVars = lhsVars->nextSibling;
				}
			}
		}
		else if(stmt->op == FUNCTION)
		{
			SymbolTable* nextSymTable = ((symbolTableEntry*)stmt->ptrToSymTableEntry)->ptrToNewScopeST;
			checkOuputParametersAssignment(stmt->children->nextSibling->nextSibling->children,opParameterList,assigned,1,funcSymTable,nextSymTable);
		}
		stmt = stmt->nextSibling;
	}
}
// type check boolean expression
void typeCheckBooleanExpr(ASTNode* boolExpr,SymbolTable* symTable,errorHead* typeCheckingErrorsHead)
{
	if(boolExpr->op == ID)
	{
		if(boolExpr->ptrToSymTableEntry != NULL)
		{
			boolExpr->type = ((symbolTableEntry*)boolExpr->ptrToSymTableEntry)->idInfoPtr->type;
		}
		else
		{
			boolExpr->type = EPSILON;
		}
	}
	else if(boolExpr->op == NUM)
	{
		boolExpr->type = INT;
	}
	else if(boolExpr->op == RNUM)
	{
		boolExpr->type = REAL;
	}
	else
	{
		ASTNode* child = boolExpr->children;
		while(child != NULL)
		{
			typeCheckBooleanExpr(child,symTable,typeCheckingErrorsHead);
			child = child->nextSibling;
		}
		SYMBOL_NAME child1Type;
		SYMBOL_NAME child2Type;
		if(boolExpr->op == AND || boolExpr->op == OR)
		{
			child1Type = boolExpr->children->type;
			child2Type = boolExpr->children->nextSibling->type;
			// BOOL_EXPR represents boolean type
			if(child1Type == BOOL_EXPR && child2Type == BOOL_EXPR)
			{
				boolExpr->type = BOOL_EXPR;
			}
			else
			{
				insertError(typeCheckingErrorsHead,boolExpr->token,6);
				boolExpr->type = EPSILON;
			}
		}
		else if(boolExpr->op == NOT)
		{
			child1Type = boolExpr->children->type;
			if(child1Type == BOOL_EXPR)
			{
				boolExpr->type = BOOL_EXPR;	
			}
			else
			{
				insertError(typeCheckingErrorsHead,boolExpr->token,6);
				boolExpr->type = EPSILON;
			}
		}
		else
		{
			child1Type = boolExpr->children->type;
			child2Type = boolExpr->children->nextSibling->type;
			if((child1Type == INT) && (child2Type == INT))
			{
				boolExpr->type = BOOL_EXPR;
			}
			else if((child1Type == REAL) && (child2Type == REAL))
			{
				boolExpr->type = BOOL_EXPR;
			}
			else
			{
				insertError(typeCheckingErrorsHead,boolExpr->token,6);
				boolExpr->type = EPSILON;
			}
		}
		// else if(boolExpr->op == LT || boolExpr->op == LE || boolExpr->op == EQ || boolExpr->op == GT || boolExpr->op == GE || boolExpr->op == NE)
		// {

		// }
	}
}
// type check arithmetic expression
void typeCheckArithmeticExpr(ASTNode* arithmeticExpr,SymbolTable* symTable,errorHead* typeCheckingErrorsHead)
{
	if(arithmeticExpr->op == ID)
	{
		if(arithmeticExpr->children != NULL)
		{
			if(arithmeticExpr->ptrToSymTableEntry != NULL)
			{
				// matrix element bounds check can be done?
				if(((symbolTableEntry*)arithmeticExpr->ptrToSymTableEntry)->idInfoPtr->widthInfo[0] != 0)
				{
					arithmeticExpr->type = INT;
					int dim1 = ((symbolTableEntry*)arithmeticExpr->ptrToSymTableEntry)->idInfoPtr->widthInfo[0];
					int dim2 = ((symbolTableEntry*)arithmeticExpr->ptrToSymTableEntry)->idInfoPtr->widthInfo[1];
					int ind1 = (arithmeticExpr->children->token->value).int_value;
					int ind2 = (arithmeticExpr->children->nextSibling->token->value).int_value;
					if(ind1 > dim1 || ind2 > dim2 || ind1 < 1 || ind2 < 1)
					{
						insertError(typeCheckingErrorsHead,arithmeticExpr->token,22);
					}
				}
				else
				{
					insertError(typeCheckingErrorsHead,arithmeticExpr->token,15);
					arithmeticExpr->type = INT;
				}
			}
			else
			{
				arithmeticExpr->type = EPSILON;	
			}
		}
		else
		{
			if(arithmeticExpr->ptrToSymTableEntry != NULL)
			{
				arithmeticExpr->type = ((symbolTableEntry*)arithmeticExpr->ptrToSymTableEntry)->idInfoPtr->type;
				if(arithmeticExpr->type == MATRIX)
				{
					if(((symbolTableEntry*)arithmeticExpr->ptrToSymTableEntry)->idInfoPtr->widthInfo[0] == 0)
					{
						insertError(typeCheckingErrorsHead,arithmeticExpr->token,3);
						arithmeticExpr->widthInfo = (int*)malloc(2*sizeof(int));
						arithmeticExpr->widthInfo[0] = 0;
						arithmeticExpr->widthInfo[1] = 0;
					}
					else
					{
						arithmeticExpr->widthInfo = (int*)malloc(2*sizeof(int));
						arithmeticExpr->widthInfo[0] = ((symbolTableEntry*)arithmeticExpr->ptrToSymTableEntry)->idInfoPtr->widthInfo[0];
						arithmeticExpr->widthInfo[1] = ((symbolTableEntry*)arithmeticExpr->ptrToSymTableEntry)->idInfoPtr->widthInfo[1];
					}
				}	
				else if(arithmeticExpr->type == STRING)
				{
					if(((symbolTableEntry*)arithmeticExpr->ptrToSymTableEntry)->idInfoPtr->widthInfo[0] == 0)
					{
						insertError(typeCheckingErrorsHead,arithmeticExpr->token,17);
						arithmeticExpr->widthInfo = (int*)malloc(sizeof(int));
						arithmeticExpr->widthInfo[0] = 0;
					}
					else
					{
						arithmeticExpr->widthInfo = (int*)malloc(sizeof(int));
						arithmeticExpr->widthInfo[0] = ((symbolTableEntry*)arithmeticExpr->ptrToSymTableEntry)->idInfoPtr->widthInfo[0];
					}
				}		
			}
			else
			{
				arithmeticExpr->type = EPSILON;	
			}
		}
	}
	else if(arithmeticExpr->op == NUM)
	{
		arithmeticExpr->type = INT;
	}
	else if(arithmeticExpr->op == RNUM)
	{
		arithmeticExpr->type = REAL;
	}
	else if(arithmeticExpr->op == STR)
	{
		arithmeticExpr->type = STRING;
		arithmeticExpr->widthInfo = extractStringSize(arithmeticExpr);
	}
	else if(arithmeticExpr->op == MATRIX)
	{
		arithmeticExpr->type = MATRIX;
		arithmeticExpr->widthInfo = extractMatrixSize(arithmeticExpr,typeCheckingErrorsHead);	
	}
	else
	{
		ASTNode* child = arithmeticExpr->children;
		while(child != NULL)
		{
			typeCheckArithmeticExpr(child,symTable,typeCheckingErrorsHead);
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
				arithmeticExpr->type = INT;
			}
			else if(child1Type == REAL && child2Type == REAL)
			{
				arithmeticExpr->type = REAL;
			}
			else if(child1Type == STRING && child2Type == STRING)
			{
				arithmeticExpr->type = STRING;
				arithmeticExpr->widthInfo = (int*)malloc(sizeof(int));
				arithmeticExpr->widthInfo[0] = child1Width[0] + child2Width[0];
			}
			else if(child1Type == MATRIX && child2Type == MATRIX)
			{
				arithmeticExpr->type = MATRIX;
				if(child1Width[0] == child2Width[0] && child1Width[1] == child2Width[1])
				{
					arithmeticExpr->widthInfo = child1Width;
				}
				else
				{
					insertError(typeCheckingErrorsHead,arithmeticExpr->token,7);
					arithmeticExpr->widthInfo = (int*)malloc(2*sizeof(int));
					arithmeticExpr->widthInfo[0] = 0;
					arithmeticExpr->widthInfo[1] = 0;
				}
			}
			else
			{
				arithmeticExpr->type = EPSILON;
				insertError(typeCheckingErrorsHead,arithmeticExpr->token,6);
			}
		}
		else if(arithmeticExpr->op == MINUS)
		{
			if(child1Type == INT && child2Type == INT)
			{
				arithmeticExpr->type = INT;
			}
			else if(child1Type == REAL && child2Type == REAL)
			{
				arithmeticExpr->type = REAL;
			}
			else if(child1Type == MATRIX && child2Type == MATRIX)
			{
				arithmeticExpr->type = MATRIX;
				if(child1Width[0] == child2Width[0] && child1Width[1] == child2Width[1])
				{
					arithmeticExpr->widthInfo = child1Width;
				}
				else
				{
					insertError(typeCheckingErrorsHead,arithmeticExpr->token,7);
					arithmeticExpr->widthInfo = (int*)malloc(2*sizeof(int));
					arithmeticExpr->widthInfo[0] = 0;
					arithmeticExpr->widthInfo[1] = 0;
				}
			}
			else
			{
				arithmeticExpr->type = EPSILON;
				insertError(typeCheckingErrorsHead,arithmeticExpr->token,6);
			}
		}
		else if(arithmeticExpr->op == MUL)
		{
			if(child1Type == INT && child2Type == INT)
			{
				arithmeticExpr->type = INT;
			}
			else if(child1Type == REAL && child2Type == REAL)
			{
				arithmeticExpr->type = REAL;
			}
			else
			{
				arithmeticExpr->type = EPSILON;
				insertError(typeCheckingErrorsHead,arithmeticExpr->token,6);
			}
		}
		else if(arithmeticExpr->op == DIV)
		{
			if(child1Type == INT && child2Type == INT)
			{
				arithmeticExpr->type = REAL;
			}
			else if(child1Type == REAL && child2Type == REAL)
			{
				arithmeticExpr->type = REAL;
			}
			else
			{
				arithmeticExpr->type = EPSILON;
				insertError(typeCheckingErrorsHead,arithmeticExpr->token,6);
			}
		}
	}
}
// get string length
int* extractStringSize(ASTNode* stringNode)
{
	int* len = (int*)malloc(sizeof(int));
	len[0] = strlen(stringNode->token->lexeme) - 2;
	return len;
}
// get matrix size
int* extractMatrixSize(ASTNode* matrixNode,errorHead* typeCheckingErrorsHead)
{
	int* width = (int*)malloc(2*sizeof(int));
	int numRows = 0;
	int currRowSize = 0;
	int numElemsPerRow = 0;
	ASTNode* row = matrixNode->children;
	while(row != NULL)
	{
		numRows++;
		ASTNode* num = row->children;
		while(num != NULL)
		{	
			currRowSize++;
			num = num->nextSibling;
		}
		if(numRows > 1) // i.e. not the first row
		{
			if(currRowSize != numElemsPerRow)
			{
				insertError(typeCheckingErrorsHead,row->children->token,16);
				width[0] = 0;
				width[1] = 0;
				return width;
			}
		}
		else // first row
		{
			numElemsPerRow = currRowSize;
		}
		currRowSize = 0;
		row = row->nextSibling;
	}

	if(numRows > 10 || numElemsPerRow > 10)
	{
		Token* tok = (Token*)malloc(sizeof(Token));
		tok->lexeme = (char*)malloc(10*sizeof(char));
		tok->line_no = matrixNode->children->children->token->line_no;
		snprintf(tok->lexeme,10,"%dx%d",width[0],width[1]);
		insertError(typeCheckingErrorsHead,tok,21);
		width[0] = 0;
		width[1] = 0;
	}
	else
	{
		width[0] = numRows;
		width[1] = numElemsPerRow;
	}
	return width;
}
// type check io statement
void typeCheckIOStmt(ASTNode* ioStmt,SymbolTable* symTable,errorHead* typeCheckingErrorsHead)
{
	symbolTableEntry* entry = ((symbolTableEntry*)ioStmt->children->ptrToSymTableEntry);
	SYMBOL_NAME operation = ioStmt->op;
	if(entry != NULL)
	{
		if(operation == READ)
		{
			if(entry->idInfoPtr->type == STRING || entry->idInfoPtr->type == MATRIX)
			{
				insertError(typeCheckingErrorsHead,ioStmt->children->token,6);
			}
		}
	}
}