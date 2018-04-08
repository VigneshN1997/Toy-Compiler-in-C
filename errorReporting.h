#include "ASTCreation.c"

struct errorSem
{
	int error_no;
	char* errorMsg;
};

struct errorList
{
	struct errorSem* err;
	Token* token;
	struct errorList* next;
};

struct errorHead
{
	struct errorList* first;
	struct errorList* last;
};

typedef struct errorSem errorSem;
typedef struct errorList errorList;
typedef struct errorHead errorHead;

errorSem semantic_error_msg_mapping[] = {
	{1,"Variable not declared before its use."},
	{2,"Variable redeclaration."},
	{3,"Matrix used before value is assigned to it."},
	{4,"Function called before being defined."},
	{5,"Function overloading not allowed"}
};


errorHead* initializeErrorHead();
void insertError(errorHead* h, Token* token,int error_no);
errorList* deleteErrorFromFront(errorHead* h);
errorSem* createNewError(int error_no);
