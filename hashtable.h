#include "def.h"

struct keyword
{
	char* name;
	TOKEN_NAME t_name;
};


struct node
{
	struct keyword* word;
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
	// double loadFactor;
	// int freeSlots;
	struct hash_node* ptr;
};

typedef struct keyword keyword;
typedef struct node node;
typedef struct hash_node hash_node;
typedef struct HASH_TABLE HASH_TABLE;

void addKeywords(keyword** keywordList, HASH_TABLE* ht, int n);
HASH_TABLE* createLookupTable(int size);
int hashingFunction(char* word, int size_of_ht);
void insertKeyword(HASH_TABLE* ht, keyword* word);
// int isSlotEmpty(HASH_TABLE* ht, int index);
keyword* findKeyword(HASH_TABLE* ht,char* word);
void printLookupTable(HASH_TABLE* ht);