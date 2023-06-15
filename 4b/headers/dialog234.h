
#ifndef INC_123TREE_DIALOG234_H
#define INC_123TREE_DIALOG234_H

#include "234.h"

int GetInt(int *a);
int GetInt_u(unsigned int *a);
char *GetStr();
int dialog(const char *msgs[], int N);
int D_Add(Node **paspen);
int D_Find(Node **paspen);
int D_Delete(Node **paspen);
int D_FindPrev(Node **paspen);
int D_Show(Node **paspen);
void delTree(Node **paspen);
void delTree_Timing(Node **paspen);
int D_Bypass(Node **paspen);
int D_Export(Node **paspen);
int D_Timing(Node **paspen);

#endif //INC_123TREE_DIALOG234_H
