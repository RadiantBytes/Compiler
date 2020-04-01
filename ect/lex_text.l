%%
[A-Z] {printf("letter");}
"+" {printf("PLUS ");}
%%
int main(int argc, char** argv)
{
yylex();
return 0;
}
