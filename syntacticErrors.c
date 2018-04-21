// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "syntacticErrors.h"

errorHeadSyntactic* initializeErrorHeadSyntax()
{
	errorHeadSyntactic* head = (errorHeadSyntactic*)malloc(sizeof(errorHeadSyntactic));
	head->first = NULL;
	head->last = NULL;
	return head;
}
// insert a lexical error
void insertLexicalError(errorHeadSyntactic* h, Token* token)
{
	errorListSyn* n = (errorListSyn*)malloc(sizeof(errorListSyn));
	n->err_syn = NULL;
	n->errorType = 0;
	n->token = token;
	n->error_no = token->err->error_no;
	n->next = NULL;
	if(h->first == NULL)
	{
		h->first = n;
		h->last = n;
	}
	else
	{
		(h->last)->next = n;
		h->last = n;
	}
}
// insert syntax error
void insertSyntaxError(errorHeadSyntactic* h, Token* token,int error_no, char* expectedToken, char* presentToken)
{
	errorListSyn* n = (errorListSyn*)malloc(sizeof(errorListSyn));
	n->err_syn = createNewSyntaxError(expectedToken,presentToken);
	n->errorType = 1;
	n->token = token;
	n->error_no = error_no;
	n->next = NULL;
	if(h->first == NULL)
	{
		h->first = n;
		h->last = n;
	}
	else
	{
		(h->last)->next = n;
		h->last = n;
	}
}
// create new syntax error
errorSyn* createNewSyntaxError(char* expectedToken, char* presentToken)
{
	errorSyn* es = (errorSyn*)malloc(sizeof(errorSyn));
	es->expectedToken = NULL;
	if(expectedToken != NULL)
	{	
		es->expectedToken = (char*)malloc(30*sizeof(char));
		strcpy(es->expectedToken,expectedToken);
		es->expectedToken[strlen(es->expectedToken)] = '\0';
	}
	es->presentToken = NULL;
	if(presentToken != NULL)
	{
		es->presentToken = (char*)malloc(30*sizeof(char));
		strcpy(es->presentToken,presentToken);
		es->presentToken[strlen(es->presentToken)] = '\0';
	}
	return es;
}
// print syntax errors on console
void printSyntaxErrors(errorHeadSyntactic* syntaxErrorsHead)
{
	printf("-------------------------\n");
	printf("LEXICAL AND SYNTAX ERRORS \n");
	printf("-------------------------\n");
	errorListSyn* syntacticError = syntaxErrorsHead->first;		
	int error_no;
	while(syntacticError != NULL)
	{
		error_no = syntacticError->error_no;
		if(syntacticError->errorType == 0)
		{
			printf("line %d: Lexical error: %s (%s)\n",syntacticError->token->line_no,syntactic_error_msg_mapping[error_no-1].errorMsg,syntacticError->token->lexeme);
		}
		else
		{
			if(error_no == 4)
			{
				printf("line %d: Syntax error:Found token %s with lexeme %s\n",syntacticError->token->line_no,syntacticError->err_syn->presentToken,syntacticError->token->lexeme);
			}
			else if(error_no == 5)
			{
				printf("line %d: Syntax error: The token %s for lexeme %s does not match at line %d. The expected token here is %s.\n",syntacticError->token->line_no,syntacticError->err_syn->presentToken,syntacticError->token->lexeme,syntacticError->token->line_no,syntacticError->err_syn->expectedToken);
			}
			else
			{
				printf("Syntax error: %s\n",syntactic_error_msg_mapping[error_no-1].errorMsg);
			}
		}
		syntacticError = syntacticError->next;
	}
}