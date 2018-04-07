#include "ASTCreation.c"

typedef struct symbolTableEntry symbolTableEntry;
typedef struct SymbolTable SymbolTable;
typedef struct idInfo idInfo;

struct symbolTableEntry
{
	char* idFuncLexeme;
	int isID;
	struct idInfo* idInfoPtr;
	struct SymbolTable* ptrToNewScopeST;
	struct symbolTableEntry* nextEntry; // entries hashed to same location will be stored using separate chaining technique
};

struct SymbolTable
{
	struct symbolTableEntry** arrOfSymbols;
	struct SymbolTable* ptrToParentST;
	int numEntries;
	int tableSize;
};

struct idInfo
{
	int* widthInfo;
	SYMBOL_NAME type;
	int offset;
};