#include "codeGenerator.h"

void initializeFile(FILE* codeFile)
{
	fprintf(codeFile, "section .data\n");
	fprintf(codeFile, "section .bss\n");
	// fprintf(codeFile, "temp1\tRESB\t20\n");
	// fprintf(codeFile, "temp2\tRESB\t20\n");
	// fprintf(codeFile, "tempmat1\tRESW\t100\n");
	// fprintf(codeFile, "tempmat2\tRESW\t100\n");
	fprintf(codeFile, "section .text\n");
	fprintf(codeFile, "global _start\n");
	fprintf(codeFile, "_start:\n");

}

void generateCode(codeNode* codeLines,SymbolTable* symTable,FILE* codeFile)
{
	initializeFile(codeFile);
	codeNode* intermediateCodeLine = codeLines;
	threeAddrCode* tac = NULL;
	while(intermediateCodeLine != NULL)
	{
		tac = intermediateCodeLine->code;
		switch(tac->op)
		{
			case ASSIGN_OP:
				createCodeFor_AssignOp(tac,codeFile);
				break;
			case PLUS_OP:
				createCodeFor_Plus(tac,codeFile);
				break;
			case MINUS_OP:
				createCodeFor_Minus(tac,codeFile);
				break;
			case MUL_OP:
				createCodeFor_Multiply(tac,codeFile);
				break;
			case DIV_OP:
				break;
			case LABEL:
				fprintf(codeFile, "%s:\n",tac->resVar->token->lexeme);
				break;
			case LT_OP:
				createCodeFor_Comparison(tac,codeFile,"JL");
				break;
			case LE_OP:
				createCodeFor_Comparison(tac,codeFile,"JLE");
				break;
			case EQ_OP:
				createCodeFor_Comparison(tac,codeFile,"JE");
				break;
			case GT_OP:
				createCodeFor_Comparison(tac,codeFile,"JG");
				break;
			case GE_OP:
				createCodeFor_Comparison(tac,codeFile,"JGE");
				break;
			case NE_OP:
				createCodeFor_Comparison(tac,codeFile,"JNE");
				break;
			case GOTO:
				fprintf(codeFile, "JMP %s\n",tac->resVar->token->lexeme);
				break;
			case READ_OP:
				break;
			case PRINT_OP:
				break;
			case ADDR_OP:
				fprintf(codeFile, "MOV EBX,EBP\n");
				fprintf(codeFile, "ADD EBX,%d\n",((symbolTableEntry*)(tac->var1->ptrToSymTableEntry))->idInfoPtr->offset);
				fprintf(codeFile, "MOV [EBP + %d],EBX \n",((symbolTableEntry*)(tac->resVar->ptrToSymTableEntry))->idInfoPtr->offset);
				break;
			case VALUE_AT_OP:
				fprintf(codeFile, "MOV EBX,[EBP + %d]\n",((symbolTableEntry*)(tac->var1->ptrToSymTableEntry))->idInfoPtr->offset);
				fprintf(codeFile, "MOV AX,[EBX]\n");
				fprintf(codeFile, "MOV [EBP + %d], AX\n",((symbolTableEntry*)(tac->resVar->ptrToSymTableEntry))->idInfoPtr->offset);
				break;
		}
		intermediateCodeLine = intermediateCodeLine->nextLine;
	}
}

void createCodeFor_Comparison(threeAddrCode* tac,FILE* codeFile, char* jmpVariant)
{
	if(tac->var1->op == NUM)
	{
		fprintf(codeFile, "MOV AX,%d\n",(tac->var1->token->value).int_value);
	}
	else if(tac->var1->op == RNUM)
	{

	}
	else if(tac->var1->op == ID)
	{
		fprintf(codeFile, "MOV AX,[EBP + %d]\n",((symbolTableEntry*)(tac->var1->ptrToSymTableEntry))->idInfoPtr->offset);
	}
	if(tac->var2->op == NUM)
	{
		fprintf(codeFile, "MOV BX,%d\n",(tac->var2->token->value).int_value);
	}
	else if(tac->var2->op == RNUM)
	{

	}
	else if(tac->var2->op == ID)
	{
		fprintf(codeFile, "MOV BX,[EBP + %d]\n",((symbolTableEntry*)(tac->var2->ptrToSymTableEntry))->idInfoPtr->offset);
	}
	fprintf(codeFile, "CMP AX,BX\n");
	fprintf(codeFile, "%s %s\n",jmpVariant,tac->resVar->token->lexeme);
}

