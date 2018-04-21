// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "hashtableDef.h"


HASH_TABLE* getLookupTable();
void addKeywords(HASH_TABLE* ht);
HASH_TABLE* createLookupTable(int size);
int hashingFunction(char* word, int size_of_ht);
void insertKeyword(HASH_TABLE* ht, keyword* word);
keyword* findKeyword(HASH_TABLE* ht,char* word);
void printLookupTable(HASH_TABLE* ht);