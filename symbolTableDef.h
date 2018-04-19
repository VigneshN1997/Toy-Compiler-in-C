// ID: 2015A7PS0355P
// Name: Vignesh N
#include "errorReporting.c"

#define SYMBOL_TABLE_SIZE 100
typedef struct symbolTableEntry symbolTableEntry;
typedef struct SymbolTable SymbolTable;
typedef struct idInfo idInfo;
typedef struct funcInfo funcInfo;

struct idInfo
{
	int* widthInfo;
	SYMBOL_NAME type;
	int offset;
};

struct funcInfo
{
	ASTNode* opParameterList;
	int numOpParameters;
	ASTNode* ipParameterList;
	int numIpParameters;
};


struct symbolTableEntry
{
	char* idFuncLexeme;
	int isID;
	struct idInfo* idInfoPtr;
	struct funcInfo* funcInfoPtr;
	struct SymbolTable* ptrToCurrSymTable;
	struct SymbolTable* ptrToNewScopeST;
	struct symbolTableEntry* nextEntry; // entries hashed to same location will be stored using separate chaining technique
};

struct SymbolTable
{
	struct symbolTableEntry** arrOfSymbols;
	struct SymbolTable* ptrToParentST;
	char* scopeName;
	int currOffset;
	int numEntries;
	int tableSize;
	int nestingLevel;
};

