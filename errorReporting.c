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
	err->errorMsg = semantic_error_msg_mapping[error_no].errorMsg; // check this
	return err;
}

