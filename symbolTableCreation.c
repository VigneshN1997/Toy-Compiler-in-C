#include "symbolTableFunctions.c"

SymbolTable* createSymbolTable(ASTNode* asTree,errorHead* symTableErrorListHead)
{
	SymbolTable* symTable = createNewSymbolTable(SYMBOL_TABLE_SIZE);
	symTable->scopeName = (char*)malloc(6*sizeof(char));
	strcpy(symTable->scopeName,"_main");
	populateSymbolTable(asTree,symTable,symTableErrorListHead);
	return symTable;
}



symbolTableEntry* createSymbolTableEntry(Token* token,int isID,int width,SYMBOL_NAME type)
{
	symbolTableEntry* entry = (symbolTableEntry*)malloc(sizeof(symbolTableEntry));
	entry->idFuncLexeme = token->lexeme; // check this
	entry->isID = isID;
	entry->idInfoPtr = NULL;
	entry->funcInfoPtr = NULL;
	if(isID)
	{
		entry->idInfoPtr = (idInfo*)malloc(sizeof(idInfo));
		entry->idInfoPtr->widthInfo = NULL;
		if(type == MATRIX)
		{
			entry->idInfoPtr->widthInfo = (int*)malloc(2*sizeof(int));
			entry->idInfoPtr->widthInfo[0] = 0;
			entry->idInfoPtr->widthInfo[1] = 0;
		}
		else
		{
			entry->idInfoPtr->widthInfo = (int*)malloc(sizeof(int));
			*(entry->idInfoPtr->widthInfo) = width;
		}
		entry->idInfoPtr->type = type;
		entry->idInfoPtr->offset = -1;	
	}
	entry->ptrToNewScopeST = NULL;
	entry->nextEntry = NULL;
	return entry;
}

symbolTableEntry* insertIDorFunID(SymbolTable* symTable, Token* token,SYMBOL_NAME type)
{
	symTable->numEntries += 1;
	int index = hashingFunction(token->lexeme,symTable->tableSize); //check

	symbolTableEntry* entry = NULL;
	if(token ->t_name == ID)
	{
		if(type == INT)
		{
			entry = createSymbolTableEntry(token,1,2,INT); //check width
			entry->idInfoPtr->offset = symTable->currOffset;
			symTable->currOffset += 2;
		}
		else if(type == REAL)
		{
			entry = createSymbolTableEntry(token,1,4,REAL);
			entry->idInfoPtr->offset = symTable->currOffset;
			symTable->currOffset += 4;
		}
		else if(type == STRING)
		{
			entry = createSymbolTableEntry(token,1,0,STRING); //check width
		}
		else if(type == MATRIX)
		{
			entry = createSymbolTableEntry(token,1,0,MATRIX);
		}
	}
	else if(token->t_name == FUNID)
	{
		entry = createSymbolTableEntry(token,0,-1,FUNID);
	}

	symbolTableEntry** table = symTable->arrOfSymbols;
	if(table[index] == NULL)
	{
		table[index] = entry;
	}
	else
	{
		symbolTableEntry* firstEntry = table[index];
		table[index] = entry;
		entry->nextEntry = firstEntry;
	}

	if(token->t_name == FUNID)
	{
		SymbolTable* ptrToNewScopeST = createNewSymbolTable(symTable->tableSize);
		ptrToNewScopeST->scopeName = entry->idFuncLexeme;
		entry->ptrToNewScopeST = ptrToNewScopeST;
		ptrToNewScopeST->ptrToParentST = symTable;
	}
	return entry;
}

SymbolTable* createNewSymbolTable(int size)
{
	SymbolTable* symTable = (SymbolTable*)malloc(sizeof(SymbolTable));
	symTable->arrOfSymbols = (symbolTableEntry**)malloc(size*sizeof(symbolTableEntry*));
	for(int i = 0; i < size; i++)
	{
		symTable->arrOfSymbols[i] = NULL;
	}
	symTable->ptrToParentST = NULL;
	symTable->numEntries = 0;
	symTable->tableSize = size;
	symTable->scopeName = NULL;
	symTable->currOffset = 0;
	return symTable;
}

