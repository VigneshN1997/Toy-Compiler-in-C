// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "lexer.c"
#define element_size 32

struct Set
{
	unsigned int num;
};

typedef struct Set Set;

Set* createSet(int size);
Set* Union(Set* A, Set* B);
void setBit(Set* A,int i);
int compareSets(Set* A,Set* B);