#include "stack.h"

STACK* createStack()
{
	STACK* st = (STACK*)malloc(sizeof(STACK));
	st->top = initializeLinkedList();
	return st;
}

void push(STACK* s,node_data* data)
{
	insertInFront(s->top, data);
}

node_data* top(STACK* s)
{
	return ((s->top)->first)->data;
}

void pop(STACK* s)
{
	node_data* data = deleteFromFront(s->top);
}

STACK* initializeStackforParsing(void* ptr_to_ParseTreeNode)
{
	STACK* st = createStack();
	grammar_var* variable = (grammar_var*)malloc(sizeof(grammar_var));
	variable->sym_str = NULL;
	variable->sym_name = $;
	variable->sym_type = T;
	node_data* n = createNodeData(NULL,variable,NULL);
	
	grammar_var* main_variable = (grammar_var*)malloc(sizeof(grammar_var));
	main_variable->sym_str = (char*)malloc(20*sizeof(char));
	strcpy(main_variable->sym_str,"mainFunction");
	main_variable->sym_name = MAIN_FUNCTION;
	main_variable->sym_type = NT;
	node_data* n2 = createNodeData(NULL,main_variable,ptr_to_ParseTreeNode);
	
	push(st,n);
	push(st,n2);
	return st;
}