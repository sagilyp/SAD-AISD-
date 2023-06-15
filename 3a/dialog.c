#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
//#include "dialog.h"


int GetInt(int *a){
    int n;
    while ((n=scanf("%u", a)) != 1){
        if(n<0){
            return 1;
        }
        //puts("You are wrong. Repeat, please!");
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

int dialog(const char *msgs[], int NMsgs){
    char *ermsg = "";
    int rc, n;
    do{
        puts(ermsg);
        ermsg = "You are wrong. Repeat, please!";
        for(int i = 0; i < NMsgs; ++i){
            puts(msgs[i]);
        }
        puts("Make your choice >>");
        n = GetInt(&rc);
        if(n == 1) {
            rc = 0;
        }
    }while(rc >= NMsgs || rc < 0);
    scanf("%*[^\n]");
    scanf("%*c");
    return rc;
}

int D_Add(Table *ptab) {
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int k, rc;
    char *errmsg = "You are wrong. Repeat, please!";
    char *m = NULL, *b = NULL;
    unsigned int temp;
    printf("Enter key: -->\n");
    m = GetStr();
    if (m == NULL) {
        return 0; // обнаружен конец файла
    }
    printf("Enter parent's key: -->\n");
    b = GetStr();
    if (b == NULL){
        return 0; // обнаружен конец файла
    }
    printf("Enter info:\n");
    k = GetInt(&temp);
    if (k == 1){
        return 0; // обнаружен конец файла
    }
    unsigned int *info = NULL;
    info = calloc(1, sizeof(unsigned int));
    *info = (unsigned int)temp;
    rc = add(ptab, m, b, info); // вставка элемента в таблицу
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
}

int D_Find(Table *ptab)
{
    unsigned int info = 0;
    char *key = NULL;
    puts("Enter key: -->");
    key = GetStr();
    if(key == NULL)
        return 0; // конец файла
    findInfo(ptab, key, &info);
    if(info){
        printf("key = %s, info = \"%u\"\n", key, info);
        free(key);
    }
    else
        printf("Item %d was not found\n", info);
    return 1;
}

int D_MPtable(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int k, rc;
    char *pkey = NULL;
    if(ptab->csize != 0) {
        puts("Enter par key >>");
        pkey = GetStr();
        if (pkey == NULL) {
            return 0; // обнаружен конец файла
        }
        if(strcmp(pkey,"") == 0){
            free(pkey);
            pkey = NULL;
        }
        k = p_check(ptab, pkey); // есть или нет род ключ совпадающий с заданным(0 или 1)
        rc = make_par_table(ptab, k, pkey);
        free(pkey);
    }
    else{
        k = 0;
        rc = 5; //пустая таблица
    }
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
}

int D_Delete(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int rc, k;
    char *a = NULL;
    puts("Enter key: -->");
    a = GetStr();
    if (a == NULL) {
        return 0; // обнаружен конец файла
    }
    k = k_check(ptab, a);
    rc = delete(ptab, k, a);
    free(a);
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
}

int D_Show(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int rc;
    if(ptab->csize == 0){
        rc = 5;
    }
    rc = show(ptab);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Export(Table *ptab){
	char *fname = NULL;
	puts("Enter file name: >>");
	fname = GetStr();
	if(!fname)
		return 0;
	if(export(ptab, fname) == 0)
		printf("Error! Unpossible file name.\n");
	free(fname);
	return 1;	
}



//#ifdef LAB_A
int D_Make(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int k, s, rc;
    if(ptab->msize != 0){
        k = -1;
        rc = 4;
    }
    else {
        puts("Choose size of the table >>");
        do {
            k = GetInt(&s);
        }while(s < 0);
        if(k == 1){
            return 0; // обнаружен конец файла
        }
        ptab->msize = s;
        ptab->ks = calloc(s, sizeof(KeySpace));
        if(ptab->ks == NULL){
            return 0;
        }
        rc = 0;
    }
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
}

//#endif
