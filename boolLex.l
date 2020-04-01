%{
  #include "IOMngr.h"
  #include "y.tab.h"

  #define YY_INPUT(buf,result,max_size)\
  { int c = getNextSourceChar();\
  result = (c == EOF) ? YY_NULL : (buf[0] = c, 1);\
  }
%}

letter[A-Za-z]
digit[0-9]

%%

{letter}({letter}|{digit})*		{return Ident;}
{digit}{digit}*								{return Int;}

[*]                           {return '*';}
[+]                           {return '+';}
[-]                           {return '-';}
[=]                           {return '=';}
[!]                           {return '!';}
[;]                           {return ';';}
[(]                           {return '(';}
[)]                           {return ')';}
[ ]                           {}
[\t\n]                        {}


.                             {
                                writeIndicator(getCurrentColumnNum());
                                writeError("Illegal Character in lex");
                              }

%%

int yywrap() {
  return 1;
}
