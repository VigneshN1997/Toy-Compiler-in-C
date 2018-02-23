#include "linked_list.h"
node_data* createNodeData(Token* token,grammar_var* variable,void* ptr_to_ParseTreeNode)
{
	node_data* data = (node_data*)malloc(sizeof(node_data));
	data->token = token;
	data->variable = variable;
	data->ptr_to_ParseTreeNode = ptr_to_ParseTreeNode;
	return data;
}



HEAD* initializeLinkedList()
{
	HEAD* h = (HEAD*)malloc(sizeof(HEAD));
	h->count = 0;
	h->first = NULL;
	h->last = NULL;
	return h;
}

void insertInFront(HEAD* h, node_data* data)
{
	ll_node* n = (ll_node*)malloc(sizeof(ll_node));
	n->data = data;	
	if(h->count == 0)
	{
		n->next = NULL;
		h->first = n;
		h->last = n;
	}
	else
	{
		n->next = h->first;
		h->first = n;
	}
	h->count++;
}

void insertAtEnd(HEAD* h, node_data* data)
{
	ll_node* n = (ll_node*)malloc(sizeof(ll_node));
	n->data = data;
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
	h->count++;
}

node_data* deleteFromFront(HEAD* h)
{
	if(h->count == 0)
	{
		return NULL;
	}
	ll_node* temp = h->first;
	h->first = temp->next;
	h->count--;
	return temp->data;
}

void printLL(HEAD* h)
{
	if(h->count != 0)
	{
		ll_node* temp = h->first;
		while(temp != NULL)
		{
			if((temp->data)->token != NULL)
			{
				printf("token:%d\t",(temp->data)->token->t_name);
			}
			else
			{
				printf("variable:%d type:%d\n",(temp->data)->variable->sym_name,(temp->data)->variable->sym_type);
			}
			temp = temp->next;
		}
	}
	else
	{
		printf("NULL\n");
	}
}