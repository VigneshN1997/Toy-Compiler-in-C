#include "parser.h"

void ComputeFirtSets(Grammar* g)
{
	int num_symbols = sizeof(grammar_var_mapping)/sizeof(grammar_var_mapping[0]);
	int num_terminals = num_symbols - num_non_terminals;
	findNullProducingSymbols(g);
	for(int i = 0; i < num_non_terminals; i++)
	{
		printf("lhs_sym:%s\tderives_epsilon:%d\n",g[i]->name,g[i]->derives_epsilon);
	}
	// int* stack_arr = (int*)malloc(num_non_terminals*sizeof(int));
	// int i = 0;
	// for(i = 0; i < num_non_terminals; i++)
	// {
	// 	stack_arr[i] = 0;
	// }
	// for(i = 0; i < num_non_terminals; i++)
	// {	
	// 	if(g[i]->first_set == NULL)
	// 	{
	// 		stack_arr[i] = 1;
	// 		ComputeFirtSet(g[i]->sym_name,g,stack_arr);
	// 		stack_arr[i] = 0;
	// 	}
	// }
}
// define num terminals
void ComputeFirtSet(SYMBOL_NAME sym,Grammar* g,int* stack_arr,int num_terminals)
{
	g[(int)sym]->first_set = createSet(num_terminals);
	
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