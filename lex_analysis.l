%{
#include "microtokens.h"
#include "lex_analysis.h"
#include "SymTab.h"
#include "IOMngr.h"

#define YY_INPUT(buf, result, max_size) \
	{ int c = getNextSourceChar(); \
		result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
	}
%}

letter [A-Za-z]
digit [0-9]

%%
{letter}({letter}|{digit})*		{printf("identifier found:%d:%s\n", yyleng, yytext); return ID;}
{digit}{digit}*								{printf("integer found:%d:%s\n", yyleng, yytext); return INT;}
\+														{printf("Plus found:%d:%s\n", yyleng, yytext); return OP;}
\-														{printf("Minus found:%d:%s\n", yyleng, yytext); return OP;}
\*														{printf("Multiply found:%d:%s\n", yyleng, yytext); return OP;}
\/														{printf("Divide found:%d:%s\n", yyleng, yytext); return OP;}
\(														{printf("Open Paren found:%d:%s\n", yyleng, yytext); return OP;}
\)														{printf("Close Paren found:%d:%s\n", yyleng, yytext); return OP;}
\=														{printf("Equal found:%d:%s\n", yyleng, yytext); return OP;}
\t														{printf("Tab found:%d:%s\n", yyleng, yytext);}
\n														{printf("Newline found:%d:%s\n", yyleng, yytext);}
\r														{}
[ ] 													{printf("Space found:%d:%s\n", yyleng, yytext);}
.                             {printf("error"); return ERROR; }

%%
