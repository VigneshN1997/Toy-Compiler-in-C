// ID: 2015A7PS0355P
// Name: Vignesh N
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
	errorList* lnode = (errorList*)malloc(sizeof(errorList));
	lnode->err = err;
	lnode->token = token;
	lnode->next = NULL;
	if(h->first == NULL)
	{
		h->first = lnode;
		h->last = lnode;
	}
	else
	{
		(h->last)->next = lnode;
		h->last = lnode;
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
	printf("----------------------------------\n");
	printf("TYPE CHECKING AND SEMANTIC ERRORS \n");
	printf("----------------------------------\n");
	FILE* fp;
	fp = stdout;
	errorList* symTableError = symTableErrorHead->first;
	errorList* typeCheckingError = typeCheckingErrorHead->first;		
	while(symTableError != NULL && typeCheckingError != NULL)
	{
		if(symTableError->token->line_no <= typeCheckingError->token->line_no)
		{
			if(symTableError->token != NULL)
			{
				fprintf(fp,"line %d : Semantic error:%s (%s)\n",symTableError->token->line_no,symTableError->err->errorMsg,symTableError->token->lexeme);
			}
			else
			{
				fprintf(fp,"line : Semantic error:%s\n",symTableError->err->errorMsg);	
			}
			symTableError = symTableError->next;
		}
		else
		{
			if(typeCheckingError->token != NULL)
			{
				fprintf(fp,"line %d : Semantic error:%s (%s)\n",typeCheckingError->token->line_no,typeCheckingError->err->errorMsg,typeCheckingError->token->lexeme);
			}
			else
			{
				fprintf(fp,"line : Semantic error:%s \n",typeCheckingError->err->errorMsg);	
			}
			typeCheckingError = typeCheckingError->next;
		}
	} 
	while(symTableError != NULL)
	{
		if(symTableError->token != NULL)
		{
			fprintf(fp,"line %d : Semantic error:%s (%s)\n",symTableError->token->line_no,symTableError->err->errorMsg,symTableError->token->lexeme);
		}
		else
		{
			fprintf(fp,"line : Semantic error:%s\n",symTableError->err->errorMsg);	
		}
		symTableError = symTableError->next;
	}
	while(typeCheckingError != NULL)
	{
		if(typeCheckingError->token != NULL)
		{
			fprintf(fp,"line %d : Semantic error:%s (%s)\n",typeCheckingError->token->line_no,typeCheckingError->err->errorMsg,typeCheckingError->token->lexeme);
		}
		else
		{
			fprintf(fp,"line : Semantic error:%s \n",typeCheckingError->err->errorMsg);	
		}
		typeCheckingError = typeCheckingError->next;
	}
}