#include "queue.h" 
#include <stdlib.h>                                                                                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                                                                              
void queue_new(Queue *smart, int size){
	smart->head = 0;
	smart->tail = 0;
	smart->q_arr = NULL;
	smart->cnt = 0;
	smart->size = size;
	smart->q_arr = calloc(size, sizeof(Item));
}

void erase_queue(Queue *smart){
	free(smart->q_arr);	
}

int queue_push(Queue *smart, Item elem){
	if(smart->head == smart->tail && smart->cnt == smart->size){ // очередь полна
		return 1;
	}
	(smart->q_arr)[smart->tail] = elem;
	smart->tail = (smart->tail + 1) % (smart->size);
	(smart->cnt)++;
	return 0;
}


int queue_pop(Queue *smart, Item *temp){
	if(smart->head == smart->tail && smart->cnt == 0){
		return 1; // очередь пуста
	}
	*temp = (smart->q_arr)[smart->head];
	smart->head = (smart->head + 1) % smart->size;
	(smart->cnt)--;
	return 0;
}	
