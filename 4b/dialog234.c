#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "headers/dialog234.h"
#include "headers/func234.h"
#include "headers/234.h"


const char *errmsgs[] = {"Ok", "Don't find key",  "Dublicate", "Void tree"};

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

int GetInt_u(unsigned int *a){
    int n;
    while ((n=scanf("%u", a)) != 1){
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
    unsigned int k;
    char *i = NULL;
    puts("Enter key: -->");
    rc = GetInt_u(&k);
    if (rc == 1)
        return 0; // обнаружен конец файла
    scanf("%*c");
    puts("Enter info: -->");
    i = GetStr();
    if (i == NULL)
        return 0; // обнаружен конец файла
    rc = insert(paspen, k, i);	// вставка элемента в таблицу
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Find(Node **paspen){
    int rc, pos;
    unsigned int k;
    Node *res;
    puts("Enter key: -->");
    rc = GetInt_u(&k);
    if (rc == 1)
        return 0; // обнаружен конец файла
    res = find(paspen, k, &pos, &rc);
    if(rc == 0){
        printf("key >> \"%u\", information >> \"%s\"\n", res->arrayItems[pos].key, res->arrayItems[pos].info);
    }
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Delete(Node **paspen){
    int rc;
    unsigned int k;
    puts("Enter key: -->");
    rc = GetInt_u(&k);
    if (rc == 1)
        return 0; // обнаружен конец файла
    rc = delNode(paspen, k);
    printf("%s\n", errmsgs[rc]);
}

int D_FindPrev(Node **paspen){
    Node *res, *ptr;
    int rc = 0, pos, kord;
    unsigned int k;
    puts("Enter number: -->");
    rc = GetInt_u(&k);
    if(rc == 1)
        return 0;
    res = findMin(paspen, &pos);
    ptr = findMax(paspen, &kord);
    if(!res)
    	rc = 3;
    else if(ptr->arrayItems[kord].key <= k)
    	printf("key >> \"%u\", information >> \"%s\"\n", ptr->arrayItems[kord].key, ptr->arrayItems[kord].info);
	else if(res->arrayItems[pos].key >= k)
		printf("key >> \"%u\", information >> \"%s\"\n", res->arrayItems[pos].key, res->arrayItems[pos].info);
    else if(res->arrayItems[pos].key < k){
    	ptr = findNext_n(res, pos, &kord);
		while(ptr->arrayItems[kord].key <= k){
			pos = kord;
			ptr = findNext_n(ptr, pos, &kord);
		}
		ptr = findPrev_n(ptr, kord, &pos);
		printf("key >> \"%u\", information >> \"%s\"\n", ptr->arrayItems[pos].key, ptr->arrayItems[pos].info);
	}
		
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

int D_Export(Node **paspen){
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

void delTree(Node **paspen){
    if(*paspen){
        int num = (*paspen)->numItems;
        while(num >= 0){
            delTree(&(*paspen)->child[num]);
			num --;	
		}
        freeNode(paspen);
    }
    *paspen = NULL;
}

void delTree_Timing(Node **paspen){
	if(*paspen){
		int num = (*paspen)->numItems;
		while(num >= 0){
			delTree_Timing(&(*paspen)->child[num]);
			num --;
		}
		free(*paspen);
	}
	*paspen = NULL;
}

//int D_Export(Node **paspen){
//    char *fname = NULL;
//    printf("Enter file name: --> ");
//    fname = GetStr(); // вся строка вводится целиком
//    if(fname == NULL)
//        return 0; // конец файла
//    if (load(paspen, fname) == 0){ // указанный файл не существует надо
//        printf("Error! Unpossible file name.\n");
//    }
//    free(fname);
//    return 1;
//}

int D_Bypass(Node **paspen){
    int rc;
    unsigned int k;
    puts("Enter digit of number: -->");
    rc = GetInt_u(&k);
    if (rc == 1)
        return 0; // обнаружен конец файла
    rc = bypass(paspen, k);
    printf("%s\n", errmsgs[rc]);
}

// таймирование. Аргумент функции не нужен, так как создаётся новое дерево
int D_Timing(Node **){
	FILE *fd = fopen("files/delete234.csv","w");
    Node *root = NULL; //&EList;
    int n = 10, cnt = 10000, i, j, m, rc, apr = 10;  // n -  высота дерева, cnt - кол-во узлов , i - счёт., m - счёт
    unsigned int key[cnt], k; // key[] - массив случайных ключей, k - ключ-вставка
    //char info[] = "info"; // что такое *key[1000]?
    clock_t first, last;
    int res[cnt][apr];
	while(apr-- > 0){
	srand(time(NULL));	
	n = 10;
    while (n-- > 0){
        for (i = 0; i < cnt; ++i){
        	key[i] = (unsigned int)rand();
        }
        for (i = 0; i < cnt; ){
        	char *info = strdup("info");
            k = (unsigned int)((rand()*rand())%200000);  // создали ключ
            if(insert(&root, k, info) == 0)   // вставка ключа в дерево
            	++i;
        }
        m = 0;
        first = clock();
        for (i = 0; i < cnt; ++i){
            if (!delNode(&root, key[i]))     // (find(&root, key[i], &j, &rc)) // ищем ключ по произвольному значению из массива ключей
                ++m;
        }        
        last = clock();
        printf("%d items was deleted\n", m);
        printf("test #%d, number of nodes = %d, time = %ld\n", 10 - n, (10 - n)*cnt, (last - first)); 
        res[9-apr][9-n] = (int)(last-first);
    }
    delTree_Timing(&root);
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

      
