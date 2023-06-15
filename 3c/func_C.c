#include "table.h"
#include "hash.h"
#include "struct.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Поиск по ключу, -1 если не нашёл
int kFind(Table *ptab, char *key, int bull){
    static int ern, n;
    int cnt, pos;
    if(bull == 1) {
        pos = hash(key, (int)strlen(key), ptab->msize);
        cnt = 0;
    }
    else{
        pos = (ern+1) % ptab->msize;
        cnt = n+1;
    }
    while(cnt < ptab->msize){
        if(ptab->ks[pos].busy == 1 && strcmp((ptab->ks)[pos].key, key) == 0){
            ern = pos;
            n = cnt;
            //printf("ern %d pos %d\n", ern, pos);
            return pos;
        }
        pos = (pos + 1) % ptab->msize;
        ++cnt;
    }
    return -1;
}


//поиск по релизу, -1 не нашёл
int kFind_r(Table *ptab, char *key, int rel){
    int cnt = 0, pos;
    pos = hash(key, (int)strlen(key), ptab->msize);
    while(cnt < ptab->msize){
        if(ptab->ks[pos].busy == 1 && strcmp(ptab->ks[pos].key, key) == 0 && ptab->ks[pos].release == (unsigned short)rel){
            return pos;
        }
        pos = (pos + 1) % ptab->msize;
        ++cnt;
    }
    return -1;
}

unsigned short findRel(Table *ptab, char *key){
    int i;
    unsigned short max;
    i = kFind(ptab, key, 1);
    if(i < 0) return 1;
    max = (ptab->ks)[i].release;
    i = kFind(ptab, key, 0);
    while(i >= 0){
        if((ptab->ks)[i].release > max){
            max = (ptab->ks)[i].release;
        }
        i = kFind(ptab, key, 0);
    }
    return max+1;
}

int Add(Table *ptab, char *key, char *info){
    if( ptab->msize == 0){
        free(key);
        free(info);
        return 5;// пустая
    }
    if(ptab->csize == ptab->msize){
        free(key);
        free(info);
        return 2;// переполнение
    }
    int i, cnt = 0, rel = 1;
    i = hash(key, (int)strlen(key), ptab->msize);
    printf("hash sum: %d\n", i);
    rel = findRel(ptab, key);
    while(ptab->ks[i].busy == 1 && cnt < ptab->msize){
        i = (i+1) % ptab->msize;
        ++cnt;
    }
    if(cnt < ptab->msize){
        ptab->ks[i].info = info;
        ptab->ks[i].key = key;
        ptab->ks[i].busy = 1;
        ptab->ks[i].release= (unsigned short)rel;
    }
    ptab->csize ++;
    return 0;
}


int Delete(Table *ptab, int k){
    if(ptab->csize == 0){
        return 5; //пустая таблица
    }
    if (k == -1) {
        return 3; // нет такого ключа
    }
    (ptab->ks)[k].busy = 0;
    (ptab->ks)[k].release = 0;
    free((ptab->ks)[k].info);
    free((ptab->ks)[k].key);
    (ptab->ks)[k].info = NULL;
    (ptab->ks)[k].key = NULL;
    --ptab->csize;
    return 0;
}



// найти ключ по номеру релиза
//
int findRelease_s(Table *ptab, char *key, int rel){
    if(ptab->csize <= 0) return 5;
    int k = kFind_r(ptab, key, rel);
    if(k < 0) return 3;
    printf("\t   Busy \t  Key \t\t\t  Info \t\t  Release\n");
    printf("_________________________________________________________________________________\n");
    printf("\t  %5hu\t  %14s\t  %14s\t  %5hu\n", (ptab->ks)[k].busy, (ptab->ks)[k].key, (ptab->ks)[k].info, (ptab->ks)[k].release);
    return 0;
}



int findInfo_s(Table *ptab, char *key){
    if(ptab->csize == 0) return 5;
    int k = kFind(ptab, key, 1);
    if(k < 0) return 3;
    printf("\t   Busy \t  Key \t\t\t  Info \t\t  Release\n");
    while (k >= 0) {
        printf("_________________________________________________________________________________\n");
        printf("\t  %5hu\t  %14s\t  %14s\t  %5hu\n", (ptab->ks)[k].busy, (ptab->ks)[k].key, (ptab->ks)[k].info, (ptab->ks)[k].release);
        k = kFind(ptab, key, 0);
    }
    return 0;
}

int show(Table *ptab){
    if(ptab->msize == 0){
        return 5;
    }
    printf("\t   Busy \t  Key \t\t\t  Info \t\t  Release\n");
    for(int i = 0; i < ptab->msize; ++i){
        printf("_________________________________________________________________________________\n");
        printf("\t  %5hu\t  %14s\t  %14s\t  %5hu\n", (ptab->ks)[i].busy, (ptab->ks)[i].key, (ptab->ks)[i].info, (ptab->ks)[i].release);
    }
    return 0;
}
