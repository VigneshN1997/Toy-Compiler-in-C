// ID: 2015A7PS0355P
// Name: Vignesh Nanda Kumar
#include "codeGenerator.h"
// initialization code
void initializeFile(FILE* codeFile, int stackSize)
{
	fprintf(codeFile, "%cinclude 'printing_reading.asm'\n",'%');
	fprintf(codeFile, "section .bss\n");
	fprintf(codeFile, "nline\tRESB\t1\n");
	fprintf(codeFile, "tempIp\tRESB\t20\n");
	fprintf(codeFile, "baseOfStack\tRESB\t%d\n",stackSize);
	fprintf(codeFile, "\n");
	fprintf(codeFile, "section .text\n");
	fprintf(codeFile, "global _start\n");
	fprintf(codeFile, "_start:\n");
	fprintf(codeFile, "MOV EBP,baseOfStack\n");
	fprintf(codeFile, "MOV AL,0AH\n");
	fprintf(codeFile, "MOV EBX,nline\n");
	fprintf(codeFile, "MOV [EBX],AL\n");
}
// exit code
void writeExitCode(FILE* codeFile)
{
	fprintf(codeFile, "MOV EAX,1\n");
	fprintf(codeFile, "MOV EBX,0\n");
	fprintf(codeFile, "INT 80H\n");	
}
// this function will write asm code to file
void generateCode(codeNode* codeLines,SymbolTable* symTable,FILE* codeFile)
{
	initializeFile(codeFile,2*(symTable->currOffset));
	codeNode* intermediateCodeLine = codeLines;
	threeAddrCode* tac = NULL;
	while(intermediateCodeLine != NULL)
	{
		tac = intermediateCodeLine->code;
		switch(tac->op)
		{
			case ASSIGN_OP:
				createCodeFor_AssignOp(tac,codeFile);
				fprintf(codeFile, "\n");
				break;
			case PLUS_OP:
				createCodeFor_Plus(tac,codeFile);
				fprintf(codeFile, "\n");
				break;
			case MINUS_OP:
				createCodeFor_Minus(tac,codeFile);
				fprintf(codeFile, "\n");
				break;
			case MUL_OP:
				createCodeFor_Multiply(tac,codeFile);
				fprintf(codeFile, "\n");
				break;
			case DIV_OP:
				printf("Cannot generate code for division.\n");
				fprintf(codeFile, "\n");
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
				fprintf(codeFile, "\n");
				break;
			case READ_OP:
				createCodeFor_Read(tac,codeFile);
				fprintf(codeFile, "\n");
				break;
			case PRINT_OP:
				createCodeFor_Print(tac,codeFile);
				fprintf(codeFile, "\n");
				break;
			case ADDR_OP:
				fprintf(codeFile, "MOV EBX,EBP\n");
				fprintf(codeFile, "ADD EBX,%d\n",((symbolTableEntry*)(tac->var1->ptrToSymTableEntry))->idInfoPtr->offset);
				fprintf(codeFile, "MOV [EBP + %d],EBX \n",((symbolTableEntry*)(tac->resVar->ptrToSymTableEntry))->idInfoPtr->offset);
				fprintf(codeFile, "\n");
				break;
			case VALUE_AT_OP:
				fprintf(codeFile, "MOV EBX,[EBP + %d]\n",((symbolTableEntry*)(tac->var1->ptrToSymTableEntry))->idInfoPtr->offset);
				fprintf(codeFile, "MOV AX,[EBX]\n");
				fprintf(codeFile, "MOV [EBP + %d], AX\n",((symbolTableEntry*)(tac->resVar->ptrToSymTableEntry))->idInfoPtr->offset);
				fprintf(codeFile, "\n");
				break;
		}
		intermediateCodeLine = intermediateCodeLine->nextLine;
	}
	writeExitCode(codeFile);
}
// for reading input
void createCodeFor_Read(threeAddrCode* tac, FILE* codeFile)
{
	symbolTableEntry* symEntry = ((symbolTableEntry*)tac->var1->ptrToSymTableEntry);
	if(symEntry->idInfoPtr->type == INT)
	{
		fprintf(codeFile, "MOV EDX,20\n");
		fprintf(codeFile, "MOV ECX,tempIp\n");
		fprintf(codeFile, "MOV EBX,0\n");
		fprintf(codeFile, "MOV EAX,3\n");
		fprintf(codeFile, "INT 80H\n");

		fprintf(codeFile, "MOV EAX,tempIp\n");
		fprintf(codeFile, "CALL atoi\n");
		fprintf(codeFile, "MOV [EBP + %d],AX\n",symEntry->idInfoPtr->offset);
	}
	else if(symEntry->idInfoPtr->type == REAL)
	{
		printf("Cannot read real numbers\n");
	}
}
// for printing variable values to console
void createCodeFor_Print(threeAddrCode* tac,FILE* codeFile)
{
	symbolTableEntry* symEntry = ((symbolTableEntry*)tac->var1->ptrToSymTableEntry);
	if(symEntry->idInfoPtr->type == INT)
	{
		printInteger(symEntry->idInfoPtr->offset,codeFile);
		// fprintf(codeFile, "MOV BX,[EBP + %d]\n",symEntry->idInfoPtr->offset);
	}
	else if(symEntry->idInfoPtr->type == REAL)
	{
		printf("Cannot print real numbers\n");
	}
	else if(symEntry->idInfoPtr->type == STRING)
	{
		fprintf(codeFile, "MOV EDX,%d\n",symEntry->idInfoPtr->widthInfo[0]);
		fprintf(codeFile, "MOV ECX,EBP\n");
		fprintf(codeFile, "ADD ECX,%d\n",symEntry->idInfoPtr->offset);
		fprintf(codeFile, "MOV EBX,1\n");
		fprintf(codeFile, "MOV EAX,4\n");
		fprintf(codeFile, "INT 80H\n");

		fprintf(codeFile, "MOV EDX,1\n");
		fprintf(codeFile, "MOV ECX,nline\n");
		fprintf(codeFile, "MOV EBX,1\n");
		fprintf(codeFile, "MOV EAX,4\n");
		fprintf(codeFile, "INT 80H\n");
	}
	// matrix is printed in column major form
	else if(symEntry->idInfoPtr->type == MATRIX)
	{
		int dim1 = symEntry->idInfoPtr->widthInfo[0];
		int dim2 = symEntry->idInfoPtr->widthInfo[1];
		int offset = symEntry->idInfoPtr->offset;
		for(int i = 0; i < dim1*dim2; i++)
		{
			printInteger(offset,codeFile);
			offset += 2;
		}
	}
}
// for printing integer
void printInteger(int offset,FILE* codeFile)
{
	fprintf(codeFile, "XOR EAX,EAX\n");
	fprintf(codeFile, "MOV AX,[EBP + %d]\n",offset);
	fprintf(codeFile, "CALL iprintLF\n");
}
// for if else boolean expressions
void createCodeFor_Comparison(threeAddrCode* tac,FILE* codeFile, char* jmpVariant)
{
	if(tac->var1->op == NUM)
	{
		fprintf(codeFile, "MOV AX,%d\n",(tac->var1->token->value).int_value);
	}
	else if(tac->var1->op == RNUM)
	{
		printf("Cannot compare real numbers.\n");
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
		printf("Cannot compare real numbers.\n");
	}
	else if(tac->var2->op == ID)
	{
		fprintf(codeFile, "MOV BX,[EBP + %d]\n",((symbolTableEntry*)(tac->var2->ptrToSymTableEntry))->idInfoPtr->offset);
	}
	fprintf(codeFile, "CMP AX,BX\n");
	fprintf(codeFile, "%s %s\n",jmpVariant,tac->resVar->token->lexeme);
}
// for assignment operation
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
		printf("Cannot store real numbers.\n");
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
			printf("Cannot store real numbers.\n");
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
// for addition operation
void createCodeFor_Plus(threeAddrCode* tac,FILE* codeFile)
{
	symbolTableEntry* symEntry1 = (symbolTableEntry*)tac->var1->ptrToSymTableEntry;
	symbolTableEntry* symEntry2 = (symbolTableEntry*)tac->var2->ptrToSymTableEntry;
	symbolTableEntry* resultEntry = (symbolTableEntry*)tac->resVar->ptrToSymTableEntry;
	if(symEntry1->idInfoPtr->type == REAL && tac->var2->op == NUM) // matrix address computation
	{
		fprintf(codeFile, "MOV EAX,[EBP+%d]\n",symEntry1->idInfoPtr->offset);
		fprintf(codeFile, "MOV EBX,%d\n",(tac->var2->token->value).int_value);
		fprintf(codeFile, "ADD EAX,EBX\n");
		fprintf(codeFile, "MOV [EBP+%d],EAX\n",resultEntry->idInfoPtr->offset);	
	}
	else if(symEntry1->idInfoPtr->type == INT && symEntry2->idInfoPtr->type == INT)
	{
		fprintf(codeFile, "MOV AX,[EBP+%d]\n",symEntry1->idInfoPtr->offset);
		fprintf(codeFile, "MOV BX,[EBP+%d]\n",symEntry2->idInfoPtr->offset);
		fprintf(codeFile, "ADD AX,BX\n");
		fprintf(codeFile, "MOV [EBP+%d],AX\n",resultEntry->idInfoPtr->offset);
	}
	else if(symEntry1->idInfoPtr->type == REAL && symEntry2->idInfoPtr->type == REAL)
	{
		printf("Cannot add real numbers.\n");
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
// for subtraction operation
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
		printf("Cannot subtract real numbers.\n");
	}
	else if(symEntry1->idInfoPtr->type == MATRIX && symEntry2->idInfoPtr->type == MATRIX)
	{
		matrixOperation(tac->var1,tac->var2,symEntry1->idInfoPtr->offset,symEntry2->idInfoPtr->offset,resultEntry->idInfoPtr->offset,codeFile,"SUB");
	}
}
// for multiplication operation
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
		printf("Cannot multiply real numbers.\n");
	}
}
// for storing string in memory
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
// for copying string from one memory location to another
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
// for storing matrix in memory in column major format
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
// for copying matrix from one memory location to another
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
// for performing matrix addition/ subtraction
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