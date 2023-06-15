
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "func.h"
#include "table.h"
#include "type.h"
//const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};

/*int GetInt(int *a){
    int n;
    while ((n=scanf("%d", a)) != 1){
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
} */

char *GetStr_F(File **fd){
	char buf[81] = {0};
	char *res = NULL;
	int len = 0;
	int n;
	do{
		n = fscanf(*fd, "%80[^\n]", buf);
		if(n < 0)
			return NULL;
		else if(n > 0){
			int chunk_len = (int)strlen(buf);
			int str_len = len + chunk_len;
			res = realloc(res, strlen + 1);
			strncpy(res + len, buf, chunk_len);
			len = str_len;
		}
		else
			fscanf(*fd, "%*c");
	}while(n > 0);
	if(len > 0)
		res[len] = '\0';
	else
		res = calloc(1, sizeof(char));
	return res;		
}

int k_check(Table *ptab, char *key){
    for(int i = 0; i < ptab->csize; ++i){
        if(strcmp((ptab->ks)[i].key,key) == 0){
            return i;
        }
    }
    return -1;
}

int p_check(Table *ptab, char *pkey){
    if(pkey == NULL){
        return -2;
    }
    for(int i = 0; i < ptab->csize; ++i){
        if(strcmp((ptab->ks)[i].key, pkey) == 0){
            return i;
        }
    }
    return -1;
}

int p_find(Table *ptab, char *key, int ern){
  int i = 0, m = ptab->csize - 1;
  while(i <= m){
  	int j = (i+m)/2;
    if((ptab->ks)[j].par == NULL){
        break;
    }
    if(strcmp((ptab->ks)[j].par, key) == 0){
    	return j;
    }
    if(strcmp((ptab->ks)[j].par, key) > 0){
    	m = j - 1;
    }
    else{
    	i = j + 1;
    }	
 }   	
    return -1;
}

int findInfo(Table *ptab, char * key, unsigned int *info)
{
    int i = k_check(ptab, key);
    if (i >= 0){
        info = (ptab->ks)[i].info;
        return 1;
    }
    return 0;
}

void t_shift_r(Table *ptab, int pos){
    for(int i = ptab->csize; i > pos; --i){
        (ptab->ks)[i] = (ptab->ks)[i-1];
    }
}

void t_shift_l(Table *ptab, int pos){
    for(int i = pos; i < ptab->csize-1; ++i){
        ptab->ks[i] = ptab->ks[i+1];
    }
}

void in_tail(Table *ptab, int pos){
    KeySpace temp = {(ptab->ks)[pos].key, (ptab->ks)[pos].par, (ptab->ks)[pos].info};
    t_shift_l(ptab, pos);
    (ptab->ks)[ptab->csize-1] = temp;
}

int swap(Table *ptab, int pos1, int pos2){
	if((pos1 > ptab->csize-1) || (pos2 > ptab->csize-1)){
		return -1;
	}
    KeySpace temp = {(ptab->ks)[pos1].key, (ptab->ks)[pos1].par, (ptab->ks)[pos1].info};
    (ptab->ks)[pos1] = (ptab->ks)[pos2];
    (ptab->ks)[pos2] = temp;
    return 0;
}

int f_insert(Table *ptab, char *pkey){
    if(pkey == NULL){
        return ptab->csize;
    }
    for(int i = 0; i < ptab->csize; ++i){
       // printf("str %s\n", (ptab->ks)[i].par);
        if((ptab->ks)[i].par == NULL || strcmp(pkey, (ptab->ks)[i].par) <= 0){
            return i;
        }
    }
   // printf("Aboba\n");
    return 0;
}


int add(Table *ptab, char *k, char *pk, unsigned int *info){
    int i;
    if(strcmp(pk,"") == 0){
        free(pk);
        pk = NULL;
    }
    if( ptab->msize == 0){
    	free(k);
    	free(pk);
    	free(info);
        return 5;// пустая
    }
    if(k_check(ptab,k) != -1){
    	free(k);
    	free(pk);
    	free(info);
        return 1;// дубликаты
    }
    if(ptab->csize == ptab->msize){
    	free(k);
    	free(pk);
    	free(info);
        return 2;// переполнение
    }
    if(p_check(ptab, pk) == -1){
    	free(k);
    	free(pk);
    	free(info);
        return 3;//нет ключа родителя
    }
    i = f_insert(ptab, pk);
   // printf("pos %d\n", i);
    t_shift_r(ptab, i);
    ptab->csize ++;
    (ptab->ks)[i].key = k;
    (ptab->ks)[i].par = pk;
    (ptab->ks)[i].info = info;
    return 0;
}

