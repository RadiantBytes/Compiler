#include <stdio.h>
#include "SymTab.h"
#include "IOMngr.h"

extern int yyparse();

SymTab *table;

int main(int argc, char * argv[]) {

  table = createSymTab(1024);

  if (!openFiles(argv[1], argv[2])) {
    printf("Open failed\n");
    exit(0);
  }

  yyparse();


}
