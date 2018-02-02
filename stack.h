#include "def.h"


struct stack_node
{
	stack_symbol* symbol;
	struct stack_node* next;
};

struct stack
{
	int size;
	struct stack_node* top;
};

typedef struct stack_node stack_node;
typedef struct stack STACK;

STACK* createStack();
void push(STACK* s,stack_symbol* s);
void pop(STACK* s);
stack_symbol* top(STACK* s);
int isEmpty(STACK* s);