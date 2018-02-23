#include "firstfollow.h"

void ComputeFirstSets(Grammar* g)
{
	int num_symbols = sizeof(grammar_var_mapping)/sizeof(grammar_var_mapping[0]);
	int num_terminals = num_symbols - num_non_terminals;
	findNullProducingSymbols(g);
	int* stack_arr = (int*)malloc(num_non_terminals*sizeof(int));
	int i = 0;
	for(i = 0; i < num_non_terminals; i++)
	{
		stack_arr[i] = 0;
	}
	for(i = 0; i < num_non_terminals; i++)
	{	
		if(g[i]->first_set == NULL)
		{
			stack_arr[i] = 1;
			ComputeFirstSet(g[i]->sym_name,g,stack_arr,num_terminals,NT);
			stack_arr[i] = 0;
		}
	}
}

void ComputeFirstSet(SYMBOL_NAME sym,Grammar* g,int* stack_arr,int num_terminals,SYMBOL_TYPE sym_type)
{
	SYMBOL_NAME eps = EPSILON;
	g[(int)sym]->first_set = createSet(num_terminals);
	rhs_head* rule = g[(int)sym]->first_rule;
	while(rule != NULL)
	{
		rhs_node* r_node = rule->first_sym;
		// int endFirstSet = 0;
		while(r_node != NULL)
		{
			if((r_node->sym)->sym_type == T)
			{
				int bit_number = (int)((r_node->sym)->sym_name) - (int)eps;
				setBit(g[(int)sym]->first_set,bit_number);
				break;
			}
			else
			{
				if(! stack_arr[(int)((r_node->sym)->sym_name)])
				{
					stack_arr[(int)((r_node->sym)->sym_name)] = 1;
					ComputeFirstSet((r_node->sym)->sym_name,g,stack_arr,num_terminals,(r_node->sym)->sym_type);
					Set* unionSet = Union(g[(int)sym]->first_set,g[(int)((r_node->sym)->sym_name)]->first_set);
					Set* tempSet = g[(int)sym]->first_set;
					g[(int)sym]->first_set = unionSet;
					free(tempSet);
					stack_arr[(int)((r_node->sym)->sym_name)] = 0;
				}
				else
				{
					Set* unionSet = Union(g[(int)sym]->first_set,g[(int)((r_node->sym)->sym_name)]->first_set);
					Set* tempSet = g[(int)sym]->first_set;
					free(tempSet);
					tempSet = g[(int)((r_node->sym)->sym_name)]->first_set;
					free(tempSet);
					g[(int)sym]->first_set = unionSet;
					g[(int)((r_node->sym)->sym_name)]->first_set = unionSet;
				}
				if(! g[(int)((r_node->sym)->sym_name)]->derives_epsilon)
				{
					break;
				}
			}
			r_node = r_node->next;
		}
		rule = rule->next_rule;
	}
}

Set* getFirstSet(Grammar* g,rhs_node* r_node,int num_terminals)
{
	SYMBOL_NAME eps = EPSILON;
	Set* first_set = createSet(num_terminals);
	int derives_epsilon = 1;
	while(r_node != NULL)
	{
		grammar_var* sym = r_node->sym;
		if(sym->sym_type == T)
		{
			int bit_number = (int)sym->sym_name - (int)eps;
			setBit(first_set,bit_number);
			derives_epsilon = 0;
			break;
		}
		else
		{
			Set* unionSet = Union(first_set,g[(int)(sym->sym_name)]->first_set);
			free(first_set);
			first_set = unionSet;
			if(! g[(int)(sym->sym_name)]->derives_epsilon)
			{
				derives_epsilon = 0;
				break;
			}
		}
		r_node = r_node->next;
	}
	if(derives_epsilon)
	{
		setBit(first_set,0);
	}
	return first_set;

}

Set* getFollowSet(Grammar* g,SYMBOL_NAME sym_name)
{
	return g[(int)sym_name]->follow_set;
}

