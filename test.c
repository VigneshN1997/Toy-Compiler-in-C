#include "hashtable.h"

int main(int argc, char const *argv[])
{
	int size = 24;
	int num_keywords = 12;
	HASH_TABLE* ht = createLookupTable(size);
	// {"end","int","real","string","matrix","_main","if","else","endif","read","print","function"};
	TOKEN_NAME tokens[] = {END,INT,REAL,STRING,MATRIX,MAIN,IF,ELSE,ENDIF,READ,PRINT,FUNCTION};
	char** words = (char**)malloc(num_keywords*sizeof(char*));
	words[0] = (char*)malloc(20*sizeof(char));
	words[1] = (char*)malloc(20*sizeof(char));
	words[2] = (char*)malloc(20*sizeof(char));
	words[3] = (char*)malloc(20*sizeof(char));
	words[4] = (char*)malloc(20*sizeof(char));
	words[5] = (char*)malloc(20*sizeof(char));
	words[6] = (char*)malloc(20*sizeof(char));
	words[7] = (char*)malloc(20*sizeof(char));
	words[8] = (char*)malloc(20*sizeof(char));
	words[9] = (char*)malloc(20*sizeof(char));
	words[10] = (char*)malloc(20*sizeof(char));
	words[11] = (char*)malloc(20*sizeof(char));
	strcpy(words[0],"end");
	strcpy(words[1],"int");
	strcpy(words[2],"real");
	strcpy(words[3],"string");
	strcpy(words[4],"matrix");
	strcpy(words[5],"_main");
	strcpy(words[6],"if");
	strcpy(words[7],"else");
	strcpy(words[8],"endif");
	strcpy(words[9],"read");
	strcpy(words[10],"print");
	strcpy(words[11],"function");


	
	keyword** keywordList = (keyword**)malloc(num_keywords*sizeof(keyword*));
	for(int i = 0; i < num_keywords; i++)
	{
		keywordList[i] = (keyword*)malloc(sizeof(keyword));
		keywordList[i]->name = words[i];
		keywordList[i]->t_name = tokens[i];
	}
	addKeywords(keywordList,ht,num_keywords);
	printLookupTable(ht);
	for(int i = 0; i < num_keywords; i++)
	{
		keyword* k = findKeyword(ht,words[i]);
		if(k!=NULL)
		{
			printf("type:%d\n",k->t_name);
		}
	}

	return 0;
}