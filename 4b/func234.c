
#include "headers/func234.h"
#include "headers/234.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Node *findMax(Node **paspen, int *pos){
    Node *ptr = *paspen, *par;
    if(!ptr)
        return NULL;
    while(ptr){
        par = ptr;
        ptr = ptr->child[ptr->numItems];
    }
    *pos = par->numItems-1;
    return par;
}

Node *findMin(Node **paspen, int *pos){
    Node *ptr = *paspen, *par = NULL;
    if(!ptr)
        return NULL;
    while(ptr){
        par = ptr;
        ptr = ptr->child[0];
    }
    *pos = 0;
    return par;
}

int shiftNode_r(Node **paspen, int pos);

void splitting(Node **paspen, int i){
    if(!(*paspen))
        exit(2);
    Node *new = calloc(1, sizeof(Node));
    Node *branch = (*paspen)->child[i];
	int m = 0, n = branch->numItems;
	shiftNode_r(paspen, i);
    //(*paspen)->child[i] = branch;
    new->numItems = 1;
    for(int j = n/2 + 1; j < n; ++j){
        new->arrayItems[0] = branch->arrayItems[j];
    }    
    for(int j = n/2 + 1; j <= n; ++j){
    	new->child[m] = branch->child[j];
    	if(branch->child[j])
    		branch->child[j]->parent = new;
    	++m;
    }
	(*paspen)->child[i+1] = new;
	new->parent = (*paspen);
	(*paspen)->arrayItems[i] = branch->arrayItems[n/2];
	branch->numItems -= 2;
	(*paspen)->numItems++;
}

Node *findNext_n(Node *x, int i, int *pos){
    int j = 0;
    if(x != NULL){

        if(x->child[i+1] != NULL){
            x = x->child[i+1];
            while(x->child[0] != NULL)
                x = x->child[0];
            *pos = 0;
            return x;
        }
        else if(i < x->numItems-1) {
            *pos = ++i;
            return x;
        }
        else if(i == x->numItems-1){
            unsigned int key = x->arrayItems[i].key;
            do {
                x = x->parent;
            }while(x != NULL && key > x->arrayItems[x->numItems-1].key);
            
            while(x != NULL && x->arrayItems[j].key < key)
                ++j;
            *pos = j;
            return x;

        }
//        ptr = x->parent;
//        while(ptr != NULL && ptr->child[ptr->numItems-1] == x){
//            //x = ptr;
//            ptr = ptr->parent;
//        }
//        while(j < ptr->numItems-1 && x->arrayItems[i].key > ptr->arrayItems[j].key)
//            j++;
//        return & ptr->arrayItems[j];
    }
    return NULL;
}

Node *findPrev_n(Node *x, int i, int *pos){
    int j = 0;
    if(x != NULL){
        if(x->child[i] != NULL){
            x = x->child[i];
            while(x->child[0] != NULL)
                x = x->child[x->numItems];
            *pos = x->numItems-1;
            return x;
        }
        else if(i > 0){
            *pos = --i;
            return x;
        }
        else if(i == 0){
            unsigned int key = x->arrayItems[i].key;
            do
                x = x->parent;
            while(x != NULL && key < x->arrayItems[0].key);
            while(j < x->numItems && x->arrayItems[j].key < key)
                ++j;
            *pos = j-1;
            return x;
        }
    }
    return NULL;
}

Node *find(Node **paspen, unsigned int key, int *pos, int *rc){
    Node *ptr = *paspen, *x;
    int i = 0;
    if(!ptr){
        *rc = 3;
        return NULL;
    }
    while(ptr != NULL){
        x = ptr;
        //printf("%d\n", i);
        if(ptr->arrayItems[i].key == key){
            *rc = 0;
            *pos = i;
            return ptr;
        }

        if(ptr->arrayItems[i].key < key){
        	++i;
        	if(i < ptr->numItems)
        	 	ptr = ptr;
        	else{
        		x = ptr;
        		ptr = ptr->child[i];
        		//ptr->parent = x;
        	}
        }
       	else{ 
       		ptr = ptr->child[i];
       	}	
        if(ptr != NULL && ptr->parent == x)
            i = 0;
    }
    *rc = 1;
    return NULL;
}


