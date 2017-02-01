#include "solver.h"

typedef struct Node {
    char *state;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *head;
    Node *tail;
    int size;
} Queue;

void node_free(Node *n);
Queue * queue_new();
void queue_free(Queue *q);
Node * remove_first(Queue *q);
void add_last(Queue *q, char *state);