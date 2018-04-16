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

void writeExitCode(FILE* codeFile)
{
	fprintf(codeFile, "MOV EAX,1\n");
	fprintf(codeFile, "MOV EBX,0\n");
	fprintf(codeFile, "INT 80H\n");	
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
	writeExitCode(codeFile);
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
		writeStringinCode(tac->var1,codeFile,resultEntry->idInfoPtr->offset);
	}
	else if(tac->var1->op == MATRIX)
	{
		writeMatrixinCode(tac->var1,codeFile,resultEntry->idInfoPtr->offset);
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
			writeStringIDinCode(tac->var1,codeFile,symEntry1->idInfoPtr->offset,resultEntry->idInfoPtr->offset);
		}
		else if(symEntry1->idInfoPtr->type == MATRIX)
		{
			writeMatrixIDinCode(tac->var1,codeFile,symEntry1->idInfoPtr->offset,resultEntry->idInfoPtr->offset);
		}
	}
}

void createCodeFor_Plus(threeAddrCode* tac,FILE* codeFile)
{
	symbolTableEntry* symEntry1 = (symbolTableEntry*)tac->var1->ptrToSymTableEntry;
	symbolTableEntry* symEntry2 = (symbolTableEntry*)tac->var2->ptrToSymTableEntry;
	symbolTableEntry* resultEntry = (symbolTableEntry*)tac->resVar->ptrToSymTableEntry;

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
		int strSt = resultEntry->idInfoPtr->offset;
		int var1Len = symEntry1->idInfoPtr->widthInfo[0];
		writeStringIDinCode(tac->var1,codeFile,symEntry1->idInfoPtr->offset,strSt);
		writeStringIDinCode(tac->var2,codeFile,symEntry2->idInfoPtr->offset,strSt + var1Len);
	}
	else if(symEntry1->idInfoPtr->type == MATRIX && symEntry2->idInfoPtr->type == MATRIX)
	{
		matrixOperation(tac->var1,tac->var2,symEntry1->idInfoPtr->offset,symEntry2->idInfoPtr->offset,resultEntry->idInfoPtr->offset,codeFile,"ADD");
	}
}

void createCodeFor_Minus(threeAddrCode* tac,FILE* codeFile)
{
	symbolTableEntry* symEntry1 = (symbolTableEntry*)tac->var1->ptrToSymTableEntry;
	symbolTableEntry* symEntry2 = (symbolTableEntry*)tac->var2->ptrToSymTableEntry;
	symbolTableEntry* resultEntry = (symbolTableEntry*)tac->resVar->ptrToSymTableEntry;

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
		matrixOperation(tac->var1,tac->var2,symEntry1->idInfoPtr->offset,symEntry2->idInfoPtr->offset,resultEntry->idInfoPtr->offset,codeFile,"SUB");
	}
}

void createCodeFor_Multiply(threeAddrCode* tac,FILE* codeFile)
{
	symbolTableEntry* symEntry1 = (symbolTableEntry*)tac->var1->ptrToSymTableEntry;
	symbolTableEntry* symEntry2 = (symbolTableEntry*)tac->var2->ptrToSymTableEntry;
	symbolTableEntry* resultEntry = (symbolTableEntry*)tac->resVar->ptrToSymTableEntry;

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

void writeStringinCode(ASTNode* stringNode, FILE* codeFile, int offset)
{
	int strlen = stringNode->widthInfo[0];
	char* string = stringNode->token->lexeme;
	for(int i = 0; i < strlen; i++)
	{
		fprintf(codeFile, "MOV AL,'%c'\n",string[i+1]);
		fprintf(codeFile, "MOV [EBP + %d],AL\n",offset);
		offset++;
	}
}

void writeStringIDinCode(ASTNode* stringNode,FILE* codeFile, int offset_rhs, int offset_lhs)
{
	int strlen = ((symbolTableEntry*)stringNode->ptrToSymTableEntry)->idInfoPtr->widthInfo[0];
	for(int i = 0; i < strlen; i++)
	{
		fprintf(codeFile, "MOV AL,[EBP + %d]\n",offset_rhs);
		fprintf(codeFile, "MOV [EBP + %d],AL\n",offset_lhs);
		offset_rhs++;
		offset_lhs++;
	}
}

void writeMatrixinCode(ASTNode* matrixNode, FILE* codeFile, int offset)
{
	int dim1 = matrixNode->widthInfo[0];
	int dim2 = matrixNode->widthInfo[1];
	int colNum = 0;
	int num = 0;
	ASTNode* row = NULL;
	ASTNode* numNode = NULL;
	while(colNum < dim2)
	{
		row = matrixNode->children;
		while(row != NULL)
		{
			num = 0;
			numNode = row->children;
			while(num != colNum)
			{
				numNode = numNode->nextSibling;
				num++;
			}
			fprintf(codeFile, "MOV AX,%d\n",(numNode->token->value).int_value);
			fprintf(codeFile, "MOV [EBP + %d],AX\n",offset);
			offset += 2;
			row = row->nextSibling;
		}
		colNum++;
	}
}

void writeMatrixIDinCode(ASTNode* matrixNode, FILE* codeFile, int offset_rhs, int offset_lhs)
{
	int dim1 = ((symbolTableEntry*)matrixNode->ptrToSymTableEntry)->idInfoPtr->widthInfo[0];
	int dim2 = ((symbolTableEntry*)matrixNode->ptrToSymTableEntry)->idInfoPtr->widthInfo[1];
	for(int i = 0; i < dim1*dim2; i++)
	{
		fprintf(codeFile, "MOV AX,[EBP + %d]\n",offset_rhs);
		fprintf(codeFile, "MOV [EBP + %d],AX\n",offset_lhs);
		offset_rhs += 2;
		offset_lhs += 2;
	}
}

void matrixOperation(ASTNode* matrixID1, ASTNode* matrixID2, int offsetID1, int offsetID2, int resOffset,FILE* codeFile,char* operation)
{
	int dim1 = ((symbolTableEntry*)matrixID1->ptrToSymTableEntry)->idInfoPtr->widthInfo[0];
	int dim2 = ((symbolTableEntry*)matrixID1->ptrToSymTableEntry)->idInfoPtr->widthInfo[1];
	for(int i = 0; i < dim1*dim2; i++)
	{
		fprintf(codeFile, "MOV AX,[EBP + %d]\n",offsetID1);
		fprintf(codeFile, "MOV BX,[EBP + %d]\n",offsetID2);
		fprintf(codeFile, "%s AX,BX\n",operation);
		fprintf(codeFile, "MOV [EBP + %d],AX\n",resOffset);

		offsetID1 += 2;
		offsetID2 += 2;
		resOffset += 2;
	}
}