void printFirstSets(Grammar* g)
{
	SYMBOL_NAME eps = EPSILON;
	int num_symbols = sizeof(grammar_var_mapping)/sizeof(grammar_var_mapping[0]);
	int num_terminals = num_symbols - num_non_terminals;
	for(int i = 0; i < num_symbols; i++)
	{
		if(grammar_var_mapping[i].sym_name == EPSILON)
		{
			continue;
		}
		else
		{
			rhs_node* n = (rhs_node*)malloc(sizeof(rhs_node));
			n->sym = (grammar_var*)malloc(sizeof(grammar_var));
			n->sym->sym_str = grammar_var_mapping[i].sym_str;
			n->sym->sym_name = grammar_var_mapping[i].sym_name;
			n->sym->sym_type = grammar_var_mapping[i].sym_type;
			// printf("name:%s\n",n->name);
			n->next = NULL;
			n->previous = NULL;
			Set* first_set = getFirstSet(g,n,num_terminals);
			free(n);
			printf("First set of %s: ",grammar_var_mapping[i].sym_str);
			printFFSet(first_set,num_terminals);
		}
	}
}

void printFollowSets(Grammar* g)
{
	SYMBOL_NAME eps = EPSILON;
	int num_symbols = sizeof(grammar_var_mapping)/sizeof(grammar_var_mapping[0]);
	int num_terminals = num_symbols - num_non_terminals;
	printf("num_symbols:%d\n",num_symbols);
	printf("num_terminals:%d\n",num_terminals);
	for(int i = 0; i < num_non_terminals; i++)
	{
		Set* follow_set = getFollowSet(g,grammar_var_mapping[i].sym_name);
		printf("Follow set of %s: ",grammar_var_mapping[i].sym_str);
		printFFSet(follow_set,num_terminals);
	}
}

void printFFSet(Set* set,int num_terminals)
{
	int index = 0;
	SYMBOL_NAME eps = EPSILON;
	for(int i = 0; i < num_terminals; i++)
	{	
		index = i / element_size;
		int bit_num = i % element_size;
		unsigned int j = 1 << bit_num;
		if(set[index].num & j)
		{
			printf("%s, ",grammar_var_mapping[i + (int)eps].sym_str);
		}
	}
	printf("\n");
}

void ComputeFollowSets(Grammar* g)
{
	SYMBOL_NAME eps = EPSILON;
	SYMBOL_NAME dollar = $;
	int num_symbols = sizeof(grammar_var_mapping)/sizeof(grammar_var_mapping[0]);
	int num_terminals = num_symbols - num_non_terminals;
	int changed = 0;
	SYMBOL_NAME lhs_sym;
	g[(int)MAIN_FUNCTION]->follow_set = createSet(num_terminals);
	setBit(g[(int)MAIN_FUNCTION]->follow_set,(int)dollar - (int)eps);
	while(1)
	{
		for(int i = 0; i < num_non_terminals; i++)
		{
			if(g[i]->follow_set == NULL)
			{
				g[i]->follow_set = createSet(num_terminals);
				changed = 1;
			}
			rhs_occurrences* rhs_occur = g[i]->rhs_occur;
			while(rhs_occur != NULL)
			{
				rhs_node* r_node = rhs_occur->ptr_to_rhs_node;
				if(r_node->next == NULL)
				{
					lhs_sym = r_node->lhs_sym; 
					if(g[(int)lhs_sym]->follow_set != NULL)
					{
						Set* unionSet = Union(g[i]->follow_set,g[(int)lhs_sym]->follow_set);
						if(compareSets(unionSet,g[i]->follow_set) == 0)
						{
							Set* temp = g[i]->follow_set;
							g[i]->follow_set = unionSet;
							free(temp);
							changed = 1;
						}
					}
				}
				else
				{
					lhs_sym = r_node->lhs_sym;
					Set* first_set = getFirstSet(g,r_node->next,num_terminals);
					Set* unionSet = Union(g[i]->follow_set,first_set);
					if(first_set[0].num %2 == 1)
					{
						unionSet[0].num--;
					}
					if(compareSets(unionSet,g[i]->follow_set) == 0)
					{
						Set* temp = g[i]->follow_set;
						g[i]->follow_set = unionSet;
						free(temp);
						changed = 1;
					}
					if(first_set[0].num %2 == 1) // epsilon is derived
					{
						if(g[(int)lhs_sym]->follow_set != NULL)
						{
							Set* unionSet = Union(g[i]->follow_set,g[(int)lhs_sym]->follow_set);
							if(compareSets(unionSet,g[i]->follow_set) == 0)
							{
								Set* temp = g[i]->follow_set;
								g[i]->follow_set = unionSet;
								free(temp);
								changed = 1;
							}
						}
					}
				}
				rhs_occur = rhs_occur->next;
			}
		}
		if(changed == 0)
		{
			break;
		}
		changed = 0;
	}
}