void createCodeFor_AssignOp(threeAddrCode* tac,FILE* codeFile)
{
	symbolTableEntry* symEntry1 = NULL;
	symbolTableEntry* resultEntry = (symbolTableEntry*)tac->resVar->ptrToSymTableEntry;
	if(tac->var1->op == NUM)
	{
		fprintf(codeFile, "MOV AX,%d\n",(tac->var1->token->value).int_value);
		fprintf(codeFile, "MOV [EBP + %d],AX\n",resultEntry->idInfoPtr->offset);
	}
	else if(tac->var1->op == RNUM)
	{

	}
	else if(tac->var1->op == STR)
	{

	}
	else if(tac->var1->op == MATRIX)
	{

	}
	else
	{
		symEntry1 = (symbolTableEntry*)tac->var1->ptrToSymTableEntry;
		if(symEntry1->idInfoPtr->type == INT)
		{
			fprintf(codeFile, "MOV AX,[EBP + %d]\n",symEntry1->idInfoPtr->offset);
			fprintf(codeFile, "MOV [EBP + %d],AX\n",resultEntry->idInfoPtr->offset);		
		}
		else if(symEntry1->idInfoPtr->type == REAL)
		{

		}
		else if(symEntry1->idInfoPtr->type == STRING)
		{

		}
		else if(symEntry1->idInfoPtr->type == MATRIX)
		{

		}
	}
}

void createCodeFor_Plus(threeAddrCode* tac,FILE* codeFile)
{
	symbolTableEntry* symEntry1 = NULL;
	symbolTableEntry* symEntry2 = NULL;
	symbolTableEntry* resultEntry = (symbolTableEntry*)tac->resVar->ptrToSymTableEntry;
	if(tac->var1->op == NUM || tac->var2->op == NUM)
	{
		if(tac->var1->op == NUM)
		{
			fprintf(codeFile, "MOV AX,%d\n",(tac->var1->token->value).int_value);
		}
		else
		{
			fprintf(codeFile, "MOV AX,[EBP + %d]\n",((symbolTableEntry*)tac->var1->ptrToSymTableEntry)->idInfoPtr->offset);
		}
		if(tac->var2->op == NUM)
		{
			fprintf(codeFile, "MOV BX,%d\n",(tac->var2->token->value).int_value);
		}
		else
		{
			fprintf(codeFile, "MOV BX,[EBP + %d]\n",((symbolTableEntry*)tac->var2->ptrToSymTableEntry)->idInfoPtr->offset);
		}
		fprintf(codeFile, "ADD AX,BX\n");
		fprintf(codeFile, "MOV [EBP + %d],AX\n",resultEntry->idInfoPtr->offset);
	}
	else if(tac->var1->op == RNUM || tac->var2->op == RNUM)
	{

	}
	else if(tac->var1->op == STR || tac->var2->op == STR)
	{

	}
	else if(tac->var1->op == MATRIX || tac->var2->op == MATRIX)
	{

	}
	else if(tac->var1->ptrToSymTableEntry != NULL && tac->var2->ptrToSymTableEntry != NULL)
	{
		symEntry1 = (symbolTableEntry*)tac->var1->ptrToSymTableEntry;
		symEntry2 = (symbolTableEntry*)tac->var2->ptrToSymTableEntry;
		if(symEntry1->idInfoPtr->type == INT && symEntry2->idInfoPtr->type == INT)
		{
			fprintf(codeFile, "MOV AX,[EBP+%d]\n",symEntry1->idInfoPtr->offset);
			fprintf(codeFile, "MOV BX,[EBP+%d]\n",symEntry2->idInfoPtr->offset);
			fprintf(codeFile, "ADD AX,BX\n");
			fprintf(codeFile, "MOV [EBP+%d],AX\n",resultEntry->idInfoPtr->offset);
		}
		else if(symEntry1->idInfoPtr->type == REAL && symEntry2->idInfoPtr->type == REAL)
		{

		}
		else if(symEntry1->idInfoPtr->type == STRING && symEntry2->idInfoPtr->type == STRING)
		{

		}
		else if(symEntry1->idInfoPtr->type == MATRIX && symEntry2->idInfoPtr->type == MATRIX)
		{

		}
	}
}

