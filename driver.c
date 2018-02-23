#include "parser.c"
int main(int argc, char* argv[])
{
	HEAD* tokenList = getAllTokens(argv[1]);
	FILE* g_file = fopen("grammar.txt","r");
	Grammar* g = extractGrammarFromFile(g_file);
	char* name = (char*)malloc(20*sizeof(char));
	strcpy(name,"testcase.txt");
	// printGrammar(g);
	// printGrammarRulesReverse(g);
	// printRHSOccurrences(g);
	ComputeFirstSets(g);
	ComputeFollowSets(g);
	// printFirstSets(g);
	// printFollowSets(g);
	parsingTable pTable = createParsingTable(g);
	// printParsingTable(pTable);
	ParseTree ptree = parseInput(tokenList,pTable);
	printParseTree(ptree,argv[2]);
	return 0;
}