symbolTableEntry* findIdorFuncName(char* findId,SymbolTable* symTable)
{
	int index = hashingFunction(findId,symTable->tableSize);
	symbolTableEntry** table = symTable->arrOfSymbols;
	if(table[index] == NULL)
	{
		return NULL;
	}
	symbolTableEntry* entry = table[index];
	if(strcmp(entry->idFuncLexeme,findId) == 0)
	{
		return entry;
	}
	else
	{
		entry = entry->nextEntry;
		while(entry != NULL)
		{
			if(strcmp(entry->idFuncLexeme,findId) == 0)
			{
				return entry;
			}
			entry = entry->nextEntry;
		}
	}
	return NULL;
}

void populateSymbolTable(ASTNode* asTree,SymbolTable* symTable,errorHead* symTableErrorListHead)
{
	ASTNode* stmts = getListOfStmts(asTree);
	while(stmts != NULL)
	{
		if(stmts->op == DECLARATION_STMT)
		{
			processDeclarationStmt(stmts,symTable,symTableErrorListHead);
		}
		else if(stmts->op == ASSIGNOP && stmts->children->op == LHS_SINGLE_VAR)
		{
			processAssignmentStmtSingleVar(stmts,symTable,symTableErrorListHead);
		}
		else if(stmts->op == ASSIGNOP && stmts->children->op == LHS_LIST_VAR)
		{
			processAssignmentStmtListVar(stmts,symTable,symTableErrorListHead);
		}
		else if(stmts->op == IF)
		{
			processIfStmt(stmts,symTable,symTableErrorListHead);	
		}
		else if(stmts->op == READ || stmts->op == PRINT)
		{
			processIOStmt(stmts,symTable,symTableErrorListHead);
		}
		else if(stmts->op == FUNID)
		{
			processFunCallStmt(stmts,symTable,symTableErrorListHead);
		}
		else if(stmts->op == FUNCTION)
		{
			processFunctionDef(stmts,symTable,symTableErrorListHead);
		}
		stmts = stmts->nextSibling;
	}
}

ASTNode* getListOfStmts(ASTNode* asTree)
{
	if(asTree->op == MAIN)
	{
		return asTree->children;
	}
	else if(asTree->op == FUNCTION)
	{
		return asTree->children->nextSibling->nextSibling->children;
	}
	else if(asTree->op == IF)
	{
		return asTree->children->nextSibling->children;
	}
	else if(asTree->op == ELSE)
	{
		return asTree->children;
	}
}

void printFirstSymbolTable(SymbolTable* symTable)
{
	symbolTableEntry** syms = symTable->arrOfSymbols;
	for(int i = 0 ; i < symTable->tableSize; i++)
	{
		if(syms[i] == NULL)
		{
			printf("NULL\n");
		}
		else
		{
			symbolTableEntry* entry = syms[i];
			while(entry != NULL)
			{
				if(entry->isID)
				{
					printf("(lexeme:%s width:%d type:%d )",entry->idFuncLexeme,entry->idInfoPtr->widthInfo[0],(int)entry->idInfoPtr->type);
				}
				else
				{
					printf("(lexeme:%s nin:%d nout:%d",entry->idFuncLexeme,entry->funcInfoPtr->numIpParameters,entry->funcInfoPtr->numOpParameters);	
				}
				printf("---->\n");
				entry = entry->nextEntry;
			}
			printf("\n");
		}
	}
}

void printErrors(errorHead* h)
{
	errorList* temp = h->first;
	while(temp != NULL)
	{
		printf("(%d)%d %s %s\n",temp->token->line_no,temp->err->error_no,temp->err->errorMsg,temp->token->lexeme);
		temp = temp->next;
	}
}