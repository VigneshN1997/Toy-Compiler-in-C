// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "codeGenerator.c"
int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("usage: ./stage1exe testcase.txt code.asm\n");
		return 0;
	}
	printf("LEVEL 4:AST, Symbol table, type checking, semantic rules,code generation modules work.");
	HEAD* tokenList = initializeLinkedList();
	FILE* g_file = fopen("grammar.txt","r");
	if(g_file == NULL)
	{
		printf("Grammar file not opened successfully\n");
		return 0;
	}
	Grammar* g = extractGrammarFromFile(g_file);
	// printGrammar(g);
	ComputeFirstSets(g);
	ComputeFollowSets(g);
	parsingTable pTable = createParsingTable(g);
	ParseTree ptree = NULL;
	ASTNode* asTree = NULL;
	errorHead* symTableErrorListHead = initializeErrorHead();
	errorHead* typeCheckingErrorsHead = initializeErrorHead();
	errorHeadSyntactic* syntaxErrorsHead = initializeErrorHeadSyntax();
	SymbolTable* symTable = NULL;
	// printGrammar(g);
	// printGrammarRulesReverse(g);
	// printRHSOccurrences(g);
	// printFirstSets(g);
	// printFollowSets(g);
	// printParsingTable(pTable);
	int* numParseTreeNodes = (int*)malloc(sizeof(int));
	int* numASTTreeNodes = (int*)malloc(sizeof(int));
	int parseTreeMem = 0;
	int asTreeMem = 0;
	double compressionPercent = 0;
	while(1)
	{
		printf("\n");
		printf("Choose an option\n");
		printf("0. Exit.\n");
		printf("1. Print the token list generated by lexer.\n");
		printf("2. Parse to verify syntactic correctness of code and print parse tree on console.\n");
		printf("3. Print Abstract Syntax Tree on console.\n"); // specify traversal order
		printf("4. Print amount of memory used by parse tree and abstract syntax tree.\n");
		printf("5. Print symbol table.\n"); // if this option is selected before and after generating code(temp vars will also be present in the symbol table)
		printf("6. Compile to verify the syntactic and semantic correctness.\n");
		printf("7. Produce assembly code\n");

		printf("Option:");
		int c;
		scanf("%d",&c);
		if(c == 0)
		{
			break;
		}
		if(c == 1)
		{
			// removeComments(argv[1]);
			if(ptree == NULL)
			{
				ptree = parseInput(g,argv[1],tokenList,pTable,syntaxErrorsHead);
			}
			printTokens(tokenList); // check this
		}
		else if(c == 2)
		{
			if(ptree == NULL)
			{
				printf("First generate tokens.\n");
			}
			// else if(asTree != NULL)
			// {
			// 	printf("AST is created so parse tree is deallocated(it cannot be printed now)\n");
			// }
			else
			{
				*numParseTreeNodes = 0;
				printParseTree(ptree,numParseTreeNodes); // check this
				parseTreeMem = (*numParseTreeNodes)*sizeof(treeNode);
				if(syntaxErrorsHead->first != NULL)
				{
					printSyntaxErrors(syntaxErrorsHead);
				}
			}
		}
		else if(c == 3)
		{
			if(ptree == NULL)
			{
				printf("First parse input code to verify syntactic correctness of code.\n");
			}
			else if(syntaxErrorsHead->first == NULL)
			{
				if(asTree == NULL)
				{
					asTree = createASTFromParseTree(ptree);
				}
				*numASTTreeNodes = 0;
				printAST(asTree,numASTTreeNodes); // write traversal order, check this
				asTreeMem = (*numASTTreeNodes)*sizeof(ASTNode);
			}
			else
			{
				printf("Cannot print AST because code is not syntactically correct\n");
			}
		}
		else if(c == 4)
		{
			if(ptree == NULL)
			{
				printf("First parse input code to verify syntactic correctness of code.(Option:2)\n");
			}
			else if(asTree == NULL)
			{
				printf("First create and print AST.(Option:3)\n");	
			}
			else
			{
				// printf("%d \n",(int)sizeof(treeNode));
				// printf("%d \n",(int)sizeof(ASTNode));
				compressionPercent = ((double)(parseTreeMem - asTreeMem)/(double)(parseTreeMem))*100;
				printf("Parse Tree number of nodes = %d            Allocated Memory = %d Bytes\n",*numParseTreeNodes,parseTreeMem);
				printf("AST number of nodes = %d                   Allocated Memory = %d Bytes\n",*numASTTreeNodes,asTreeMem);
				printf("Compression percentage = %lf\n",compressionPercent);
			}
		}
		else if(c == 5)
		{
			if(ptree == NULL)
			{
				printf("First parse input code to verify syntactic correctness of code.\n");
			}
			else if(asTree == NULL)
			{
				printf("First create and print AST.\n");	
			}
			else if(syntaxErrorsHead->first == NULL)
			{
				if(symTable == NULL)
				{
					symTable = createSymbolTable(asTree,symTableErrorListHead,typeCheckingErrorsHead);
				}
				printSymbolTable(symTable); // check this
			}
			else
			{
				printf("Code is not syntactically correct\n");
			}
		}
		else if(c == 6)
		{
			if(ptree == NULL)
			{
				printf("First parse input code to verify syntactic correctness of code.\n");
			}
			else if(syntaxErrorsHead->first != NULL)
			{
				printSyntaxErrors(syntaxErrorsHead);
			}
			else if(asTree == NULL)
			{
				printf("First create and print AST.\n");	
			}
			else if(symTable == NULL)
			{
				printf("First create and print symbol table.\n");	
			}
			else if(symTableErrorListHead->first != NULL || typeCheckingErrorsHead->first != NULL)
			{
				printf("Code is syntactically correct.\n");
				printErrors(symTableErrorListHead,typeCheckingErrorsHead);
			}
			else
			{
				printf("Code compiles successfully.\n");
			}
		}
		else if(c == 7)
		{
			if(ptree == NULL)
			{
				printf("First parse input code to verify syntactic correctness of code.\n");
			}
			else if(syntaxErrorsHead->first != NULL)
			{
				printf("Code contains syntax errors.\n");	
			}
			else if(asTree == NULL)
			{
				printf("First create and print AST.\n");	
			}
			else if(symTable == NULL)
			{
				printf("First create and print symbol table.\n");	
			}
			else if(syntaxErrorsHead->first == NULL && symTableErrorListHead->first == NULL && typeCheckingErrorsHead->first == NULL)
			{
				int* tempVarNum = (int*)malloc(sizeof(int));
				int* labelVarNum = (int*)malloc(sizeof(int));
				*tempVarNum = 0;
				*labelVarNum = 0;
				if(asTree->code == NULL)
				{
					createIntermediateCode(asTree,symTable,tempVarNum,labelVarNum);
					appendAllStatements(asTree);
				}
				// printIntCode(asTree->code); // remove this
				FILE* codeFile = fopen(argv[2],"w");
				generateCode((codeNode*)asTree->code,symTable,codeFile);
				fclose(codeFile);
			}
			else
			{
				printf("Code contains semantic errors.\n");
			}
		}
	}
	free(g);
	free(pTable);
	// free(ptree);
	fclose(g_file);
	return 0;
}