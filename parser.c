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

ParseTree parseInput(HEAD* tokenList,parsingTable pTable)
{
	SYMBOL_NAME eps = EPSILON;
	ParseTree tree = initializeTree();
	STACK* stack = initializeStackforParsing(tree);
	// printStack(stack);
	ll_node** ptr_to_node = &(tokenList->first);
	Token* lookahead = getNextToken(tokenList,ptr_to_node);
	
	grammar_var* stackTopSym;
	int index;
	while(lookahead->t_name != $)
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
				lookahead = getNextToken(tokenList,ptr_to_node);
			}
			else if(stackTopSym->sym_name == $)
			{

			}
		}
		// printf("%s %d\n",lookahead->lexeme,lookahead->t_name);
	}
	return tree;
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
