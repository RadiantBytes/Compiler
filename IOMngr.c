#include "IOMngr.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1000


// file pointers to the source file (input) the option listing file (output)
FILE * sourcefp;
FILE * listingfp;

char sourcefpName[256];

// a buffer to hold the current line of input
char lineBuffer[MAXLINE];

// variables to keep track of the current char
int currLine;
int currCol;

int setBuffer(int lineNum);
/* Assigns lineBuffer to the line within the provided source file
   at line lineNum
   Return 1 if successful, 0 if EOF
Note: 1-indexed
*/

int openFiles(char * sourceName,  char * listingName) {

	sourcefp = fopen(sourceName, "r");

	strcpy(sourcefpName, sourceName);

	if (sourcefp == NULL) {
		perror("ERROR: The source file could not be opened in IOMnger");
		return 0;
	}



	/* DEBUG: print source file contents
	   printf("\nWriting file contents: ");

	   char c;

	   while(1) {
	   	c = fgetc(sourcefp);
	   	if( feof(sourcefp) ) {
	   		break;
	   	}
	   	printf("%c", c);
	  }
		*/

	// If listingName is NULL, print to stdout
	if (listingName == NULL) {
		printf("#A listing file will not be created!\n\n");

	}
	// If listingName is not NULL, open the listingName file
	else {
		printf("#A listing file will be created under the name %s.\n", listingName);

		listingfp = fopen(listingName, "w");

	}


	// Initiate the	buffer to the first line in the source file
	setBuffer(1);



	return 1;

}

void closeFiles() {

	fclose(sourcefp);
	if (listingfp)
		fclose(listingfp);

	// printf("\n#File pointers have been closed\n");

	strcpy(sourcefpName, "");
}

char getNextSourceChar() {

// Don't skip over the char at line 0 col 0

printf("Checking %c ", lineBuffer[currCol]);

if (lineBuffer[currCol] == EOF) {
	return EOF;
}

// Parse non-null char in the current lineBuffer
else if (lineBuffer[currCol] != '\0') {
		if (lineBuffer[currCol] == '\t') {
			currCol += 4;
		}
		else {
			currCol++;
		}
		if (lineBuffer[currCol] == '\n') {

			writeMessage(lineBuffer);
		}

		return lineBuffer[currCol];
	}


	// If end of the current line was reached, check the next line of the source file
	else {
		if (setBuffer(currLine+1)) {
			if (lineBuffer[currCol] == '\n') {
				writeMessage(lineBuffer);
			}
			currCol++;
			return lineBuffer[currCol];
		}
}

	// If reached, the source file has been fully parsed
	//printf("#End of source file parsed in IOMnger");
	return EOF;

}

void writeIndicator(int column) {

	//printf("\n#Writing indicator on line %d\n", column);
	column++;
	if (listingfp) {
		// Must account for line numbering
		char temp[MAXLINE];

		sprintf(temp, "%d", currLine);

		fprintf(listingfp, "%*c\n", strlen(temp)+2+column, '^');
	}

	else
		printf("%*c\n", column, '^');

}

void writeMessage(char * message) {

	if (listingfp) {
		fprintf(listingfp, "%d. %s", currLine, message);
}
	else
		printf("%s", message);
}

void writeError(char * message) {

	if (listingfp) {
		fprintf(listingfp, "%s\n\n", message);
}
	else
		printf("%s\n\n", message);
}

int getCurrentLineNum() {
	return currLine;
}

int getCurrentColumnNum() {
	return currCol;
}

int setBuffer(int lineNum) {
	if (sourcefp == NULL) {
		perror("ERROR: The source file is closed, the buffer could not be set in IOMnger");
		return 0;
	}

	// Fetch the text line located at lineNum in the source file; Assign it to lineBuffer

	// ISSUE: Why do i have to open it again here?
	sourcefp = fopen(sourcefpName, "r");

	int count = 1;
	while (fgets(lineBuffer, MAXLINE, sourcefp) != NULL) {

		if (count == lineNum) {
			currLine = lineNum;

			// First line WILL NOT be parsed without this clause present
			// I don't know why this works as a fix
			if (lineNum == 1)
				currCol = 0;
			else
				currCol = -1;



			// Check for a blank line here to save runtime
			if (lineBuffer[0] == '\n')
				writeMessage("\n");

			return 1;
		}
		else {
			count++;
		}
	}

	// This should only be reached if requested lineNum > total number of lines in the source file
	//printf("At end of setLineBuffer");
	return 0;

}

//  int main (int argc, char * argv[]) {
//
//
// 	if (!openFiles(argv[1], argv[2])) {
//     printf("Open failed\n");
//     exit(0);
//   }
//
//
//  	// DEBUG: getNextSourceChar
//  	char c;
//
// 	while (c != EOF) {
// 		c = getNextSourceChar();
// 	}
//
//  	closeFiles();
//
//  	return 0;
//  }
