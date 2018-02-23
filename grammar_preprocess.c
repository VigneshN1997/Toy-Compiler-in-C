#include "grammar_preprocess.h"

Grammar* extractGrammarFromFile(FILE* fp)
{
	char* rule = NULL;
	size_t len = 0;
	ssize_t read;
	
	Grammar* gram = createGrammarAdjacencyList();
	
	while((read = getline(&rule,&len,fp)) != -1)
	{
		processRule(rule,gram);
		// printf("len:%d\n",read);
	}
	if(rule)
	{
		free(rule);
	}
	return gram;
}

Grammar* createGrammarAdjacencyList()
{
	Grammar* gram = (Grammar*)malloc((num_non_terminals+1)*sizeof(Grammar));
	int i;
	for(i = 0; i < num_non_terminals; i++)
	{
		gram[(int)grammar_var_mapping[i].sym_name] = (Grammar)malloc(sizeof(LHS));
		gram[(int)grammar_var_mapping[i].sym_name]->sym_name = grammar_var_mapping[i].sym_name;
		gram[(int)grammar_var_mapping[i].sym_name]->name = grammar_var_mapping[i].sym_str;
		gram[(int)grammar_var_mapping[i].sym_name]->first_rule = NULL;
		gram[(int)grammar_var_mapping[i].sym_name]->last_rule = NULL;
		gram[(int)grammar_var_mapping[i].sym_name]->rhs_occur = NULL;
		gram[(int)grammar_var_mapping[i].sym_name]->derives_epsilon = 0;
		gram[(int)grammar_var_mapping[i].sym_name]->first_set = NULL;
		gram[(int)grammar_var_mapping[i].sym_name]->follow_set = NULL;
	}
	gram[(int)EPSILON] = (Grammar)malloc(sizeof(LHS));
	gram[(int)EPSILON]->sym_name = EPSILON;
	gram[(int)EPSILON]->name = "EPSILON";
	gram[(int)EPSILON]->first_rule = NULL;
	gram[(int)EPSILON]->last_rule = NULL;
	gram[(int)EPSILON]->rhs_occur = NULL;
	return gram;
}

void processRule(char* rule,Grammar* g)
{
	char delim[5] = "===>";
	char* lhs = strtok(rule,delim);
	char* rhs = strtok(NULL,delim);
	SYMBOL_NAME lhs_sym = convertStrSymtoEnum(lhs).sym_name;
	storeRuleinGrammar(g,lhs_sym,rhs);
	// printf("lhs:%s  rhs:%s\n",lhs,rhs);
}

grammar_var convertStrSymtoEnum(char* str)
{
	int num_symbols = sizeof(grammar_var_mapping)/sizeof(grammar_var_mapping[0]);
	int i;
	for(i = 0; i < num_symbols; i++)
	{
		if(strcmp(grammar_var_mapping[i].sym_str,str) == 0)
		{
			return grammar_var_mapping[i];
		}
	}
}

void storeRuleinGrammar(Grammar* g,SYMBOL_NAME lhs_sym,char* rhs)
{
	// printf("lhs:%d\n",lhs_sym);
	char delim[2] = "|";
	char * rest = rhs;
	char* rhs_part = strtok_r(rest,delim,&rest);
	int i = 1;
	// printf("%d\n",lhs_sym);
	while(rhs_part != NULL)
	{
		// printf("rhs%d:%s\n",i,rhs_part);
		storeRHS(g,lhs_sym,rhs_part);
		rhs_part = strtok_r(rest,delim,&rest);
		i++;
	}
}

void storeRHS(Grammar* g,SYMBOL_NAME lhs_sym,char* rhs_part)
{
	char delim[2] = " ";
	char* rest1 = rhs_part;
	char* rest2;
	char* rhs_symbol = strtok_r(rest1,delim,&rest1);
	int j;
	rhs_head* r1 = (rhs_head*)malloc(sizeof(rhs_head));
	r1->first_sym = NULL;
	r1->last_sym = NULL;
	r1->next_rule = NULL;	
	if(g[(int)lhs_sym]->first_rule == NULL)
	{
		g[(int)lhs_sym]->first_rule	= r1;
		g[(int)lhs_sym]->last_rule = r1;
	}
	else
	{
		(g[(int)lhs_sym]->last_rule)->next_rule = r1;
		g[(int)lhs_sym]->last_rule = r1;	
	}
	while(rhs_symbol != NULL)
	{
		rhs_symbol = strtok_r(rhs_symbol,"\n",&rest2);
		// printf("rhs_sym%d:%s ",j,rhs_symbol);
		grammar_var sym = convertStrSymtoEnum(rhs_symbol);
		insertRHSSymbol(g,r1,sym,lhs_sym);
		rhs_symbol = strtok_r(rest1,delim,&rest1);
		j++;
	}
	// printf("\n");
}

