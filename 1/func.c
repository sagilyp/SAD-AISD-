#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

void end(){
	printf("До свидания!\n");
}

int GetInt(int *a){                                                                                                                           
    int n;                                                                                                                                    
    while ((n=scanf("%d", a)) != 1){
    	if(n<0){
    		return 0;
    	}                                                                                                          
        scanf("%*[^\n]");                                                                                                                     
        scanf("%*c");                                                                                                                         
    }           
    return 1;                                                                                                                             
 }

int input(Matrix *table){
	int m;
	printf("Введите количество строк в вашей матрице ->\n");
	int n = GetInt(&m);
	while(n != 1 || m < 1 ){
		if(n == 0){
			end();
			return -1;
		}	
		printf("Введите корректное значение ->\n");
		n = GetInt(&m);
	}
	table->size = m;
	table->matr = (Line *)malloc(m * sizeof(Line));
	for(int i = 1; i <= m; i++){
		printf("Введите количество чисел в %d строке ->\n", i);
		int j;
		int k = GetInt(&j);
		while(k != 1 || j < 1 ){
			if(k == 0){
				end();
				return -1;
			}
			printf("Введите корректное значение ->\n");
			k = GetInt(&j);
		}
		table->matr[i-1].line = (int *)malloc(j*sizeof(int));
		table->allot = i;
		table->matr[i-1].n = j;
		for(int l = 0; l < j; ++l){
			int elem;
			printf("Введите %d элемент %d строки ->\n", l+1, i);
			k = GetInt(&elem);
			while(k != 1){
				if(k == 0){
					end();
					return -1;
				}
				printf("Введите корректное значение ->\n");
				k = GetInt(&elem);
			}
			table->matr[i-1].line[l] = elem;
			
		}	
	}
	return 0;
}



Matrix *ind(Matrix *table){
	Matrix res = {0, NULL};
	res->lines = calloc(table->size, sizeof(Line));
}

int *sum_up(Matrix table){
	int m = table.size;
	int *res = malloc(m * sizeof(int));
	for(int i = 0; i < m; ++i){
		int sum = 0;
		for(int j = 0; j < table.matr[i].n; ++j){
			sum += table.matr[i].line[j];
		}
		res[i] = sum;
	}
	return res;
}


void output(char *str, Matrix table){
	int m = table.size;
	printf("%s\n", str);
	for(int i = 0; i < m; ++i){
		int k = table.matr[i].n;
		for(int j = 0; j < k; ++j){
			printf("%4d", table.matr[i].line[j]);
		}
		printf("\n");
	}
}              

void print_arr(char *str, int *res, Matrix table){
	int p = table.size;
	printf("%s\n", str);
	for(int i = 0; i < p; ++i){
		printf("%d\t\t",res[i]);
	}
	printf("\n");
	
}

void erase(Matrix *table){
	int k = table->allot;
	for(int i = 0; i < k; i++){
		free(table->matr[i].line);
	}
	free(table->matr);
}


