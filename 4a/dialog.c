
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#include "struct.h"
#include "headers/func.h"

const char *errmsgs[] = {"Ok", "Don't find key",  "Void tree"};

int GetInt(int *a){
    int n;
    while ((n=scanf("%d", a)) != 1){
        if(n<0){
            return 1;
        }
        puts("You are wrong. Repeat, please!");
        scanf("%*[^\n]");
        scanf("%*c");
    }
    return 0;
}

char *GetStr(){
    char buf[81] ={0};
    char *res = NULL;
    int len = 0;
    int n;
    do{
        n = scanf("%80[^\n]", buf);
        if (n < 0){
            return NULL;
        }
        else if (n > 0){
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            strncpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else{
            scanf("%*c");
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

int dialog(const char *msgs[], int N)
{
    const char *errmsg = "";
    int rc;
    int i, n;

    do{
        puts(errmsg);
        errmsg = "You are wrong. Repeat, please!";

        // вывод списка альтернатив
        for (i = 0; i < N; ++i)
            puts(msgs[i]);
        puts("Make your choice: --> ");

        n = GetInt(&rc); // ввод номера альтернативы
        if (n == 1) // конец файла - конец работы
            rc = 0;
    } while (rc < 0 || rc >= N);
    scanf("%*[^\n]");
    scanf("%*c");
    return rc;
}

// Диалоговая функция включения в дерево нового элемента.
// Требует ввести ключ.
// Если ключ задан неправильно, вся строка игнорируется
int D_Add(Node **paspen)
{
    int rc;
    char *k = NULL, *i = NULL;
    puts("Enter key: -->");
    k = GetStr();
    if (k == NULL)
        return 0; // обнаружен конец файла
    puts("Enter info: -->");
    i = GetStr();
    if (i == NULL)
        return 0; // обнаружен конец файла
    rc = insert(paspen, k, i);	// вставка элемента в таблицу
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Find(Node **paspen){
    int rc;
    char *k;
    Node *res;
    puts("Enter key: -->");
    k = GetStr();
    if (k == NULL)
        return 0; // обнаружен конец файла
    res = find(paspen, k, &rc);
    if(rc == 0){
        printf("key >> \"%s\", information >> \"%s\"\n", res->key, res->info);
    }
    free(k);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Delete(Node **paspen){
    int rc;
    char *k;
    puts("Enter key: -->");
    k = GetStr();
    if (k == NULL)
        return 0; // обнаружен конец файла
    rc = delNode(paspen, k);
    free(k);
    printf("%s\n", errmsgs[rc]);
}

int D_MinFind(Node **paspen){
    Node *res = findMin(paspen);
    int rc = 0;
    if(!res)
        rc = 2;
    else
        printf("key >> \"%s\", information >> \"%s\"\n", res->key, res->info);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Show(Node **paspen){
    int rc = 0;
    printf("\n=== Your tree: ===\n\n");
    putTree(*paspen, 0);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

void delTree(Node **paspen){
    if(*paspen){
        delTree(&(*paspen)->left);
        free((*paspen)->key);
        free((*paspen)->info);
        delTree(&(*paspen)->right);
        free(*paspen);
    }
    *paspen = NULL;
}

void delTree_T(Node **paspen){
	if(*paspen){
		delTree_T(&((*paspen)->left));
		delTree_T(&((*paspen)->right));
		free(*paspen);
	}
}

int D_Import(Node **paspen){
    char *fname = NULL;
    printf("Enter file name: --> ");
    fname = GetStr(); // вся строка вводится целиком
    if(fname == NULL)
        return 0; // конец файла  
    if (load(paspen, fname) == 0){ // указанный файл не существует надо
        printf("Error! Unpossible file name.\n");
    }
    free(fname);
    return 1;
}

int D_Bypass(Node **paspen){
    int rc;
    char *k;
    puts("Enter key: -->");
    k = GetStr();
    if (k == NULL)
        return 0; // обнаружен конец файла
    rc = bypass(paspen, k);
    free(k);
    printf("%s\n", errmsgs[rc]);
}

// таймирование. Аргумент функции не нужен, так как создаётся новое дерево
int D_Timing(Node **){
	FILE *fd = fopen("files/find.csv","w");
	char ent = '\n';
    Node *root = NULL; //&EList;
    int n = 10, cnt = 10000, i, j, m, rc, apr = 10;  // n -  высота дерева, cnt - кол-во узлов , i - счёт., m - счёт
    char key[10000][5], k[5]; // key[] - массив случайных ключей, k - ключ-вставка
    char info[] = "info"; // что такое *key[1000]?
    clock_t first, last;
    int res[apr][n];
	while(apr-- > 0){  
    	srand(time(NULL));
    	n = 10;
    	while (n-- > 0){
        	for (i = 0; i < 10000; ++i){
            	for(j = 0; j < 4; ++j) {
                	key[i][j] = (char)(((rand() * rand())) % 26 + 97);
            	}
            	key[i][4] = '\0';
            //printf("%s\n", key[i]);
        	}
        	for (i = 0; i < cnt; ){
            	for(j = 0; j < 4; ++j) {
                	k[j] = (char)(((rand() * rand())) % 26 + 97);  // создали ключ
            	}
            	k[4] = '\0';
            	if(insert(&root, k, info) == 0)   // вставка ключа в дерево
            		++i;
        	}
        	m = 0;
        	first = clock();
        	for (i = 0; i < 10000; ++i)
            	if (delNode(&root, key[i])) // ищем ключ по произвольному значению из массива ключей
                	++m;
        	last = clock();
        	printf("%d items was found\n", m);
        	printf("test #%d, number of nodes = %d, time = %ld\n", 10 - n, (10 - n)*cnt, (last - first)); 
        	res[9-apr][9-n] = (int)(last-first);
    	}
    	delTree_T(&root);
    }
    for(i = 0; i < 10; ++i){
    	for(j = 1; j < 10; ++j){
    		res[0][i] += res[j][i];
    	}
    	res[0][i] = res[0][i]/10;
    	fprintf(fd, "%d;%d", (i+1)*10000, res[0][i]);
    	//fprintf(fd, "%c", ent);
		fprintf(fd, "%c", '\n');
    } 
    fclose(fd);
    return 1;
}

int img(FILE *fd, Node **paspen, int level){
	Node *ptr = *paspen;
	if(!ptr)
		return 0;
	//fprintf(fd, "%s", ptr->key);	
	if(ptr->left != NULL && strcmp(ptr->key, ptr->left->key) == 0){
		fprintf(fd, "%s", ptr->key);
		fprintf(fd, "%s", ".vr");
		fprintf(fd, "%d", level);
		fprintf(fd, "%s", " -> ");
		fprintf(fd, "%s", ptr->left->key);
		fprintf(fd, "%s", ".vr");
		fprintf(fd, "%d", level+1);
		fprintf(fd, "%c", '\n');
	}
	else if(ptr->left != NULL && strcmp(ptr->key, ptr->left->key) != 0){
		fprintf(fd, "%s", ptr->key);
		level = -1;
		fprintf(fd, "%s", " -> ");
		fprintf(fd, "%s", ptr->left->key);
		fprintf(fd, "%c", '\n');
	}
	if(ptr->right != NULL){
	//	fprintf(fd, "%c", ',');
		fprintf(fd, "%s", ptr->key);
		fprintf(fd, "%s", " -> ");
		fprintf(fd, "%s", ptr->right->key);
		fprintf(fd, "%c", '\n');
	}
	img(fd,&(ptr->left), level+1);
	img(fd,&(ptr->right), level+1);
}

int D_Image(Node **paspen){
	FILE *f = fopen("img.gv", "w");
	if(!f)
		return 0;
	fprintf(f, "%s\n", "digraph G{");
	img(f, paspen, 0);	
	fprintf(f, "%s\n", "}");
	fclose(f);
	system("dot -Tpng img.gv -o g.png && catimg g.png");
	system("rm img.gv && rm g.png");
	return 1;
	
}
