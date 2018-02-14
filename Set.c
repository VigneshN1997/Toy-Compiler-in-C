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

void printSet(Set* A)
{
	int set_size = sizeof(A)/sizeof(A[0]);
	for(int i = 0; i < set_size; i++)
	{
		decToBinary(A[i].num);
	}
	printf("\n");
}

void decToBinary(unsigned int n)
{
    // array to store binary number
    int binaryNum[32] = {0};
 
    // counter for binary array
    int i = 0;
    while (n > 0) {
 
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
 
    // printing binary array in reverse order
    for (int j = element_size - 1; j >= 0; j--)
        printf("%d",binaryNum[j]);
    printf("\t");
}


/*int main(int argc, char const *argv[])
{
	Set* A = createSet(42);
	Set* B = createSet(42);
	setBit(A,0);
	printSet(A);
	setBit(A,2);
	printSet(A);
	setBit(A,4);
	printSet(A);
	setBit(A,5);
	printSet(A);
	setBit(A,6);
	printSet(A);
	setBit(B,1);
	printSet(B);
	setBit(B,3);
	printSet(B);
	setBit(B,63);
	printSet(B);
	setBit(B,7);
	printSet(B);
	setBit(B,9);
	printSet(B);
	Set* C = Union(A,B);
	printSet(C);
	return 0;
}*/