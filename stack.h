// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
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