void createCodeFor_Minus(threeAddrCode* tac,FILE* codeFile)
{
	symbolTableEntry* symEntry1 = NULL;
	symbolTableEntry* symEntry2 = NULL;
	symbolTableEntry* resultEntry = (symbolTableEntry*)tac->resVar->ptrToSymTableEntry;
	if(tac->var1->op == NUM || tac->var2->op == NUM)
	{
		if(tac->var1->op == NUM)
		{
			fprintf(codeFile, "MOV AX,%d\n",(tac->var1->token->value).int_value);
		}
		else
		{
			fprintf(codeFile, "MOV AX,[EBP + %d]\n",((symbolTableEntry*)tac->var1->ptrToSymTableEntry)->idInfoPtr->offset);
		}
		if(tac->var2->op == NUM)
		{
			fprintf(codeFile, "MOV BX,%d\n",(tac->var2->token->value).int_value);
		}
		else
		{
			fprintf(codeFile, "MOV BX,[EBP + %d]\n",((symbolTableEntry*)tac->var2->ptrToSymTableEntry)->idInfoPtr->offset);
		}
		fprintf(codeFile, "SUB AX,BX\n");
		fprintf(codeFile, "MOV [EBP + %d],AX\n",resultEntry->idInfoPtr->offset);
	}
	else if(tac->var1->op == RNUM || tac->var2->op == RNUM)
	{

	}
	else if(tac->var1->op == MATRIX || tac->var2->op == MATRIX)
	{

	}
	else if(tac->var1->ptrToSymTableEntry != NULL && tac->var2->ptrToSymTableEntry != NULL)
	{
		symEntry1 = (symbolTableEntry*)tac->var1->ptrToSymTableEntry;
		symEntry2 = (symbolTableEntry*)tac->var2->ptrToSymTableEntry;
		if(symEntry1->idInfoPtr->type == INT && symEntry2->idInfoPtr->type == INT)
		{
			fprintf(codeFile, "MOV AX,[EBP+%d]\n",symEntry1->idInfoPtr->offset);
			fprintf(codeFile, "MOV BX,[EBP+%d]\n",symEntry2->idInfoPtr->offset);
			fprintf(codeFile, "SUB AX,BX\n");
			fprintf(codeFile, "MOV [EBP+%d],AX\n",resultEntry->idInfoPtr->offset);
		}
		else if(symEntry1->idInfoPtr->type == REAL && symEntry2->idInfoPtr->type == REAL)
		{

		}
		else if(symEntry1->idInfoPtr->type == MATRIX && symEntry2->idInfoPtr->type == MATRIX)
		{

		}
	}
}


void createCodeFor_Multiply(threeAddrCode* tac,FILE* codeFile)
{
	symbolTableEntry* symEntry1 = NULL;
	symbolTableEntry* symEntry2 = NULL;
	symbolTableEntry* resultEntry = (symbolTableEntry*)tac->resVar->ptrToSymTableEntry;
	if(tac->var1->op == NUM || tac->var2->op == NUM)
	{
		if(tac->var1->op == NUM)
		{
			fprintf(codeFile, "MOV AX,%d\n",(tac->var1->token->value).int_value);
		}
		else
		{
			fprintf(codeFile, "MOV AX,[EBP + %d]\n",((symbolTableEntry*)tac->var1->ptrToSymTableEntry)->idInfoPtr->offset);
		}
		if(tac->var2->op == NUM)
		{
			fprintf(codeFile, "MOV BX,%d\n",(tac->var2->token->value).int_value);
		}
		else
		{
			fprintf(codeFile, "MOV BX,[EBP + %d]\n",((symbolTableEntry*)tac->var2->ptrToSymTableEntry)->idInfoPtr->offset);
		}
		fprintf(codeFile, "MUL BX\n");
		fprintf(codeFile, "MOV [EBP + %d],AX\n",resultEntry->idInfoPtr->offset);
	}
	else if(tac->var1->op == RNUM || tac->var2->op == RNUM)
	{

	}
	else if(tac->var1->ptrToSymTableEntry != NULL && tac->var2->ptrToSymTableEntry != NULL)
	{
		symEntry1 = (symbolTableEntry*)tac->var1->ptrToSymTableEntry;
		symEntry2 = (symbolTableEntry*)tac->var2->ptrToSymTableEntry;
		if(symEntry1->idInfoPtr->type == INT && symEntry2->idInfoPtr->type == INT)
		{
			fprintf(codeFile, "MOV AX,[EBP+%d]\n",symEntry1->idInfoPtr->offset);
			fprintf(codeFile, "MOV BX,[EBP+%d]\n",symEntry2->idInfoPtr->offset);
			fprintf(codeFile, "MUL BX\n");
			fprintf(codeFile, "MOV [EBP+%d],AX\n",resultEntry->idInfoPtr->offset);
		}
		else if(symEntry1->idInfoPtr->type == REAL && symEntry2->idInfoPtr->type == REAL)
		{

		}
	}
}