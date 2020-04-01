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

Prog        :   StmtSeq           {printf("StmtSeq > "); printSymTab();}
            ;
StmtSeq     :   Stmt StmtSeq      {printf("Stmt StmtSeq > "); }
            |                     {printf("#empty > "); }
            ;
Stmt        :   Id '=' Expr ';'   {printf("Id = Expr ; : $3 is %d", $3); storeVar($1, $3);}
            ;
Expr        :   Expr '+' Term     {printf("Expr + Term > "); $$ = doAdd($1, $3);}
            |   Term              {printf("Term > "); $$ = $1;}
            ;
Term        :   Term '*' Factor   {printf("Term * Factor > "); $$ = doMult($1, $3);}
            |   Factor            {printf("Factor > "); $$ = $1;}
            ;
Factor      :   '-'Factor         {printf("-Factor ; > "); $$ = doNegative($2); printf(" $$ is now %d ", $$);}
            |   '(' Expr ')'      {printf("( Exper ) > "); $$ = $2;}
            |   Id                {printf("Id > "); $$ = getVal($1);}
            |   Int               {printf("Int > "); $$ = atoi(yytext);}
            ;
Id          :   Ident             {printf("Ident > "); $$ = strdup(yytext);}
            ;

%%

int yyerror(char *s) {
  printf("\nIn yyerror: %s\n", s);
  writeIndicator(getCurrentColumnNum());
  writeError("Illegal Character in YACC");
  return 1;
}
