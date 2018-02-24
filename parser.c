#include "parser.h"

ParseTree createParseTree(node_data* nodeVal)
{
	ParseTree tree = (ParseTree)malloc(sizeof(treeNode));
	tree->nodeVal = nodeVal;
	tree->nextSibling = NULL;
	tree->children = NULL;
	tree->parent = NULL;
	return tree;
}

ParseTree initializeTree()
{
	grammar_var* variable = (grammar_var*)malloc(sizeof(grammar_var));
	variable->sym_str = (char*)malloc(20*sizeof(char));
	strcpy(variable->sym_str,"mainFunction");
	variable->sym_name = MAIN_FUNCTION;
	variable->sym_type = NT;
	node_data* nodeVal = createNodeData(NULL,variable,NULL);
	ParseTree tree = createParseTree(nodeVal);
	return tree;
}

ParseTree parseInput(Grammar* g,char* fileName,HEAD* tokenList,parsingTable pTable)
{
	HASH_TABLE* lookupTable = getLookupTable();
	FILE* fp1 = fopen(fileName,"r");
	int diff_buffer = 0;
	int buffer_read_into = 1;
	size_t nread;
	int* line_number = (int*)malloc(sizeof(int));
	*line_number = 1;
	char *lexemeBegin, *forward, *buffer1, *buffer2;
	buffer1 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	buffer2 = (char*)malloc(BUFFER_SIZE*sizeof(char));
	buffer1[BUFFER_SIZE - 1] = '?'; 					// to indicate end of buffers
	buffer2[BUFFER_SIZE - 1] = '?';
	nread = fread(buffer1,sizeof(char),BUFFER_SIZE - 1,fp1);
	if(nread < BUFFER_SIZE - 1)
	{
		// printf("%d\n",nread);
		buffer1[nread] = '!'; // to indicate end of file
	}
	lexemeBegin = buffer1;								// initialize the lexemeBegin pointer		
	forward = buffer1;
	char* buffer1_end = buffer1 + BUFFER_SIZE - 1;		
	char* buffer2_end = buffer2 + BUFFER_SIZE - 1;
	char* prvs_buff_end,*curr_buff_start;
	curr_buff_start = buffer1;
	prvs_buff_end = buffer1_end;
	int flag = 0;
	Token* lookahead = NULL;

	int error_come = 0;


	SYMBOL_NAME eps = EPSILON;
	// int num_symbols = sizeof(grammar_var_mapping)/sizeof(grammar_var_mapping[0]);
	// int num_terminals = num_symbols - num_non_terminals;
	ParseTree tree = initializeTree();
	STACK* stack = initializeStackforParsing(tree);
	
	lookahead = nextToken(&diff_buffer,&buffer_read_into,&lexemeBegin,&forward,&buffer1,&buffer2,fp1,buffer1_end,buffer2_end,&prvs_buff_end,&curr_buff_start,&flag,line_number,lookupTable);
	while(lookahead != NULL && lookahead->err != NULL)
	{
		error_come = 1;
		printf("%d: Lexical error: %s (%s)\n",lookahead->line_no,error_msg_mapping[(lookahead->err->error_no) - 1].msg,lookahead->lexeme);
		lookahead = nextToken(&diff_buffer,&buffer_read_into,&lexemeBegin,&forward,&buffer1,&buffer2,fp1,buffer1_end,buffer2_end,&prvs_buff_end,&curr_buff_start,&flag,line_number,lookupTable);
	}
	if(lookahead != NULL)
	{
		node_data* data1 = createNodeData(lookahead,NULL,NULL);
		insertAtEnd(tokenList,data1);
	}
	grammar_var* stackTopSym;
	int index;
	while(lookahead != NULL)
	{
		node_data* stackTopData = top(stack);
		stackTopSym = stackTopData->variable;
		if(stackTopSym->sym_type == NT)
		{
			index = (int)lookahead->t_name - ((int)eps + 1);
			if(pTable[(int)stackTopSym->sym_name][index] != NULL)
			{
				rhs_head* rule_rhs = pTable[(int)stackTopSym->sym_name][index];
				ParseTree lhsTreePtr = (ParseTree)stackTopData->ptr_to_ParseTreeNode;
				pop(stack);
				insertChildreninStackandTree(rule_rhs,lhsTreePtr,stack);
				free(stackTopData);
			}	
			else
			{
				// do error recovery
				error_come = 1;
				printf("%d: Syntax error:found token %s with lexeme %s\n",lookahead->line_no,grammar_var_mapping[lookahead->t_name].sym_str,lookahead->lexeme);
				Set* syncSet = getFollowSet(g,stackTopSym->sym_name);
				int found = findTerminalinSyncSet(lookahead->t_name,syncSet);
				while(found == 0 && lookahead != NULL)
				{
					lookahead = nextToken(&diff_buffer,&buffer_read_into,&lexemeBegin,&forward,&buffer1,&buffer2,fp1,buffer1_end,buffer2_end,&prvs_buff_end,&curr_buff_start,&flag,line_number,lookupTable);
					while(lookahead != NULL && lookahead->err != NULL)
					{
						printf("%d: Lexical error: %s (%s)\n",lookahead->line_no,error_msg_mapping[(lookahead->err->error_no) - 1].msg,lookahead->lexeme);
						lookahead = nextToken(&diff_buffer,&buffer_read_into,&lexemeBegin,&forward,&buffer1,&buffer2,fp1,buffer1_end,buffer2_end,&prvs_buff_end,&curr_buff_start,&flag,line_number,lookupTable);
					}
					if(lookahead != NULL)
					{
						node_data* data1 = createNodeData(lookahead,NULL,NULL);
						insertAtEnd(tokenList,data1);
						found = findTerminalinSyncSet(lookahead->t_name,syncSet);
					}
				}
				if(found == 1)
				{
					pop(stack);
					free(stackTopData);
				}
			}
		}
		else
		{
			if(stackTopSym->sym_name == lookahead->t_name)
			{
				ParseTree terminalPtr = (ParseTree)stackTopData->ptr_to_ParseTreeNode;
				copyDetailstoTerminalNode(terminalPtr,lookahead);
				pop(stack);
				free(stackTopData);
				// lookahead = getNextToken(tokenList,&ptr_to_node);
				lookahead = nextToken(&diff_buffer,&buffer_read_into,&lexemeBegin,&forward,&buffer1,&buffer2,fp1,buffer1_end,buffer2_end,&prvs_buff_end,&curr_buff_start,&flag,line_number,lookupTable);
				while(lookahead != NULL && lookahead->err != NULL)
				{
					error_come = 1;
					printf("%d: Lexical error: %s (%s)\n",lookahead->line_no,error_msg_mapping[(lookahead->err->error_no) - 1].msg,lookahead->lexeme);
					lookahead = nextToken(&diff_buffer,&buffer_read_into,&lexemeBegin,&forward,&buffer1,&buffer2,fp1,buffer1_end,buffer2_end,&prvs_buff_end,&curr_buff_start,&flag,line_number,lookupTable);
				}
				if(lookahead != NULL)
				{
					node_data* data1 = createNodeData(lookahead,NULL,NULL);
					insertAtEnd(tokenList,data1);
				}	
			}
			else if(stackTopSym->sym_name == $)
			{
				// do error recovery
				error_come = 1;
				printf("pda stack empty\n");
				return tree;
			}
			else
			{
				error_come = 1;
				printf("%d: Syntax error: The token %s for lexeme %s does not match at line %d. The expected token here is %s.\n",lookahead->line_no,grammar_var_mapping[lookahead->t_name].sym_str,lookahead->lexeme,lookahead->line_no,stackTopSym->sym_str);
				pop(stack);
				free(stackTopData);
				lookahead = nextToken(&diff_buffer,&buffer_read_into,&lexemeBegin,&forward,&buffer1,&buffer2,fp1,buffer1_end,buffer2_end,&prvs_buff_end,&curr_buff_start,&flag,line_number,lookupTable);
				while(lookahead != NULL && lookahead->err != NULL)
				{
					error_come = 1;
					printf("%d: Lexical error: %s (%s)\n",lookahead->line_no,error_msg_mapping[(lookahead->err->error_no) - 1].msg,lookahead->lexeme);
					lookahead = nextToken(&diff_buffer,&buffer_read_into,&lexemeBegin,&forward,&buffer1,&buffer2,fp1,buffer1_end,buffer2_end,&prvs_buff_end,&curr_buff_start,&flag,line_number,lookupTable);
				}
				if(lookahead != NULL)
				{
					node_data* data1 = createNodeData(lookahead,NULL,NULL);
					insertAtEnd(tokenList,data1);
				}
			}
		}
		// printf("%s %d\n",lookahead->lexeme,lookahead->t_name);
	}
	if(lookahead == NULL && top(stack)->variable->sym_name != $)
	{
		printf("error: something left on stack\n");
	}
	else if(lookahead == NULL && top(stack)->variable->sym_name == $ && error_come == 0)
	{
		printf("Successful compilation\n");
	}
	free(buffer1);
	free(buffer2);
	free(lookupTable);
	fclose(fp1);
	return tree;
}