// вставка нового элемента в дерево
// 0 - ок, 2 - дубликаты
int insert(Node **paspen, unsigned int key, char *info){

    if(!(*paspen)){ // если создаём дерево
    //	puts("создаём дерево");
        Node *new = (Node *)calloc(1, sizeof(Node));
        new->numItems = 1;
        new->arrayItems[0].key = key;
        new->arrayItems[0].info = info;
        new->parent = NULL;
        *paspen = new;
        return 0;
    }
    if((*paspen)->parent == NULL && (*paspen)->numItems == 2*M-1){ // если заполнен корень, то разбиваем его
        Node *root = (Node *)calloc(1, sizeof(Node));
      //  puts("      разбиваем корень");
        root->child[0] = *paspen;
        (*paspen)->parent = root;
        splitting(&root, 0);
        root->child[0] = *paspen;
        *paspen = root;
    }
    Node *ptr = *paspen, *x;
    int i = 0;
    while(ptr->child[0] != NULL){ // поиск целевого узла
	//	puts("поиск целевого узла");
    	for(i = 0; i < ptr->numItems; ++i){
    		if(ptr->arrayItems[i].key == key)
    			return 2;
    		if(ptr->arrayItems[i].key > key){
    			break;
    		}
    	}
    	x = ptr->child[i];
    	if(x->numItems == 2*M - 1){
    		splitting(&ptr, i);
    //		puts("split");
    		//if(key == ptr->child[i]->arrayItems[ptr->child[i]->numItems-1].key) return 2;
    		if(key > ptr->arrayItems[i].key)
    			x = ptr->child[i+1];
    	}
    	ptr = x;

    }
    
    for(i = 0; i < ptr->numItems; ++i){
    //	puts("Куда вставить?");
    	if(ptr->arrayItems[i].key == key)
    		return 2;
    	if(ptr->arrayItems[i].key > key)
    		break;	
    }
   // printf("в %d конечно\n", i);
    shiftNode_r(&ptr, i);
    ptr->arrayItems[i].key = key;
    ptr->arrayItems[i].info = info;
    ptr->numItems++;
    return 0;
}

void freeNode(Node **ptr){
    for(int i = 0; i < (*ptr)->numItems; ++i) {
        if((*ptr)->arrayItems[i].info != NULL) {
        	free((*ptr)->arrayItems[i].info);
        }
    }
    free(*ptr);
}

int shiftNode_l(Node **paspen, int pos){
    int i;
    if(pos >= 2*M-1 || pos < 0){
        return -1;
    }
    for(i = pos; i < (*paspen)->numItems - 1; ++i){
        (*paspen)->arrayItems[i] = (*paspen)->arrayItems[i+1];
    }
    //if((*paspen)->arrayItems[(*paspen)->numItems-1].info)   free((*paspen)->arrayItems[(*paspen)->numItems-1].info);
    for(i = pos; i < (*paspen)->numItems; ++i){
        (*paspen)->child[i] = (*paspen)->child[i+1];
    }
    //(*paspen)->numItems--;
    return 0;
}

int shiftNode_r(Node **paspen, int pos){
    int i;
    if(pos >= 2*M-2 || pos < 0){
        return -1;
    }
    for(i = (*paspen)->numItems; i > pos; i--){
        (*paspen)->arrayItems[i] = (*paspen)->arrayItems[i-1];
    }
    for(i = (*paspen)->numItems+1; i > pos; i--){
        (*paspen)->child[i] = (*paspen)->child[i-1];
    }
    //(*paspen)->numItems++;
    return 0;
}

