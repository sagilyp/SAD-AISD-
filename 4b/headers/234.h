//
// Created by Gareev Rustam on 18.05.2023.
//

#ifndef INC_123TREE_234_H
#define INC_123TREE_234_H
#define M 2
    typedef struct Item{
        unsigned int key;
        char *info;
    }Item;

    typedef struct Node{
        int numItems;
        Item arrayItems[2*M-1];
        struct Node *child[2*M]; //*right, *mid_right, *mid_left, *left;
        struct Node *parent;
    }Node;
#endif //INC_123TREE_234_H
