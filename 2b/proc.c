#include "queue.h"
#include <stdio.h>
#include "func.h"

 int process(Queue *smart, Item *mas, Trash bag){                                                                                                     
 	 int i = 0;
     int k = 0;                                                                                                                              
     Item temp;
     if(k < bag.len){ // вставляем первый элемент в очередь                                                                                           
     	if(queue_push(smart, mas[k]) == 1){
             return 1;                                                                                                                       
         }
         ++k;                                                                                                                                
     }
     do{                                                                                                                                     
         if(queue_pop(smart, &temp) == 1){ // если очередь пуста
             if(k < bag.len){                                                                                                                
                 if(queue_push(smart, mas[k]) == 1){
                     return 1;                                                                                                               
                 }
                 ++k;                                                                                                                        
             queue_pop(smart, &temp);
             }                                                                                                                               
         } //вытаскиваем элемент из очереди
         printf("\n");                                                                                                                 
         for(int j = 0; j < bag.frq; ++j){ // frq раз выполняем
             if(bag.cvant == 0)   break; // если время истекло                                                                                                  
             if(k < bag.len && mas[k].tc <= i){  // вставка нового элемента в очередь, если время подходит
                 if(queue_push(smart,  mas[k]) == 1){                                                                                        
                     return 1;
                 }                                                                                                                           
                 ++k;
             }             
             while(temp.tc > i){ // приводим к нужному времени
                 ++i;                                                                                                                        
             }
             if(temp.te == 0){ //////// если процесс выполнен                                                                                                 
             	if(queue_pop(smart, &temp) == 1){
                     if(k < bag.len){                                                                                                        
                         if(queue_push(smart, mas[k]) == 1){
                             return 1;                                                                                                       
                         }
                         ++k;                                                                                                                
                     }
                     queue_pop(smart, &temp);                                                                                                
                 }
                 while(temp.tc > i){ // приводим                                                                                             
                     ++i;
                 }                                                                                                                           
             } ////////////*******///////////////---------------|
             temp.te--;                                                                                                                      
             //temp.when[i] = 1;
             printf(">> в квант времени %d выполняется %f процесс\n", i, temp.id);
             ++i;
             bag.cvant--;
         } // wb
         if(temp.te != 0){
             if(queue_push(smart, temp) == 1){
                 return 1;
             }
         }
         else if(k < bag.len && mas[k].tc <= i){          
                 if(queue_push(smart, mas[k]) == 1){
                     return 1;
                 }
                 ++k;
         }
     } while(bag.cvant != 0);
 return 0;
 }
