#include "Set.h"

Set* createSet(int size)
{
	int num_elems;
	if(size % element_size == 0)
	{
		num_elems = size / element_size;
	}
	else
	{
		num_elems = (size / element_size) + 1;
	}
	Set* s1 = (Set*)malloc(num_elems*sizeof(Set));
	for(int i = 0; i < num_elems; i++)
	{
		s1[i].num = 0;
	}
	return s1;
}

Set* Union(Set* A, Set* B)
{
	int set_size = sizeof(A)/sizeof(A[0]);
	Set* union_set = (Set*)malloc(set_size*sizeof(Set));
	for(int i = 0; i < set_size; i++)
	{
		union_set[i].num = A[i].num | B[i].num;
	}
	// printf("%d\n",set_size);
	return union_set;
}

void setBit(Set* A,int i)
{
	int set_size = sizeof(A)/sizeof(A[0]);
	int index = i / element_size;
	int bit_num = i % element_size;
	unsigned int j = 1 << bit_num;
	A[index].num = A[index].num | j;
}

int compareSets(Set* A,Set* B)
{
	int equal = 1;
	int set_size = sizeof(A)/sizeof(A[0]);
	for(int i = 0; i < set_size; i++)
	{
		if(A[i].num != B[i].num)
		{
			equal = 0;
			break;
		}
	}
	return equal;
}