void findNullProducingSymbols(Grammar* g)
{
	int flag = 0;
	int i = 0;
	while(1)
	{
		for(i = 0; i < num_non_terminals; i++)
		{
			if(g[i]->derives_epsilon)
			{
				continue;
			}
			else
			{
				SYMBOL_NAME lhs_sym = g[i]->sym_name;
				rhs_head* rule = g[i]->first_rule;
				while(rule != NULL)
				{
					rhs_node* rh = rule->first_sym;
					int derives_epsilon = 1;
					while(rh != NULL)
					{
						if(rh->sym->sym_type == T)
						{
							derives_epsilon = 0;
							break;
						}
						else if(!g[(int)rh->sym->sym_name]->derives_epsilon)
						{
							derives_epsilon = 0;
							break;
						}
						rh = rh->next;
					}
					if(derives_epsilon == 1)
					{
						g[(int)lhs_sym]->derives_epsilon = 1;
						flag = 1;
						break;
					}
					rule = rule->next_rule;
				}
			}
		}
		if(flag == 1)
		{
			flag = 0;
		}
		else
		{
			break;
		}
	}
}

parsingTable createParsingTable(Grammar* g)
{
	SYMBOL_NAME eps = EPSILON;
	int i,j;
	int sz_set;
	int num_symbols = sizeof(grammar_var_mapping)/sizeof(grammar_var_mapping[0]);
	int num_terminals = num_symbols - num_non_terminals;
	parsingTable pTable = (parsingTable)malloc(num_non_terminals*sizeof(rhs_head**));
	for(i = 0; i < num_non_terminals; i++)
	{
		pTable[i] = (rhs_head**)malloc((num_terminals-1)*sizeof(rhs_head*));
		for(j = 0; j < num_terminals-1; j++)
		{
			pTable[i][j] = NULL;
		}
	}
	for(i = 0; i < num_non_terminals; i++)
	{
		rhs_head* rule = g[i]->first_rule;
		while(rule != NULL)
		{
			Set* first_set = getFirstSet(g,rule->first_sym,num_terminals); // A->alpha
			sz_set = sizeof(first_set)/sizeof(first_set[0]);
			int k = 1;
			unsigned int num = first_set[k-1].num;
			if(num % 2 == 1) // epsilon in in first(alpha)
			{
				if(((rule->first_sym)->sym)->sym_name == EPSILON)
				{
					Set* follow_set = getFollowSet(g,g[i]->sym_name);
					int k2 = 1;
					unsigned int num_fol = follow_set[k2-1].num/2;
					for(int m = 1; m < num_terminals; m++)
					{
						if(m >= k2*element_size)
						{
							k2++;
							num_fol = follow_set[k2-1].num;
						}
						if(num_fol % 2 == 1)
						{
							pTable[i][m-1] = rule;
						}
						num_fol = num_fol/2;		
					}
				}
			}
			num = num/2;
			for(j = 1; j < num_terminals; j++)
			{
				if(j >= k*element_size)
				{
					k++;
					num = first_set[k-1].num;
				}
				if(num % 2 == 1)
				{
					pTable[i][j-1] = rule;
				}
				num = num/2;
			}
			rule = rule->next_rule;
		}
	}
	return pTable;
}

void printParsingTable(parsingTable pTable)
{
	SYMBOL_NAME eps = EPSILON;
	int i,j;
	int num_symbols = sizeof(grammar_var_mapping)/sizeof(grammar_var_mapping[0]);
	int num_terminals = num_symbols - num_non_terminals;
	for(i = 0; i < num_non_terminals; i++)
	{
		for(j = 0; j < num_terminals - 1; j++)
		{
			printf("M(%s,%s) : ",grammar_var_mapping[i].sym_str,grammar_var_mapping[j + (int)EPSILON + 1].sym_str);
			if(pTable[i][j] == NULL)
			{
				printf("NULL\n");
				continue;
			}
			printRule(pTable[i][j]);
		}
	}
}

void printRule(rhs_head* rule)
{
	SYMBOL_NAME lhs_sym = (rule->first_sym)->lhs_sym;
	printf("%s => ",grammar_var_mapping[lhs_sym].sym_str);
	rhs_node* r_node = rule->first_sym;
	while(r_node != NULL)
	{
		printf("%s ",(r_node->sym)->sym_str);
		r_node = r_node->next;
	}
	printf("\n");
}