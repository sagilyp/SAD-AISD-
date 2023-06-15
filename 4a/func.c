#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <graphviz/cgraph.h>

#include "headers/struct.h"

char *GetStr_F(FILE **fd){
	if(feof(*fd))
		return NULL;
    char buf[81] ={0};
    char *res = NULL;
    int len = 0;
    int n;
    do{
        n = fscanf(*fd,"%80[^\n]", buf);
        if (n < 0){
            return NULL;
        }
        else if (n > 0){
            int chunk_len = (int)strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            strncpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else{
            fscanf(*fd,"%*c");
        }
    } while(n > 0);
    if(len > 0){
        res[len] = '\0';
    }
    else{
        res = calloc(1, sizeof(char));
    }
    return res;
}

Node *find(Node **paspen, char *key, int *rc){
    Node *ptr = *paspen;
    if(!ptr){
        *rc = 2;
        return NULL;
    }
    while(ptr){
        if(strcmp(ptr->key, key) == 0){
            *rc = 0;
            return ptr;
        }
        ptr = strcmp(ptr->key, key) > 0 ? ptr->left : ptr->right;
    }
    *rc = 1;
    return NULL;
}

Node *findMax(Node **paspen){
    Node *ptr = *paspen;
    if(!ptr)
        return NULL;
    while(ptr)
        ptr = ptr->right;
    return ptr;
}

Node *findMin(Node **paspen){
    Node *ptr = *paspen;
    if(!ptr)
        return NULL;
    while(ptr->left)
        ptr = ptr->left;
    return ptr;
}
Node *findNext(Node **paspen, char *key){
    int n;
    Node *ptr, *x = find(paspen, key, &n);
    if(n == 0){
        if(x->right != NULL){
            ptr = findMin(&(x->right));
            return ptr;
        }
        ptr = x->parent;
        while(ptr != NULL && ptr->right == x){
            x = ptr;
            ptr = x->parent;
        }
        return ptr;
    }
    return NULL;
}

Node *findNext_n(Node *x){
    Node *ptr;
    if(x != NULL){
        if(x->right != NULL){
            ptr = findMin(&(x->right));
            return ptr;
        }
        ptr = x->parent;
        while(ptr != NULL && ptr->right == x){
            x = ptr;
            ptr = x->parent;
        }
        return ptr;
    }
    return NULL;
}


int insert(Node **paspen, char *key, char *info){
    Node *root = *paspen, *par = NULL, *new = NULL, *ptr;
    new = (Node *)calloc(1, sizeof(Node));
    new->key = key;
    new->info = info;
    //printf("key>> %s  info>> %s\n", new->key, new->info);
//    new->parent = NULL;
     new->left = NULL;
//    new->right = NULL;
    if(!root){
     //   puts("nikak");
        *paspen = new;
    }
    else{
        ptr = root;
        while(ptr){
     //       puts("aboba1");
            par = ptr;
            if(strcmp(ptr->key, key) == 0)
                break;
            if(strcmp(ptr->key, key) > 0)
                ptr = ptr->left;
            else //if(strcmp(ptr->key, key) < 0)
                ptr = ptr->right;
        }
     //   puts("aboba2");
        new->parent = par;
        if(strcmp(par->key, key) > 0){
            par->left = new;
        }
        else if(strcmp(par->key, key) < 0){
            par->right = new;
        }
        else{
            ptr = par->left;
            par->left = new;
            new->left = ptr;
        }
        *paspen = root;
    }
    return 0;
}

int delNode(Node **paspen, char *key){
    Node *root = *paspen, *ptr = root, *x, *y, *p, *par;
    int n;
    if(!ptr) {
        return 2;
    }
    x = find(paspen, key, &n);
   // puts("2");
    if(n == 0){
        if(x->left == NULL || x->right == NULL){
            y = x;
        }
        else{
            y = findNext(paspen, x->key);
        //    puts("1");
        }
       // puts("`rkvy jgznm");
       // printf("\tkey >> %s\n", y->key);

        if(y->left)
            p = y->left;
        else
            p = y->right;
        par = y->parent;

        if(p)
            p->parent = par;

        if(!par)
            root = p;
        else
            if(par->left == y)
                par->left = p;
            else
                par->right = p;

        if(y != x){
       // 	puts("тот случай");
        //	printf("\tkey >> %s\n", y->key);
        	free(x->key);
        	free(x->info);
            x->key = strdup(y->key);
            x->info = strdup(y->info);
        }
        free(y->info);
        free(y->key);
        free(y);
        *paspen = root;
        return 0;
    }
    return 1;
}



int putTree(Node *aspen, int level){
    if(!aspen)
        return 2;
    int i = level;
    putTree(aspen->right, level + 1);

//    if(aspen->parent == NULL){
//        puts("|");
//        puts("|");
//        printf("%s\n", aspen->key);
//        puts("|");
//    }
//    else if((aspen->parent)->right == aspen){
//        while(i-- > 0){
//            printf("\t");
//        }
//        i = level;
//        printf("----");
//        printf("+ %s\n", aspen->key);
//        while(i-- > 0){
//            printf("\t");
//        }
//        printf("|\n");
//    }
//    else{
//        while(i-- > 0){
//            printf("\t");
//        }
//        printf("|\n");
//        i = level;
//        while(i-- > 0){
//            printf("\t");
//        }
//        printf("----");
//        printf("+ %s\n", aspen->key);
//    }

    while(i-- > 0){
        printf("\t");
    }
    printf("----");
    printf("+ %s\n", aspen->key);
    putTree(aspen->left, level + 1);
    return 0;
}

int load(Node **proot, char *ffname){
	char *fname = calloc(strlen(ffname) + 7, sizeof(char));// *pwd = strdup("files/");
	strcat(fname, "files/");
	strcat(fname, ffname);
    FILE *fd = fopen(fname, "r");
    if(!fd)
        return 0;
    char *key, *info;
    //int m;
    while(!feof(fd)){
        key = GetStr_F(&fd);
        info = GetStr_F(&fd);
      //  puts("круто");
       // printf("key >> %s, info >> %s\n", key, info);
        if(key != NULL && info != NULL && strcmp(key,"") != 0 && strcmp(info,"") != 0){
         //   puts("вставка");
            insert(proot, key, info);
        }
        else {
            free(key);
            free(info);
        }
    }
    fclose(fd);
    fd = NULL;
    //free(pwd);
    free(fname);
    return 1;
}

 int bypass(Node **proot, char *key){
    Node *ptr = *proot;
    int rc;
    if(!ptr)
        return 2;
    if(strcmp(key,"") != 0) {
        ptr = find(proot, key, &rc);
    //    puts("aboba");
        if(rc)
            return 1;
        do{
            printf("%s, ", ptr->key);
            ptr = findNext_n(ptr);
        }while(ptr);
        return 0;
    }
    (bypass(&(ptr->left), ""));
    printf("%s, ", ptr->key);
        //ptr = ptr->parent;
        //printf("%s, ", ptr->key);
    (bypass(&(ptr->right),""));
    //printf("%s, ", ptr->key);
    puts("");
    return 0;
}
