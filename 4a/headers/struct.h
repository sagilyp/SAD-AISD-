#ifndef ACD
#define ACD

//const char *errmsgs[] = {"Ok", "Don't find key",  "Void tree"};

typedef struct Node{
    char *key;
    char *info;
    struct Node *right;
    struct Node *left;
    struct Node *parent;
}Node;

#endif
