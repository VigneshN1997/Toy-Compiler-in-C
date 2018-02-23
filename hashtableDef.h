#include "linked_list.c"

struct node
{
	keyword* word;
	struct node* next;
};

struct hash_node
{
	int num_nodes;
	struct node* first;
};

struct HASH_TABLE
{
	int entries;
	int size;
	struct hash_node* ptr;
};

typedef struct node node;
typedef struct hash_node hash_node;
typedef struct HASH_TABLE HASH_TABLE;