/*int D_Add(Table *ptab) {
	const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int k, rc;
    unsigned int temp;
    //char *errmsg = "You are wrong. Repeat, please!";
    char *m = NULL, *b = NULL;
    //unsigned int *info = NULL;
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
    //while (temp < 0) {
    //    puts(errmsg);
    //    k = GetInt(&temp);
    //}
    if (k == 1){
        return 0; // обнаружен конец файла
    }
   // info = calloc(1, sizeof(unsigned int));
   // *info = (unsigned int)temp;
    rc = Add(ptab, m, b, info); // вставка элемента в таблицу
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
}

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

int D_Find(Table *ptab){
	const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int k, rc;
    char *key = NULL;
    if(ptab->csize != 0) {
        puts("Enter key >>");
        key = GetStr();
        if (key == NULL) {
        	free(key);
            return 0; // обнаружен конец файла
        }
        k = KFind(ptab, key);
        if (k != -1) {
            rc = 0; // все окей
            printf("\t  %s\t  %s\t  %u\n", (ptab->ks)[k].key, (ptab->ks)[k].par, *((ptab->ks)[k].info));
        } else {
            rc = 3; // нет ключа
        }
        free(key);
    }
    else{
        k = 0;
        rc = 5; //пустая таблица
    }
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
} */

void delete_tail(Table *ptab, int pos) {
        int i = pos;
        while(swap(ptab, i, i+1) == 0){
        	++i;
        	printf("error %d", i);
        } 
        ptab->csize --;
        free((ptab->ks)[ptab->csize].key);
        free((ptab->ks)[ptab->csize].par);
        free((ptab->ks)[ptab->csize].info);
}

int show(Table *ptab){
	if(ptab->csize == 0){
		return 5;
	}
	for(int i = 0; i < ptab->csize; ++i){
		printf("__________________________________________________________________________________________\n");
		printf("\t  %14s\t  %14s\t  %10u\n", (ptab->ks)[i].key, (ptab->ks)[i].par, *((ptab->ks)[i].info));
	}	
	return 0;
}

int make_par_table(Table *ptab, int pos, char *pkey){
    if(ptab->csize == 0){
        return 5; //пустая таблица
    }
    if(pos == -1){
        return 3; // ключ не найден
    }
    if(pos != -2) {
        const char *canon = (ptab->ks)[pos].key;
        for (int i = 0; i < ptab->csize; ++i) {
            if ((ptab->ks)[i].par != NULL && strcmp((ptab->ks)[i].par, canon) == 0) {
                printf("__________________________________________________________________________________________\n");
                printf("\t  %14s\t  %14s\t  %10u\n", (ptab->ks)[i].key, (ptab->ks)[i].par, *((ptab->ks)[i].info));
            }
        }
    }
    else{
        for (int i = 0; i < ptab->csize; ++i) {
            if (pkey == NULL && (ptab->ks)[i].par == NULL) {
                printf("__________________________________________________________________________________________\n");
                printf("\t  %14s\t  %14s\t  %10u\n", (ptab->ks)[i].key, (ptab->ks)[i].par, *((ptab->ks)[i].info));
            }
        }
    }
    return 0;
}

/*int D_MPtable(Table *ptab){
	const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int k, rc;
    char *pkey = NULL;
    if(ptab->csize != 0) {
        puts("Enter key >>");
        pkey = GetStr();
        if (pkey == NULL) {
            return 0; // обнаружен конец файла
        }
        if(strcmp(pkey,"") == 0){
            free(pkey);
            pkey = NULL;
        }
        k = PCheck(ptab, pkey);
        rc = MPtable(ptab, k, pkey);
        free(pkey);
    }
    else{
        k = 0;
        rc = 5; //пустая таблица
    }
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
}
*/

int delete(Table *ptab, int k, char *key){
    int i;
    char *canon = (ptab->ks)[k].key;
    if(ptab->csize == 0){
        return 5; //пустая таблица
    }
    if (k == -1) {
        return 3; // нет такого ключа
    }
    i = p_find(ptab, canon, 0);
    while(i != -1){
        free((ptab->ks)[i].par);
        (ptab->ks)[i].par = NULL;
        in_tail(ptab, i);
        i = p_find(ptab, canon, 0);
    }
    k = k_check(ptab, canon);
    delete_tail(ptab, k);
    return 0;
}

/*int D_Delete(Table *ptab){
	const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int rc, k;
    char *a = NULL;
    puts("Enter key: -->");
    a = GetStr();
    if (a == NULL) {
        return 0; // обнаружен конец файла
    }
    k = KFind(ptab, a);
    rc = Delete(ptab, k);
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
*/
void delTable(Table *ptab){
	const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Don't find key", "Table is defined", "Void table"};
    int rc;
    if(ptab->msize == 0){
        rc = 5;
    }
    else{
        for(int i = 0; i < ptab->csize; ++i){
            free((ptab->ks)[i].key);
            free((ptab->ks)[i].par);
            free((ptab->ks)[i].info);
        }
        free(ptab->ks);
        rc = 0;
    }
    printf("%s\n", errmsgs[rc]);
}