// удаление элемента из дерева по ключу
// 0 - ок, 1 - ключ не найден, 3 - пустое дерево
int delNode(Node **paspen, unsigned int key){
    Node *ptr = *paspen, *y, *z;
    int i, flag;
    if(!ptr)
        return 3;
    if(ptr->child[0] != NULL && ptr->numItems == 1 && ptr->child[0]->numItems == M-1 && ptr->child[1]->numItems == M-1){ // обработка корня
	//	puts("обработка корня");
        Node *y = ptr->child[0], *z = ptr->child[1];
        y->arrayItems[1] = ptr->arrayItems[0];
        y->arrayItems[2] = z->arrayItems[0];
        y->numItems = 2*M - 1;
        y->child[2*M-2] = z->child[0];
        y->child[2*M-1] = z->child[1];
        y->parent = NULL;
        if(z->child[0]){
        	z->child[0]->parent = y;
        	z->child[1]->parent = y;
		}
        free(ptr);
        free(z);
        *paspen = y;
    }
    ptr = *paspen;
    while(ptr->child[0]){ // пока ptr не лист
        flag = 0;
        i = 0;
        while(i < ptr->numItems && ptr->arrayItems[i].key <= key){
            if(ptr->arrayItems[i].key == key) {
                flag = 1;
                break;
            }
            if(ptr->arrayItems[i].key > key){
            	break;
            }
            ++i;
        }
      //  printf("ключ %u в %d поддереве\n", key, i);
        if(flag == 0) { // ситуация 1 - ключ не найден в узле ptr
		//	puts("ситуация 1 - ключ не найден в узле ptr");
            Node *z_r = NULL, *z_l = NULL;
            y = ptr->child[i];
            if(i < ptr->numItems) {
                z_r = ptr->child[i + 1];
            }
            if(i > 0) {
                z_l = ptr->child[i - 1];
            }
            if(y->numItems == M-1 && ((z_r != NULL && z_r->numItems >= M) || (z_l != NULL && z_l->numItems >= M))){
		//		puts("1.1");
                if(z_r != NULL && z_r->numItems >= M){
                    y->arrayItems[y->numItems] = ptr->arrayItems[i];
                    y->numItems++;
                    ptr->arrayItems[i] = z_r->arrayItems[0];
                    y->child[y->numItems] = z_r->child[0];
                    shiftNode_l(&z_r, 0);
                    z_r->numItems--;
                }
                else{
                    shiftNode_r(&y, 0);
                    y->arrayItems[0] = ptr->arrayItems[i-1];
                    ptr->arrayItems[i-1] = z_l->arrayItems[z_l->numItems-1];
                    y->child[0] = z_l->child[z_l->numItems];
                    z_l->numItems--;
                    y->numItems++;
                }
            }
            else if(y->numItems == M-1 && ((z_r != NULL && z_r->numItems == M-1) || (z_l != NULL && z_l->numItems == M-1))){
		//		puts("ААА это возможно!?");
                if(z_r != NULL){
          //      	puts("право");
                	y->arrayItems[1] = ptr->arrayItems[i];
                	y->arrayItems[2] = z_r->arrayItems[0];
                	shiftNode_l(&ptr, i);
                	ptr->numItems--;
                	y->numItems += 2;
                	y->child[2] = z_r->child[0];
                	y->child[3] = z_r->child[1];
                	if(z_r->child[0]){
                		z_r->child[0]->parent = y;
                		z_r->child[1]->parent = y;
                	}
                	ptr->child[i] = y;
                	free(z_r);
                
                }
                else{
            //    	puts("лево");
                	shiftNode_r(&y, 0);
                	y->numItems++;
                	y->arrayItems[0] = ptr->arrayItems[i-1];
                	shiftNode_r(&y, 0);
                	y->numItems++;
                	y->arrayItems[0] = z_l->arrayItems[0];
                	y->child[0] = z_l->child[0];
                	y->child[1] = z_l->child[1];
                	if(z_l->child[0]){
                		z_l->child[0]->parent = y;
                		z_l->child[1]->parent = y;
                	}
                	shiftNode_l(&ptr, i-1);
                	ptr->numItems--;
                	free(z_l);
                }	
                if(ptr->numItems == 0){
                	if(ptr->parent){
                		int l = 0;
                		while(ptr->parent->child[l] != ptr)
                			++l;
                		ptr->parent->child[l] = y;
                		y->parent = ptr->parent;
                		free(ptr);
                	}
                }
            }
            ptr = y;
        }
        else if(flag == 1){ // ситуация 2 - ключ k в узле ptr
		//	puts("ситуация 2 - ключ k в узле ptr");

            int pos;
            y = ptr->child[i];
            z = ptr->child[i+1];
            if(y->numItems >= M){
          //  	puts("2.1");
                Node *prev_y = findPrev_n(ptr, i, &pos);
                ptr->arrayItems[i].key = prev_y->arrayItems[pos].key;
                free(ptr->arrayItems[i].info);
                ptr->arrayItems[i].info = strdup(prev_y->arrayItems[pos].info);
                ptr = y;
                key = prev_y->arrayItems[pos].key;
            }
            else if(z->numItems >= M){
            //	puts("2.2");
                Node *prev_y = findNext_n(ptr, i, &pos);
                ptr->arrayItems[i].key = prev_y->arrayItems[pos].key;
                free(ptr->arrayItems[i].info);
                ptr->arrayItems[i].info = strdup(prev_y->arrayItems[pos].info);
                ptr = z;
                key = prev_y->arrayItems[pos].key;
            }
            else if(y->numItems == M-1 && z->numItems == M-1){
			//	puts("2.3");
                y->arrayItems[y->numItems] = ptr->arrayItems[i];
                shiftNode_l(&ptr, i);
                ptr->child[i] = y;
                ptr->numItems--;
                y->numItems++;
                y->arrayItems[y->numItems] = z->arrayItems[0];
                y->numItems++;
                y->child[y->numItems-1] = z->child[0];
                y->child[y->numItems] = z->child[1];
                if(z->child[0]){
                	z->child[0]->parent = y;
                	z->child[1]->parent = y;
				}
                if(z) free(z);
                ptr = y;
            }
        }
    }
    if(!ptr->child[0]){
    	for(i = 0; i < ptr->numItems; ++i){
    		if(ptr->arrayItems[i].key == key){
    		//	printf("info %s", ptr->arrayItems[i].info);
				free(ptr->arrayItems[i].info);
    			shiftNode_l(&ptr, i);
    			ptr->numItems--;
    			return 0;
    		}
    	}		
    }		
    return 1;
}

