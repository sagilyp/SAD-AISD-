#ifndef DAF
#define DAF

#include "struct.h"

Node *find(Node **paspen, char *key, int *rc);
Node *findMax(Node **paspen);
Node *findMin(Node **paspen);
Node *findNext(Node **paspen, char *key);
Node *findNext_n(Node *x);
int insert(Node **paspen, char *key, char *info);
int delNode(Node **paspen, char *key);
int putTree(Node *aspen, int level);
int load(Node **proot, char *fname);
int bypass(Node **proot, char *key);


#endif
