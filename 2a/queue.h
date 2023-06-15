#ifndef QUEUE
#define QUEUE

#include "item.h"

#ifdef L_IM
typedef struct Node{                                                                                                                          
    Item data;                                                                                                                                
    struct Node *next;                                                                                                                        
}Node;                                                                                                                                       
                                                                                                                                              
 typedef struct Queue{                                                                                                                        
     Node *head;                                                                                                                              
     Node *tail;                                                                                                                              
 }Queue;
#endif

#ifdef V_IM
 typedef struct Queue{                                                                                                                        
     int head;                                                                                                                                
     int tail;                                                                                                                                
     Item *q_arr;                                                                                                                             
     int cnt;                                                                                                                                 
     int size;                                                                                                                                
 }Queue;       
#endif

void queue_new(Queue *smart, int size);
int queue_push(Queue *smart, Item elem);
int queue_pop(Queue *smart, Item *temp);
void erase_queue(Queue *smart);

#endif
