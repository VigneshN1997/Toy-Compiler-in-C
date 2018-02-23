#include "stack.c"

ParseTree createParseTree(node_data* nodeVal);
void addChildren(ParseTree tree,rhs_head* rule_rhs);
ParseTree parseInput(HEAD* tokenList,parsingTable pTable);

void insertChildreninStackandTree(rhs_head* rule_rhs, ParseTree lhsTreePtr,STACK* stack);
treeNode* createTreeNode(rhs_node* r_node);
node_data* createStackNode(rhs_node* r_node,treeNode* tree_node);
treeNode* insertChild(treeNode* t_node, treeNode* children);

void copyDetailstoTerminalNode(ParseTree terminalPtr, Token* lookahead);
void printStack(STACK* stack);


void printParseTree(ParseTree tree,char* outfile);
void doInOrderTraversal(ParseTree tree,FILE* fp);
void printTokens(HEAD* tokenList);