void insertRHSSymbol(Grammar* g,rhs_head* r_head, grammar_var sym,SYMBOL_NAME lhs_sym)
{
	rhs_node* n = (rhs_node*)malloc(sizeof(rhs_node));
	n->lhs_sym = lhs_sym;
	n->sym = (grammar_var*)malloc(sizeof(grammar_var));
	n->sym->sym_str = sym.sym_str;
	n->sym->sym_name = sym.sym_name;
	n->sym->sym_type = sym.sym_type;
	// printf("name:%s\n",n->name);
	n->next = NULL;
	n->previous = NULL;
	if(r_head->first_sym == NULL)
	{
		r_head->first_sym = n;
		r_head->last_sym = n;
	}
	else
	{
		(r_head->last_sym)->next = n;
		n->previous = r_head->last_sym;
		r_head->last_sym = n;
	}
	if(sym.sym_type == NT)
	{
		rhs_occurrences* occur1 = (rhs_occurrences*)malloc(sizeof(rhs_occurrences));
		occur1->ptr_to_rhs_node = n;
		occur1->next = NULL;
		if(g[(int)sym.sym_name]->rhs_occur == NULL)
		{
			g[(int)sym.sym_name]->rhs_occur = occur1;	
		}
		else
		{
			occur1->next = g[(int)sym.sym_name]->rhs_occur;
			g[(int)sym.sym_name]->rhs_occur = occur1;
		}
	}
	else if(sym.sym_name == EPSILON)
	{
		rhs_occurrences* occur1 = (rhs_occurrences*)malloc(sizeof(rhs_occurrences));
		occur1->ptr_to_rhs_node = n;
		occur1->next = NULL;
		g[(int)lhs_sym]->derives_epsilon = 1;
		if(g[(int)EPSILON]->rhs_occur == NULL)
		{
			g[(int)EPSILON]->rhs_occur = occur1;	
		}
		else
		{
			occur1->next = g[(int)EPSILON]->rhs_occur;
			g[(int)EPSILON]->rhs_occur = occur1;
		}
	}
}

rhs_occurrences* getRHSOccurrences(Grammar* gram,SYMBOL_NAME lhs_sym)
{
	return gram[(int)lhs_sym]->rhs_occur;
}

void printGrammar(Grammar* gram)
{
	int i;
	for(i = 0; i < num_non_terminals+1; i++)
	{
		printf("lhs symbol:%s(%d)\t",gram[i]->name,(int)gram[i]->sym_name);
		rhs_head* rule = gram[i]->first_rule;
		while(rule != NULL)
		{
			rhs_node* temp = rule->first_sym;
			while(temp != NULL)
			{
				printf("%s:(%d)(%d)(%d) ",temp->sym->sym_str,(int)temp->sym->sym_name,(int)temp->lhs_sym,(int)temp->sym->sym_type);
				temp = temp->next;
			}
			printf("| ");
			rule = rule->next_rule;
		}
		printf("\n");
	}
}
// (%d)(%d)(%d) ,(int)temp->sym_name,(int)temp->lhs_sym,(int)temp->sym_type

void printRHSOccurrences(Grammar* gram)
{
	for(int i = 0; i < num_non_terminals+1; i++)
	{
		printf("lhs symbol:%s(%d)\t",gram[i]->name,(int)gram[i]->sym_name);
		rhs_occurrences* temp = gram[i]->rhs_occur;
		while(temp != NULL)
		{
			rhs_node* a = temp->ptr_to_rhs_node;
			printf("%s:(%d)(%d)(%d) ",a->sym->sym_str,(int)a->sym->sym_name,(int)a->lhs_sym,(int)a->sym->sym_type);
			temp = temp->next;
		}
		printf("\n");
	}
}

void printGrammarRulesReverse(Grammar* gram)
{
	int i;
	for(i = 0; i < num_non_terminals+1; i++)
	{
		printf("lhs symbol:%s(%d)\t",gram[i]->name,(int)gram[i]->sym_name);
		rhs_head* rule = gram[i]->first_rule;
		while(rule != NULL)
		{
			rhs_node* temp = rule->last_sym;
			while(temp != NULL)
			{
				printf("%s:(%d)(%d)(%d) ",temp->sym->sym_str,(int)temp->sym->sym_name,(int)temp->lhs_sym,(int)temp->sym->sym_type);
				temp = temp->previous;
			}
			printf("| ");
			rule = rule->next_rule;
		}
		printf("\n");
	}
}