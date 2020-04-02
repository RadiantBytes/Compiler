#include <stdio.h>
#include "microtokens.h"
#include "lex_analysis.h"
#include "SymTab.h"
#include "IOMngr.h"

extern int yylex();
extern char *yytext;

int yywrap() {
	return(1);
}

// Print the contents of a SymTab to stdout
void printSymTabContents(SymTab *table) {

	int i = startIterator(table);

	// Cast the current void * attribute to a typeInfo struct
	struct typeInfo *temp;
	temp = getCurrentAttr(table);

	// If printing a SymTab of identifiers
	if (temp->type == ID) {
    printf("\n\n##Printing Identifiers:\n");

		while (i) {

			temp = getCurrentAttr(table);

			printf("\nIdentifier : %s\t", table->current->name);


			//printf("On line(s) : %d ", temp->lineInfo->lineNumber);
			printf("On line(s): ");

			while (temp->lineInfo) {
				printf("%d", temp->lineInfo->lineNumber);

				// Account for trailing comma
				if (temp->lineInfo->next != NULL)
					printf(", ");

				temp->lineInfo = temp->lineInfo->next;
			}


			i = nextEntry(table);
		}

	}
	// else if printing a SymTab of integers
	else if (temp->type == INT) {
    printf("\n\n##Printing Integers:\n");

		while (i) {

			temp = getCurrentAttr(table);

			printf("\nInteger : %s\t\t", table->current->name);

			printf("Occurances : %d", temp->occurances);

			i = nextEntry(table);
		}
	}

	// else if printing a SymTab of operators
	else if (temp->type == OP) {
    printf("\n\n##Printing Operators:\n");

		while (i) {

			temp = getCurrentAttr(table);

			printf("\nOperator : %s\t\t", table->current->name);

			printf("Occurances : %d", temp->occurances);

			i = nextEntry(table);
		}
	}

	printf("\n");
  return;
}

int main (int argc, char * argv[]) {

	int x;
	int token;

	struct typeInfo *attr;
	struct typeInfo *tempAttr;

	struct node *line;
	struct node *tempLine;

	/*
idTable:  Store list of line numbers where each identifier appears
intTable: Store number of occurances of each integer in the source file
opTable:  Store number of occurances of each op in the source file
*/
	SymTab * idTable;
	SymTab * intTable;
	SymTab * opTable;

	idTable = createSymTab(1024);
	intTable = createSymTab(1024);
	opTable = createSymTab(1024);


	if (!openFiles(argv[1], argv[2])) {
		printf("Open failed\n");
		exit(0);
	}

	while (token = yylex()) {
		switch(token) {
			case ID:
			attr = (struct typeInfo *) malloc(sizeof(struct typeInfo));
			tempAttr = (struct typeInfo *) malloc(sizeof(struct typeInfo));
			line = (struct node *) malloc(sizeof(struct node));
			tempLine = (struct node *) malloc(sizeof(struct node));

			// Don't care about counting ID occurances
			attr->type = ID;



			line->lineNumber = getCurrentLineNum();

			//attr->lineInfo->lineNumber = getCurrentLineNum();

			if (findName(idTable, yytext)) {
			// The integer is in the SymTab, update occurances

				tempLine = getCurrentAttr(idTable);

				if (tempLine != NULL) {
					while (tempLine->next != NULL) {
						tempLine = tempLine->next;
					}

					tempLine->next = line;
				} else {
					setCurrentAttr(idTable, line);
				}

			}
			else {

			// The integer is not in SymTab, add it
			enterName(idTable, yytext);
			line->next = NULL;
			attr->lineInfo = line;
			setCurrentAttr(idTable, attr);

			}

			break;

			case INT:
				  attr = (struct typeInfo *) malloc(sizeof(struct typeInfo));
					tempAttr = (struct typeInfo *) malloc(sizeof(struct typeInfo));

				  attr->type = INT;

					attr->lineInfo = NULL;

          if (findName(intTable, yytext)) {
          // The integer is in the SymTab, update occurances
						tempAttr = getCurrentAttr(intTable);
					  attr->occurances = tempAttr->occurances + 1;
            setCurrentAttr(intTable, attr);

          }
          else {
          // The integer is not in SymTab, add it
          enterName(intTable, yytext);
          attr->occurances = 1;
					setCurrentAttr(intTable, attr);

          }

				  break;

			case OP:

			attr = (struct typeInfo *) malloc(sizeof(struct typeInfo));
			tempAttr = (struct typeInfo *) malloc(sizeof(struct typeInfo));

			attr->type = OP;

			attr->lineInfo = NULL;


			if (findName(opTable, yytext)) {
			// The operator is in the SymTab, update occurances
				tempAttr = getCurrentAttr(opTable);
				attr->occurances = tempAttr->occurances + 1;
				setCurrentAttr(opTable, attr);

			}
			else {
			// The operator is not in SymTab, add it
			enterName(opTable, yytext);
			attr->occurances = 1;
			setCurrentAttr(opTable, attr);

			}
        break;

			case ERROR:
				writeIndicator(getCurrentColumnNum());
				writeError("Illegal Character in lex");
				break;

      default:
        break;
			}
	}

	printf("\n____________________________\n");


	if (!isEmpty(idTable))
		printSymTabContents(idTable);
	if (!isEmpty(intTable))
  	printSymTabContents(intTable);
	if (!isEmpty(opTable))
		printSymTabContents(opTable);

	closeFiles();

}
