#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"



int main(){                                                                                                                                 
     Matrix table = {NULL, 0, 0};                                                                                                               
     int *res = NULL;                                                                                                                        
     int foo = input(&table);                                                                                                                
     if(foo == 1){                                                                                                                           
         printf("ОШИБКА!\n");                                                                                                                
         return 1;                                                                                                                           
     }                                                                                                                                       
     if(foo == -1){
     	 erase(&table);                                                                                                                         
         return 0;                                                                                                                           
     }                                                                                                                                       
    // res = sum_up(table);                                                                                                                    
     output("Ваша матрица :" ,table);                                                                                                        
	// print_arr("Получившийся вектор :", res, table);  
	 output("Новая матрица :" ,table);                                                                                                                                                                                                  
     erase(&table);
     free(res); 
     end();                                                                                                                             
     return 0;
}