int decimal_place(unsigned int number) {
    int res = 0;
    while(number > 0) {
        ++res;
        number = number/10;
    }
    return res;
}

/*int putTree(Node *aspen, int level){
    if(!aspen)
        return 3;
    int num = aspen->numItems, i = level, j = 0, flag = 0;
    while(num > j){
    	printf("|%u", aspen->arrayItems[j].key);
    	++j;
    }
    printf(" /размер %d\n", num);
	while(num > j){
    	while(i-- > 0){
        	if(flag == 0 && i == level-1){
            	printf("|----");
            	flag = 1;
        	}
        	else
            	printf("|\t");
    	}
        printf("|%u\n", aspen->arrayItems[j].key);
        j++;
        i = level;
    }
    puts("|");
   // puts("|");
    j = 0; 
    j = 0;
    puts("--------------");
    while(num >= j){
        putTree(aspen->child[j], level+1);
        j++;
    }
    return 0;
}*/


int load(Node **proot, char *fname){
	//char *fname = calloc();// *pwd = strdup("files/"); 
	//fname = strcat(pwd, ffname);
    FILE *fd = fopen(fname, "r");
    if(!fd)
        return 0;
    char *key, *info;
  	unsigned int k;
    //int m;
    while(!feof(fd)){
        key = GetStr_F(&fd);
        info = GetStr_F(&fd);
      //  puts("круто");
        if(key != NULL && info != NULL && strcmp(key,"") != 0 && strcmp(info,"") != 0){
			k = (unsigned int)atoi(key);
        //    puts("вставка");
        //    printf("key >> %u, info >> %s\n", k, info);
            if(insert(proot, k, info) != 0)
            	free(info);
        }
        else {
            if(info) free(info);
        }
        free(key);
    }
    fclose(fd);
    fd = NULL;
   // free(pwd);
    return 1;
}



int putTree(Node *node, int len ){
	int i;
    int l=len;
    if (node){
                for (int j = node->numItems-1;j >= 0;j--){
                        putTree(((node)->child[j+1]),l+1);
                        if (j < node->numItems){
                                for (i = 0;i < l ;i++){
                                        printf("   |");
                                }
                                printf("%u\n",(node)->arrayItems[j].key);
                        }
                }
                putTree((node)->child[0],l+1);
   			return 0;     
   }
   return 3;
}        

int bypass(Node **proot, int ind){ // ind - заданное число десятичных разрядов
    Node *ptr = *proot;
    int rc, pos;
    if(!ptr)
        return 3;
    ptr = findMin(proot,  &pos);
    do{
        if(decimal_place(ptr->arrayItems[pos].key) == ind)
            printf("%u, ", ptr->arrayItems[pos].key);
        else if(decimal_place(ptr->arrayItems[pos].key) > ind)
            return 0;
        rc = pos;
        ptr = findNext_n(ptr, rc, &pos);
    }while(ptr);
    return 0;
}
