#include "symbolTableCreation.h"

symbolTableEntry* createSymbolTableEntry(Token* token,int isID,int width,SYMBOL_NAME type)
{
	symbolTableEntry* entry = (symbolTableEntry*)malloc(sizeof(symbolTableEntry));
	entry->idFuncLexeme = token->lexeme; // check this
	entry->isID = isID;
	entry->idInfoPtr = NULL;
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

void insertIDorFunID(SymbolTable* symTable, Token* token,SYMBOL_NAME type)
{
	symTable->numEntries += 1;
	int index = hashingFunction(token->lexeme,symTable->size); //check

	symbolTableEntry* entry = NULL;
	if(token ->t_name == ID)
	{
		if(type == INT)
		{
			entry = createSymbolTableEntry(token,1,4,INT); //check width
		}
		else if(type == REAL)
		{
			entry = createSymbolTableEntry(token,1,4,REAL);
		}
		else if(type == STRING)
		{
			entry = createSymbolTableEntry(token,1,0,STRING); //check width
		}
		else if(type == MATRIX)
		{
			entry = createSymbolTableEntry(token,1,0,MATRIX)
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
		SymbolTable* ptrToNewScopeST = createNewSymbolTable(symTable->size);
		ptrToNewScopeST->ptrToParentST = symTable;
	}
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
	symTable->size = size;
	return symTable;
}
