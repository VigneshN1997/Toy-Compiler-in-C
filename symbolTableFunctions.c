// ID: 2015A7PS0355P
// Name: Vignesh N
#include "symbolTableCreation.h"
void processDeclarationStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead)
{
	SYMBOL_NAME type = stmt->children->op;
	ASTNode* var = stmt->children->nextSibling;
	symbolTableEntry* entry = NULL;
	while(var != NULL)
	{
		entry = findIdorFuncName(var->token->lexeme,symTable);
		if(entry != NULL)
		{
			// variable redeclaration
			insertError(symTableErrorListHead,var->token,2);
		}
		else
		{
			var->ptrToSymTableEntry = insertIDorFunID(symTable,var->token,type);
		}
		var = var->nextSibling;
	}
}

void processAssignmentStmtSingleVar(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead)
{
	ASTNode* lhs = stmt->children->children;
	ASTNode* rhs = stmt->children->nextSibling;
	symbolTableEntry* entry = checkIdExistence(lhs->token->lexeme,symTable);
	if(entry == NULL)
	{
		insertError(symTableErrorListHead,lhs->token,1);
	}
	else
	{
		lhs->ptrToSymTableEntry = entry;
	}
	if(rhs->op == SIZE)
	{
		ASTNode* id = rhs->children;
		entry = checkIdExistence(id->token->lexeme,symTable);
		if(entry == NULL)
		{
			insertError(symTableErrorListHead,lhs->token,1);		
		}
		else
		{
			id->ptrToSymTableEntry = entry;
		}
		// handle string type variables(if they have not been assigned thier size will be 0)
	}
	else if(rhs->op == FUNID)
	{
		processFunCallStmt(rhs,symTable,symTableErrorListHead);
	}
	else
	{
		processArithmeticExprST(rhs,symTable,symTableErrorListHead);
	}
}

void processAssignmentStmtListVar(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead)
{
	ASTNode* lhs = stmt->children->children;
	ASTNode* rhs = stmt->children->nextSibling;
	symbolTableEntry* entry = NULL;
	while(lhs != NULL)
	{
		entry = checkIdExistence(lhs->token->lexeme,symTable);
		if(entry == NULL)
		{
			insertError(symTableErrorListHead,lhs->token,1);
		}
		else
		{
			lhs->ptrToSymTableEntry = entry;
		}
		lhs = lhs->nextSibling;
	}
	if(rhs->op == SIZE)
	{
		ASTNode* id = rhs->children;
		entry = checkIdExistence(id->token->lexeme,symTable);
		if(entry == NULL)
		{
			insertError(symTableErrorListHead,lhs->token,1);		
		}
		else
		{
			id->ptrToSymTableEntry = entry;
		}
		// handle matrix type variables(if they have not been assigned thier size will be 0)
	}
	else if(rhs->op == FUNID)
	{
		processFunCallStmt(rhs,symTable,symTableErrorListHead);
	}
}

void processIfStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead,errorHead* typeCheckingErrorsHead)
{
	ASTNode* booleanExpr = stmt->children;
	ASTNode* elseStmt = stmt->children->nextSibling->nextSibling;
	processBooleanExpr(booleanExpr,symTable,symTableErrorListHead);
	typeCheckBooleanExpr(booleanExpr,symTable,typeCheckingErrorsHead);
	populateSymbolTable(stmt,symTable,symTableErrorListHead,typeCheckingErrorsHead);
	populateSymbolTable(elseStmt,symTable,symTableErrorListHead,typeCheckingErrorsHead);
}

void processIOStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead)
{
	ASTNode* var = stmt->children;
	symbolTableEntry* entry = checkIdExistence(var->token->lexeme,symTable);
	if(entry == NULL)
	{
		insertError(symTableErrorListHead,var->token,1);
	}
	else
	{
		var->ptrToSymTableEntry = entry;
	}
}

