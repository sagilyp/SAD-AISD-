#define DELIM_SP " "                                                                                                                           
#define  DELIM_SL '/'


#include <stdio.h>
#include "func.h"
#include "queue.h"
#include <stdlib.h>
#include <readline/readline.h>


int GetInt(int *a){                                                                                                                          
     int n;                                                                                                                                   
     while ((n=scanf("%d", a)) != 1){                                                                                                         
          if(n<0){                                                                                                                            
              return 1;                                                                                                                       
          }                                                                                                                                   
          scanf("%*[^\n]");                                                                                                                   
          scanf("%*c");                                                                                                                       
      }                                                                                                                                       
     return 0;                                                                                                                                
 }                                                                                                                                            
                                                                                                                                              
                                                                                                                                              
 char *my_strtok(char *p,char delim){                                                                                                         
     static char *old;                                                                                                                        
     int j = 0;                                                                                                                               
     char *buf = NULL;                                                                                                                        
     char *tkn = NULL;                                                                                                                        
     if (p != NULL) {                                                                                                                         
         old = NULL;                                                                                                                          
         buf = p;                                                                                                                             
     }                                                                                                                                        
     else{                                                                                                                                    
         buf = old;                                                                                                                           
     }                                                                                                                                        
     if(buf[0] == '\0'){                                                                                                                      
         return NULL;                                                                                                                         
     }                                                                                                                                        
     while(buf[j] == delim && buf[j] != '\0' ){                                                                                               
         j++;   
     }                                                                                                                                        
     tkn = buf+j;                                                                                                                             
     while (buf[j] != delim && buf[j] != '\0'){                                                                                               
         j++;                                                                                                                                 
     }                                                                                                                                        
     if(buf[j] != '\0') {                                                                                                                     
         buf[j] = '\0';                                                                                                                       
         j++;                                                                                                                                 
         old = buf+j;                                                                                                                         
     }                                                                                                                                        
     else{                                                                                                                                    
         old = buf + j;                                                                                                                       
     }
     return tkn;
 }

 int input(Trash *bag, Item **mas){
     Item elem;
     char *str = NULL;
     int i = 0;
     char *tkn;
     int temp;
     float temp_f;
     char *tkn_atom;
     int flag_m = 0;
     int flag_p = 0;
     int n = 0;
     printf("Вас приветсвует модель распределения процессорного времени по алгоритму Round Robin\n"
            "Введите входные параметры в указанном виде:frq id/tc/te,\n"
            " где frq - частота процессора (натуральное число > 0)\n"            
            "      id — идентификатор процесса (число),\n"
            "      tc — время появления процесса (натуральное число),\n"
            "      te — требуемое процессом процессорное время (натуральное число)\n");
         str = readline(""); //  5 1/2/3 2/3/4
         if(str == NULL){
             free(str);
             return -1;
         }
         tkn = strtok(str, DELIM_SP);
         if(tkn == NULL){
             free(str);
             return 1;
         }
         while (tkn[i] != '\0') {
             char a = tkn[i];
             int das = a;
             if (das < '0' || das > '9') {
                 free(str);
                 return 1;
             }
             ++i;
         }
         i = 0;
         bag->frq = atoi(tkn);
         tkn = strtok(NULL, DELIM_SP);
         if(tkn == NULL){
             free(str);
             return 1;
         }
         while(tkn != NULL){
             tkn_atom = my_strtok(tkn, DELIM_SL);
             while(tkn_atom != NULL){
                 while(tkn_atom[i] != '\0'){
                     char a = tkn_atom[i];
                     int das = a;
                     if(das == '-' && n+1 == 1){
                         flag_m += 1;
                     }
                     if(das == '-' && i != 0){
                         free(str);
                         return 1;
                     }
                     if(das == '.' && n+1 == 1){
                         flag_p += 1;
                     }
                     if(a == '.' && i == 0){
                         free(str);
                         return 1;
                     }
                     if(i != 0 && a == '.' && tkn_atom[i-1] == '-'){
                         free(str);
                         return 1;
                     }
                     if((das < '0' || das > '9') && flag_p > 1 && n+1 == 1){
                         free(str);
                         return 1;
                     }
                     if((das < '0' || das > '9') && flag_m > 1 && n+1 == 1){
                         free(str);
                         return 1;
                     }
                     if ((das < '0' || das > '9') && n+1 != 1){
                         free(str);
                         return 1;
                     }
                     ++i;
                 }
                 i = 0;
                 ++n;
                 if(n == 1) temp_f = atof(tkn_atom);
                 else temp = atoi(tkn_atom);
                 if(n > 3){
                     free(str);
                     return 1;
                 }
                 else if(n == 1) elem.id = temp_f;
                 else if(n == 2) elem.tc = temp;
                 else if(n == 3){
                     elem.te = temp;
                     bag->cvant += temp;
                 }
                 tkn_atom = my_strtok(NULL, DELIM_SL);
                 flag_m = 0;
                 flag_p = 0;
             }
             if(n < 3 ){
                 free(str);
                 return 1;
             }
             n = 0;
             (bag->len)++;
             *mas = (Item *)realloc(*mas, (bag->len)*sizeof(Item));
             (*mas)[(bag->len)-1] = elem;
             tkn = strtok(NULL, DELIM_SP);
         }
     free(str);
     return 0;
 }
 
 int comp(const Item *elem1, const Item *elem2){
     if(elem1->tc > elem2->tc){
         return 1;
     }
     if(elem1->tc < elem2->tc){
         return -1;
     }
     else return 0;
 }                           

