// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "lexerDef.h"
struct ll_node
{
	node_data* data;
	struct ll_node* next;
};

struct head
{
	int count;
	struct ll_node* first;
	struct ll_node* last;
};

typedef struct ll_node ll_node;
typedef struct head HEAD;

HEAD* initializeLinkedList();
void insertInFront(HEAD* h, node_data* data);
void insertAtEnd(HEAD* h, node_data* data);
node_data* deleteFromFront(HEAD* h);
void printLL(HEAD* h);
node_data* createNodeData(Token* token,grammar_var* variable,void* ptr_to_ParseTreeNode);