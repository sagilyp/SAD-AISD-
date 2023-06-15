#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void queue_new(Queue *smart, int size){
	smart->head = NULL;
	smart->tail = NULL;
}

void erase_queue(Queue *queue) {
    Node *ptr = queue->head;
    Node *ptr_prev;
    while (ptr) {
        ptr_prev = ptr;
        ptr = ptr->next;
        free(ptr_prev);
    }
}

int queue_push(Queue *queue, Item data) {
    Node *new = (Node *) malloc(sizeof(Node));
    if (!new) {
        return 1;
    }
    new->data = data;
    new->next = NULL;
    if (!queue->head) {
        queue->head = new;
        queue->tail = new;
    } else {
        queue->tail->next = new;
        queue->tail = new;
    }
    return 0;
}

int queue_pop(Queue *queue, Item *output) {
    if (!queue->head) {
        return 1;
    }
    *output = queue->head->data;
    if (queue->head == queue->tail) {
        queue->tail = NULL;
    }
    Node *head = queue->head;
    queue->head = head->next;
    free(head);
    return 0;
}
