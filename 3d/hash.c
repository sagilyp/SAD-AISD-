#include "hash.h"

#include <stdlib.h>
#include <string.h>

int get_easy(int N){
    int i, t, k, res;
    for (k=2; k<=N; k++){
        i = 2;
        t = 0;
        while(i < k){
            if (k%i==0){
                t++;
            }
            i++;
        }
        if(t==0){
            res = k;
        }
    }
    return res;
}
int hash1(char *key){
    int res = 1;
    for(int i = 0; i < (int)strlen(key); ++i){
        res = res * (int)key[i];
    }
    return 37*res;
}

int hash2(char *key){
    int r = 0;
    for(int i = 0; i < (int)strlen(key); ++i){
        r = r + (int)key[i];
    }
    return 13 * get_easy(r);
}

int hash(char *key, int i, int m){
    return abs((hash1(key) + i*hash2(key)) % m);
}
