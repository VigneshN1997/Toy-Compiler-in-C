#include "grammar_preprocess.h"

void extractGrammarFromFile(FILE* fp)
{
	char* rule = NULL;
	size_t len = 0;
	ssize_t read;

	while((read = getline(&rule,&len,fp)) != -1)
	{
		processRule(rule,(int)read);
		// printf("len:%d\n",read);
	}
	if(rule)
	{
		free(rule);
	}
}

// void processRule(char* rule,int rule_len)
// {
	
// }