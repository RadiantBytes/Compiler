#include "SymTab.h"
#include "IOMngr.h"

#define ID 1
#define INT 2
#define ERROR 3

struct typeInfo {
  int type;
  int lineNumber;
};
