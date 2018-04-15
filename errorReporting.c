#include "errorReporting.h"


errorHead* initializeErrorHead()
{
	errorHead* head = (errorHead*)malloc(sizeof(errorHead));
	head->first = NULL;
	head->last = NULL;
	return head;
}

void insertError(errorHead* h, Token* token,int error_no)
{
	errorSem* err = createNewError(error_no);
	errorList* node = (errorList*)malloc(sizeof(errorList));
	node->err = err;
	node->token = token;
	node->next = NULL;
	if(h->first == NULL)
	{
		h->first = node;
		h->last = node;
	}
	else
	{
		(h->last)->next = node;
		h->last = node;
	}
}

errorList* deleteErrorFromFront(errorHead* h)
{
	if(h->first == NULL)
	{
		return NULL;
	}
	errorList* temp = h->first;
	h->first = temp->next;
	return temp;
}

errorSem* createNewError(int error_no)
{
	errorSem* err = (errorSem*)malloc(sizeof(errorSem));
	err->error_no = error_no;
	err->errorMsg = semantic_error_msg_mapping[error_no - 1].errorMsg; // check this
	return err;
}

void printErrors(errorHead* symTableErrorHead,errorHead* typeCheckingErrorHead)
{
	FILE* fp;
	fp = fopen("errors.txt","w");
	errorList* symTableError = symTableErrorHead->first;
	errorList* typeCheckingError = typeCheckingErrorHead->first;		
	while(symTableError != NULL && typeCheckingError != NULL)
	{
		if(symTableError->token->line_no <= typeCheckingError->token->line_no)
		{
			fprintf(fp,"line %d : %s (%s)\n",symTableError->token->line_no,symTableError->err->errorMsg,symTableError->token->lexeme);
			symTableError = symTableError->next;
		}
		else
		{
			fprintf(fp,"line %d : %s (%s)\n",typeCheckingError->token->line_no,typeCheckingError->err->errorMsg,typeCheckingError->token->lexeme);
			typeCheckingError = typeCheckingError->next;
		}
	} 
	while(symTableError != NULL)
	{
		fprintf(fp,"line %d : %s (%s)\n",symTableError->token->line_no,symTableError->err->errorMsg,symTableError->token->lexeme);
		symTableError = symTableError->next;
	}
	while(typeCheckingError != NULL)
	{
		fprintf(fp,"line %d : %s (%s)\n",typeCheckingError->token->line_no,typeCheckingError->err->errorMsg,typeCheckingError->token->lexeme);
		typeCheckingError = typeCheckingError->next;
	}
	fclose(fp);
}