void processFunCallStmt(ASTNode* stmt,SymbolTable* symTable,errorHead* symTableErrorListHead)
{
	symbolTableEntry* entry = checkIdExistence(stmt->token->lexeme,symTable);
	if(entry == NULL)
	{
		insertError(symTableErrorListHead,stmt->token,4);	
	}
	stmt->ptrToSymTableEntry = entry;
	ASTNode* parameter = stmt->children;
	while(parameter != NULL)
	{
		if(parameter->op == ID)
		{
			entry = checkIdExistence(parameter->token->lexeme,symTable);
			if(entry == NULL)
			{
				insertError(symTableErrorListHead,parameter->token,1);			
			}
			parameter->ptrToSymTableEntry = entry;
		}
		parameter = parameter->nextSibling;
	}
}

void processFunctionDef(ASTNode* stmt, SymbolTable* symTable,errorHead* symTableErrorListHead,errorHead* typeCheckingErrorsHead)
{
	ASTNode* parameterListIn = stmt->children->children;
	ASTNode* parameterListOut = stmt->children->nextSibling->children;
	symbolTableEntry* entry = findIdorFuncName(stmt->token->lexeme,symTable);
	if(entry != NULL)
	{
		insertError(symTableErrorListHead,stmt->token,5);
		// should I still process the function stmts??
	}
	else
	{
		entry = insertIDorFunID(symTable, stmt->token,FUNID);
		stmt->ptrToSymTableEntry = entry; // is it needed?
		entry->funcInfoPtr = (funcInfo*)malloc(sizeof(funcInfo));
		entry->funcInfoPtr->ipParameterList = parameterListIn;
		entry->funcInfoPtr->opParameterList = parameterListOut;
		ASTNode* temp = parameterListIn;
		int numParams = 0;
		while(temp != NULL)
		{
			numParams++;
			insertIDorFunID(entry->ptrToNewScopeST,temp->children->token,temp->op);
			temp = temp->nextSibling;	
		}
		entry->funcInfoPtr->numIpParameters = numParams;
		numParams = 0;
		temp = parameterListOut;
		while(temp != NULL)
		{
			numParams++;
			insertIDorFunID(entry->ptrToNewScopeST,temp->children->token,temp->op);
			temp = temp->nextSibling;	
		}
		entry->funcInfoPtr->numOpParameters = numParams;

		populateSymbolTable(stmt,entry->ptrToNewScopeST,symTableErrorListHead,typeCheckingErrorsHead);
	}
}

void processBooleanExpr(ASTNode* expr, SymbolTable* symTable,errorHead* symTableErrorListHead)
{
	if(expr->op == ID)
	{
		symbolTableEntry* entry = checkIdExistence(expr->token->lexeme,symTable);
		if(entry == NULL)
		{
			insertError(symTableErrorListHead,expr->token,1);
		}
		else
		{
			expr->ptrToSymTableEntry = entry;
		}
		return;
	}
	else
	{
		ASTNode* child = expr->children;
		while(child != NULL)
		{
			processBooleanExpr(child,symTable,symTableErrorListHead);
			child = child->nextSibling;
		}
	}
}

void processArithmeticExprST(ASTNode* expr, SymbolTable* symTable,errorHead* symTableErrorListHead)
{
	if(expr->op == ID)
	{
		symbolTableEntry* entry = checkIdExistence(expr->token->lexeme,symTable);
		if(entry == NULL)
		{
			insertError(symTableErrorListHead,expr->token,1);
		}
		else
		{
			expr->ptrToSymTableEntry = entry;
		}
		return;
	}
	else
	{
		ASTNode* child = expr->children;
		while(child != NULL)
		{
			processArithmeticExprST(child,symTable,symTableErrorListHead);
			child = child->nextSibling;
		}
	}
}

symbolTableEntry* checkIdExistence(char* tokenLexeme, SymbolTable* symTable)
{
	symbolTableEntry* entry = findIdorFuncName(tokenLexeme,symTable);
	if(entry != NULL)
	{
		return entry;
	}
	SymbolTable* parentSymTable = symTable->ptrToParentST;
	while(parentSymTable != NULL)
	{
		entry = findIdorFuncName(tokenLexeme,parentSymTable);
		if(entry != NULL)
		{
			return entry;
		}
		parentSymTable = parentSymTable->ptrToParentST;
	}
	return NULL;
}

// special handling for matrix