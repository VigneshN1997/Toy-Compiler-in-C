#include "grammar_preprocess.c"

void findNullProducingSymbols(Grammar* g);
void ComputeFirstSets(Grammar* g);
void ComputeFirstSet(SYMBOL_NAME sym,Grammar* g,int* stack_arr,int num_terminals,SYMBOL_TYPE sym_type);
void ComputeFollowSets(Grammar* g);

Set* getFirstSet(Grammar* g,rhs_node* r_node,int num_terminals);
Set* getFollowSet(Grammar* g,SYMBOL_NAME sym_name);

void printFirstSets(Grammar* g);
void printFollowSets(Grammar* g);
void printFFSet(Set* set,int num_terminals);

parsingTable createParsingTable(Grammar* g);
void printParsingTable(parsingTable pTable);
void printRule(rhs_head* rule);