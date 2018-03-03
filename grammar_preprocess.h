// ID: 2015A7PS0355P
// Name: Vignesh N
#include "parserDef.h"

Grammar* extractGrammarFromFile(FILE* fp);
Grammar* createGrammarAdjacencyList();

void processRule(char* rule,Grammar* g);
grammar_var convertStrSymtoEnum(char* str);
void storeRuleinGrammar(Grammar* g,SYMBOL_NAME lhs_sym,char* rhs);
void storeRHS(Grammar* g,SYMBOL_NAME lhs_sym,char* rhs_part);
void insertRHSSymbol(Grammar* g,rhs_head* r_head, grammar_var sym,SYMBOL_NAME lhs_sym);

rhs_occurrences* getRHSOccurrences(Grammar* gram,SYMBOL_NAME lhs_sym);

void printRHSOccurrences(Grammar* gram);
void printGrammar(Grammar* gram);
void printGrammarRulesReverse(Grammar* gram);

