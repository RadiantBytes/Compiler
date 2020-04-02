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
{letter}({letter}|{digit})*		{return ID;}
{digit}{digit}*								{return INT;}
\+														{return OP;}
\-														{return OP;}
\*														{return OP;}
\/														{return OP;}
\(														{return OP;}
\)														{return OP;}
\=														{return OP;}
\t														{}
\n														{}
\r														{}
[ ] 													{}
[\00]													{}
.                             {return ERROR; }

%%
