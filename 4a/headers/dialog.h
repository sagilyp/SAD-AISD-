#ifndef SDA
#define SDA

#include "struct.h" 

int dialog(const char *msgs[], int N);
int D_Add(Node **paspen);
int D_Find(Node **paspen);
int D_Delete(Node **paspen);
int D_MinFind(Node **paspen);
int D_Show(Node **paspen);
void delTree(Node **paspen);
int D_Import(Node **paspen);
int D_Bypass(Node **paspen);
int D_Timing(Node **);
int D_Image(Node **);

int GetInt(int *a);
char *GetStr();

#endif
