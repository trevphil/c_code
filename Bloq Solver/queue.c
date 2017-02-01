#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void node_free(Node *n) {
    if (n == NULL) return;
    n->next = NULL;
    free(n);
}

Queue * queue_new() {
    Queue *q = malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void queue_free(Queue *q) {
    if (q == NULL) return;
    Node *current = q->head;
    while (current != NULL) {
        Node *temp = current->next;
        node_free(current);
        current = temp;
    }
    q->tail = NULL;
    free(q);
}

Node * remove_first(Queue *q) {
    if (q->size == 0) return NULL;
    
    Node *first = q->head;
    q->head = first->next;
    first->next = NULL;
    
    int old_size = q->size;
    q->size = old_size - 1;
    
    if (q->size == 0) {
        q->tail = NULL;
    }
    
    return first;
}

void add_last(Queue *q, char *state) {
    Node *new_node = malloc(sizeof(Node));
    new_node->state = state;
    new_node->next = NULL;
    
    if (q->size == 0) {
        q->head = new_node;
        q->tail = new_node;
        q->size = 1;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
        
        int old_size = q->size;
        q->size = old_size + 1;
    }
}
