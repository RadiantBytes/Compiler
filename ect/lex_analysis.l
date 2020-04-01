%{
#include "lex_analysis.h"

#define YY_INPUT(buf,result,max_size)\
  {int c = getNextSourceChar();\
    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1);\
  }

    SymTab *table;
    int moreEntries;
%}

letter[A-Za-z]
digit[0-9]

%%

{letter}{letter}* {return ID;}
{digit}{digit}*   {return INT;}

[]     {}
\t     {}
\r     {}
\n     {}
\00    {}

.      {return ERROR;}

%%

main(int argc, char *argv[]) {
  int token;
  struct typeInfo *attribute;

  table = createSymTab(19);
  openFiles(argv[1], argv[2]);

  while (token = yylex()) {
    switch (token) {
      case ID:  enterName(table, yytext);
              attribute = (struct typeInfo *) malloc(sizeof(struct typeInfo));
              attribute->type = ID;
              attribute->lineNumber = getCurrentLineNum();
              setCurrentAttr(table, attribute);
              break;
      case INT: enterName(table, yytext);
              attribute = (struct typeInfo *) malloc(sizeof(struct typeInfo));
              attribute->type = INT;
              attribute->lineNumber = getCurrentLineNum();
              setCurrentAttr(table, attribute);
              break;
      case ERROR: writeIndicator(getCurrentColumnNum());
              writeMessage("Illegal Character");
              break;
    }
  }

  moreEntries = startIterator(table);
  while (moreEntries) {
          attribute = getCurrentAttr(table);
          printf("Token:\t%s\tToken  Type:\t%d\tlineNumber:
                                       \t%d\n",getCurrentName(table),
                                       attribute->type, attribute->lineNumber);
          moreEntries = nextEntry(table);
  }
  CloseFiles();
}

yywrap () {
}
