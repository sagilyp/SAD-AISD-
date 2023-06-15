#include "dialog.h"
#include "table.h"
#include "struct.h"
#include "hash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int GetInt(int *a){
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
    printf("Enter key: -->\n");
    m = GetStr();
    if (m == NULL) {
        return 0; // обнаружен конец файла
    }
    printf("Enter info: -->\n");
    b = GetStr();
    if (b == NULL){
        return 0; // обнаружен конец файла
    }
    rc = Add(ptab, m, b); // вставка элемента в таблицу
   // #ifdef LAB_D
    	free(m);
    	free(b);
   // #endif
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_ReleaseFind(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int k, rc, rel, pos;
    char *key = NULL;
    puts("Enter key >>");
    key = GetStr();
    if (key == NULL) {
        free(key);
        return 0; // обнаружен конец файла
    }
    puts("Enter release number >>");
    k = GetInt(&rel);
    if (k == 1) {
        return 0; // обнаружен конец файла
    }
    rc = findRelease_s(ptab, key, rel);
    free(key);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Find(Table *ptab) {
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined","Void table"};
    char *key = NULL;
    int rc;
    puts("Enter key: -->");
    key = GetStr();
    if (key == NULL)
        return 0; // конец файла
    rc = findInfo_s(ptab, key);
    free(key);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_ReleaseDelete(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int rc, k, rel;
    char *key = NULL;
    puts("Enter key: -->");
    key = GetStr();
    if (key == NULL) {
        return 0; // обнаружен конец файла
    }
    puts("Enter release number: -->");
    k = GetInt(&rel);
    if (k == 1) {
        return 0; // обнаружен конец файла
    }
    k = kFind_r(ptab, key, rel);
    rc = Delete(ptab, k);
    free(key);
    printf("%s\n", errmsgs[rc]);
    return 1;
}


int D_Delete(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int rc, k, rel;
    char *key = NULL;
    puts("Enter key: -->");
    key = GetStr();
    if (key == NULL) {
        return 0; // обнаружен конец файла
    }
    k = kFind(ptab, key, 1);
    rc = Delete(ptab, k);
    while(k >= 0) {
        k = kFind(ptab, key, 0);
        Delete(ptab, k);
    }
    free(key);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Show(Table *ptab){
    const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int rc;
    rc = show(ptab);
    printf("%s\n", errmsgs[rc]);
    return 1;
}


//#ifdef LAB_D
	int D_Load(Table *ptab){
		    int SZ;
		    char *fname = NULL;
		    printf("Enter file name: --> ");
		    fname = GetStr(); // вся строка вводится целиком
		    if(fname == NULL)
		        return 0; // конец файла
		    if (load(ptab, fname) == 0){ // указанный файл не существует надо - создать новый файл и таблицу
		        printf("Enter possible vector size: -->");
		        if (GetInt(&SZ) == 1)
		            return 0; // конец файла
		        create(ptab, fname, SZ);
		    }
		    free(fname);
		    return 1;
		}	

	int D_Save(Table *ptab)
	{
	    fseek(ptab->fd, sizeof(int), SEEK_SET);
	    fwrite(&ptab->csize, sizeof(int), 1, ptab->fd);
	    //fwrite(ptab->first, sizeof(Item), ptab->n, ptab->fd);
	    fclose(ptab->fd);
	    ptab->fd = NULL;
	    return 1;
	}
//#endif

