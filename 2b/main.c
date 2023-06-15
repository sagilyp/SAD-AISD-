
#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "queue.h"
                           

 int main() {                                                                                                                                
    int flag = 0;                                                                                                                                 
	while( 1 ){
     do{
     	 Item *mas = NULL;
         Trash bag = {0, 0, 0, 0};                                                                                                           
		 Queue smart;
		 int k;	
		 #ifdef V_IM
		 int s;                                                                                                                   
         printf("Введите размер массива >>\n");  
         s = GetInt(&k);                                                                                                          
         while(s != 0 || k <= 0){
             if(s == 1){                                                                                                                     
                 printf("До свидания!\n");
                 return 0;                                                                                                                   
             }
             printf("Введите натуральное число >>\n");                                                                                                    
             s = GetInt(&k);
         }
         #endif
		 queue_new(&smart, k);
         int l;
         l = input(&bag, &mas);
         while(l != 0){
             if(l == -1){
                 printf("До свидания!\n");
				 erase_queue(&smart);
                 free(mas);
                 return 0;
             }
             printf("Неверный формат ввода. Попробуйте снова:\n");
             l = input(&bag, &mas);
         }
         qsort(mas, bag.len, sizeof(Item), (int (*)(const void *, const void * ))comp);
         if(process(&smart, mas, bag) == 1){
             flag = 1;                                                                                                                       
             printf("\tОшибка памяти:выделенного размера массива\n"
              "\tне хватило для выполнения данной задачи. Попробуйте ещё раз\n");
         }
         else flag = 0;
         free(mas);
         erase_queue(&smart);
     }while(flag == 1);
     }
     printf("До свидания!\n");
     return 0;
 }

