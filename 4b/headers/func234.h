
#ifndef INC_123TREE_FUNC234_H
#define INC_123TREE_FUNC234_H

#include "234.h"
#include <stdio.h>
    Node *findMax(Node **paspen, int *pos);
    void splitting(Node **paspen, int i);
    Node *findNext_n(Node *x, int i, int *pos);
    Node *findPrev_n(Node *x, int i, int *pos);
    Node *find(Node **paspen, unsigned int key, int *pos, int *rc);
    int insert(Node **paspen, unsigned int key, char *info);
    void freeNode(Node **ptr);
    int shiftNode_l(Node **paspen, int pos);
    int shiftNode_r(Node **paspen, int pos);
    int delNode(Node **paspen, unsigned int key);
    int putTree(Node *aspen, int level);
    int decimal_place(unsigned int number);
    int bypass(Node **proot, int ind);
    Node *findMin(Node **paspen, int *pos);
    char *GetStr_F(FILE **fd);
    int load(Node **proot, char *fname);

#endif //INC_123TREE_FUNC234_H
