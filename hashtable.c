// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "hashtable.h"

// get lookup table of keywords
HASH_TABLE* getLookupTable()
{
	HASH_TABLE* ht = createLookupTable(table_size);
	addKeywords(ht);
	return ht;
}

// add keywords to lookup table
void addKeywords(HASH_TABLE* ht)
{
	int num_keywords = sizeof(keyword_mapping)/sizeof(keyword_mapping[0]);
	int i;
	for(i = 0; i < num_keywords; i++)
	{
		insertKeyword(ht,keyword_mapping + i);
	}
}

// implement this
int hashingFunction(char* word, int size_of_ht)
{
	int hash = word[0] % size_of_ht;
	int c;
	for(int i = 1; i < strlen(word); i++)
	{
		c = word[i];
		hash = (hash*31 + c) % size_of_ht;
	}
	// printf("%s %d\n",word,hash);
	return hash%size_of_ht;
}

// create a new lookup table
HASH_TABLE* createLookupTable(int size)
{
	HASH_TABLE* h = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));
	h->entries = 0;
	h->size = size;
	// h->loadFactor = 0;
	// h->freeSlots = size;
	h->ptr = (hash_node*)malloc(size*sizeof(hash_node));
	hash_node* hs = h->ptr;
	for(int i = 0; i < size; i++)
	{
		hs[i].num_nodes = 0;
		hs[i].first = NULL;
	}
	return h;
}

// hash keyword into the table
void insertKeyword(HASH_TABLE* ht, keyword* word)
{
	ht->entries += 1;
	int index = hashingFunction(word->name,ht->size);
	hash_node* temp = (ht->ptr) + index;
	node* n = (node*)malloc(sizeof(node));
	n->word = word;
	n->next = NULL;

	if(temp->first == NULL)
	{
		temp->first = n;
		temp->num_nodes++;
	}
	else
	{
		node* curr = temp->first;
		while(curr->next != NULL)
		{
			curr = curr->next;
		}
		curr->next = n;
		temp->num_nodes++;
	}
}

// find a word in the table(if doesnt exist return NULL)
keyword* findKeyword(HASH_TABLE* ht,char* find_word)
{
	int index = hashingFunction(find_word,ht->size);
	// printf("index:%d\n",index);
	node* n = ((ht->ptr)+index)->first;
	if(n == NULL)
	{
		return NULL;
	}
	keyword* sym = n->word;
	if(strcmp(sym->name,find_word) == 0)
	{
		return sym;
	}
	else
	{
		n = n->next;
		while(n != NULL)
		{
			sym = n->word;
			if(strcmp(sym->name,find_word) == 0)
			{
				return sym;
			}
			n = n->next;
		}
	}
	return NULL;
}

// print hash table(for debugging)
void printLookupTable(HASH_TABLE* ht)
{
	int i = 0;
	hash_node* hs = ht->ptr;
	for(i = 0; i < ht->size; i++)
	{
		if(hs[i].first == NULL)
		{
			printf("NULL\n");
		}
		else
		{
			node* n = hs[i].first;
			while(n != NULL)
			{
				printf("%s:%d:%d->",(n->word)->name,(int)((n->word)->t_name),(int)strlen((n->word)->name));
				n = n->next;
			}
			printf("\n");
		}
	}
}
