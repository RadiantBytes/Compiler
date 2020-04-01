#define ID 15
#define INT 16
#define OP 17
#define ERROR 18

struct typeInfo {
	int type;
	struct node *lineInfo;
	int occurances;
};

struct node {
	int lineNumber;
	struct node *next;
};
