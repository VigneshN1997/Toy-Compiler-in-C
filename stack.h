#include "firstfollow.c"

struct stack
{
	HEAD* top;
};

typedef struct stack STACK;

STACK* createStack();
void push(STACK* s,node_data* data);
void pop(STACK* s);
node_data* top(STACK* s);
STACK* initializeStackforParsing();
// int isEmpty(STACK* s);