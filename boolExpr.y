%{

#include "semantics.h"
#include "IOMngr.h"
#include <string.h>

extern int yylex();     /* the next token function */
extern char *yytext;    /* the matched token text */
extern int yyleng;
extern int yyerror(char *);


extern SymTab *table;
extern SymEntry *entry;

%}

%union {
    int integer;
    char * string;
}

%start Prog


%type <string> Prog
%type <string> StmtSeq
%type <string> Stmt
%type <integer> Expr
%type <integer> Term
%type <integer> Factor
%type <string> Id

%token Ident
%token Int

%%

Prog        :   StmtSeq           {printSymTab();}
            ;
StmtSeq     :   Stmt StmtSeq      {}
            |                     {}
            ;
Stmt        :   Id '=' Expr ';'   {storeVar($1, $3);}
            ;
Expr        :   Expr '+' Term     {$$ = doAdd($1, $3);}
            |   Term              {$$ = $1;}
            ;
Term        :   Term '*' Factor   {$$ = doMult($1, $3);}
            |   Factor            {$$ = $1;}
            ;
Factor      :   '-'Factor         {$$ = doNegative($2);}
            |   '(' Expr ')'      {$$ = $2;}
            |   Id                {$$ = getVal($1);}
            |   Int               {$$ = atoi(yytext);}
            ;
Id          :   Ident             {$$ = strdup(yytext);}
            ;

%%

int yyerror(char *s) {
  writeIndicator(getCurrentColumnNum());
  writeError("Illegal Character in YACC");
  return 1;
}
