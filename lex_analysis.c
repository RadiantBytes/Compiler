#include<stdio.h>

extern int yyparse();


int main(int argc, char** argv) {

if (yyparse() == 0) {
  printf("Success");
}

}
int yywrap() {return 0;}
