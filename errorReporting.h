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
	{1,"Variable not declared before its use"},
	{2,"Variable redeclaration error"},
	{3,"Matrix used before value is assigned to it"},
	{4,"Function called before being defined"},
	{5,"Function overloading not allowed"},
	{6,"type mismatch error"},
	{7, "matrix operation between matrices of incompatible sizes"},
	{8, "output parameter not assigned value"},
	{9, "number of input parameters in function call does not match with the number of formal input parameters"},
	{10, "types of actual and formal input parameters do not match"},
	{11, "recursive function calls are not allowed"},
	{12, "type mismatch error in size expression"}
	{13, "number of variables in lhs exceed than required in size expression"}
	{14,"number of lhs variables dont match number of output parameters of function"},
	{15,"matrix not initialized but matrix element used"},
	{16, "matrix row sizes not consistent"},
	{17,"string used before value is assigned to it"},
	{18,"matrix already assigned(new assignment cannot be of different size)"}
	{19,"string already assigned(new assignment cannot be of different size)"}

};


errorHead* initializeErrorHead();
void insertError(errorHead* h, Token* token,int error_no);
errorList* deleteErrorFromFront(errorHead* h);
errorSem* createNewError(int error_no);
