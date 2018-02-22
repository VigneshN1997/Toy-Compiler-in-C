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
// void printSet(Set* A);
// void decToBinary(unsigned int n);
int compareSets(Set* A,Set* B);