#include "grammar_preprocess.c"

void ComputeFirtSets(Grammar* g);
void ComputeFirtSet(SYMBOL_NAME sym,Grammar* g,int* stack_arr,int num_terminals);
void findNullProducingSymbols(Grammar* g);