int findTerminalinSyncSet(SYMBOL_NAME sym_name,Set* syncSet)
{
	SYMBOL_NAME eps = EPSILON;
	int i = (int)sym_name - (int)eps;
	int index =  i / element_size;
	int bit_num = i % element_size;
	unsigned int j = 1 << bit_num;
	unsigned int num = syncSet[index].num;
	if(j & num)
	{
		return 1;
	}
	return 0;
}


void insertChildreninStackandTree(rhs_head* rule_rhs, ParseTree lhsTreePtr,STACK* stack)
{
	rhs_node* r_node = rule_rhs->last_sym; // traverse rule from last symbol to first
	treeNode* children = NULL;
	while(r_node != NULL)
	{
		treeNode* t_node = createTreeNode(r_node);
		t_node->parent = lhsTreePtr;
		children = insertChild(t_node,children);
		if(r_node->sym->sym_name == EPSILON)
		{
			// printf("hey\n");
			lhsTreePtr->children = children;
			return;
		}
		node_data* s_node = createStackNode(r_node,t_node);
		push(stack,s_node);
		r_node = r_node->previous;
	}
	lhsTreePtr->children = children;
}

treeNode* createTreeNode(rhs_node* r_node)
{
	grammar_var* variable = (grammar_var*)malloc(sizeof(grammar_var));
	variable->sym_str = r_node->sym->sym_str;
	variable->sym_name = r_node->sym->sym_name;
	variable->sym_type = r_node->sym->sym_type;
	node_data* data = createNodeData(NULL,variable,NULL);
	treeNode* tree_node = createParseTree(data);
	return tree_node;
}

