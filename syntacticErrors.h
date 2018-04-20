// ID: 2015A7PS0355P
// Name: Vignesh N
#include "stack.c"

struct errMsgStruct
{
	int error_no;
	char* errorMsg;
};

struct errorSyn
{
	char* expectedToken;
	char* presentToken;
};

struct errorListSyn
{
	struct errorSyn* err_syn;
	int errorType; // 0:lexical, 1:syntactic
	Token* token;
	int error_no;
	struct errorListSyn* next;
};

struct errorHeadSyntactic
{
	struct errorListSyn* first;
	struct errorListSyn* last;
};

typedef struct errMsgStruct errMsgStruct;
typedef struct errorSyn errorSyn;
typedef struct errorListSyn errorListSyn;
typedef struct errorHeadSyntactic errorHeadSyntactic;

errMsgStruct syntactic_error_msg_mapping[] = {
	{1, "Indentifier is longer than prescribed length"},
	{2, "Unknown symbol "},
	{3, "Unknown pattern "},
	{4, "syntax error 1"},
	{5, "syntax error 2"},
	{6, "PDA stack empty"},
	{7, "Some(terminal/ non terminal) left on stack"}
};


errorHeadSyntactic* initializeErrorHeadSyntax();
void insertLexicalError(errorHeadSyntactic* h, Token* token);
void insertSyntaxError(errorHeadSyntactic* h, Token* token,int error_no, char* expectedToken, char* presentToken);
errorSyn* createNewSyntaxError(char* expectedToken, char* presentToken);
void printSyntaxErrors(errorHeadSyntactic* syntaxErrorsHead);