node_data* createStackNode(rhs_node* r_node,treeNode* tree_node)
{
	grammar_var* variable = (grammar_var*)malloc(sizeof(grammar_var));
	variable->sym_str = (char*)malloc(30*sizeof(char));
	strcpy(variable->sym_str,r_node->sym->sym_str);
	variable->sym_name = r_node->sym->sym_name;
	variable->sym_type = r_node->sym->sym_type;
	node_data* data = createNodeData(NULL,variable,tree_node);
	return data;
}

treeNode* insertChild(treeNode* t_node, treeNode* children)
{
	if(children == NULL)
	{
		return t_node;
	}
	t_node->nextSibling = children;
	return t_node;
}


void copyDetailstoTerminalNode(ParseTree terminalPtr, Token* lookahead)
{
	terminalPtr->nodeVal->token = lookahead;
}

void printStack(STACK* stack)
{
	HEAD* top = stack->top;
	ll_node* temp = top->first;
	while(temp != NULL)
	{
		node_data* d = temp->data;
		if(d->variable->sym_str !=NULL)
		{
			ParseTree ptr = (ParseTree)d->ptr_to_ParseTreeNode;
			printf("var:%s  tree:%s\n",d->variable->sym_str,ptr->nodeVal->variable->sym_str);
		}	
		else
		{
			printf("%d\n",d->variable->sym_name);
		}
		temp = temp->next;
	}
}

void printParseTree(ParseTree tree,char* outfile)
{
	FILE* fp;
	fp = fopen(outfile,"w");
	doInOrderTraversal(tree,fp);
	fclose(fp);
}

void doInOrderTraversal(ParseTree tree,FILE* fp)
{
	if(tree->children == NULL)
	{
		Token* tok = tree->nodeVal->token;
		treeNode* parent = tree->parent;
		if(tok == NULL) // means it is EPSILON leaf node
		{
			fprintf(fp, "- - - - %s YES %s\n",tree->parent->nodeVal->variable->sym_str,tree->nodeVal->variable->sym_str);
		}
		else if(tok->t_name == NUM)
		{
			fprintf(fp,"%s %d %s %d %s YES -\n",tok->lexeme,tok->line_no,tree->nodeVal->variable->sym_str,(tok->value).int_value,parent->nodeVal->variable->sym_str);
		}
		else if(tok->t_name == RNUM)
		{
			fprintf(fp,"%s %d %s %lf %s YES -\n",tok->lexeme,tok->line_no,tree->nodeVal->variable->sym_str,(tok->value).real_value,parent->nodeVal->variable->sym_str);
		}
		else
		{
			fprintf(fp,"%s %d %s - %s YES -\n",tok->lexeme,tok->line_no,tree->nodeVal->variable->sym_str,parent->nodeVal->variable->sym_str);
		}
	}
	else
	{
		ParseTree leftMostChild = tree->children;
		doInOrderTraversal(leftMostChild,fp);
		grammar_var* variable = tree->nodeVal->variable;
		treeNode* parent = tree->parent;
		if(parent == NULL)
		{
			fprintf(fp, "- - - - ROOT NO %s\n",variable->sym_str);
		}
		else
		{
			fprintf(fp, "- - - - %s NO %s\n",parent->nodeVal->variable->sym_str,variable->sym_str);	
		}
		ParseTree otherChildren = leftMostChild->nextSibling;
		while(otherChildren != NULL)
		{
			doInOrderTraversal(otherChildren,fp);
			otherChildren = otherChildren->nextSibling;
		}
	}
}

void printTokens(HEAD* tokenList)
{
	ll_node* n = tokenList->first;
	Token* tok = NULL;
	while(n != NULL)
	{
		tok = n->data->token;
		printf("%s %s %d\n",grammar_var_mapping[(int)tok->t_name].sym_str,tok->lexeme,tok->line_no);
		n = n->next;
	}
}