#ifndef DEQUE_
#define DEQUE_

#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    Vector2* data;     
    int size;           
    int capacity;       
} Deque;


Deque* create_deque() {
    Deque* dq = (Deque*)calloc(1, sizeof(Deque));
    dq->capacity = 10;
    dq->size = 0;
    dq->data = (Vector2*)calloc(dq->capacity , sizeof(Vector2));
    return dq;
}

void push_front(Deque* dq, Vector2 item) {
    if (dq->size == dq->capacity) {
        dq->capacity *= 2;
        dq->data = (Vector2*)realloc(dq->data, dq->capacity * sizeof(Vector2));
    }
    
    for (int i = dq->size; i > 0; i--) {
        dq->data[i] = dq->data[i-1];
    }
    
    dq->data[0] = item;
    dq->size++;
}

void push_back(Deque* dq, Vector2 item) {
    if (dq->size == dq->capacity) {
        dq->capacity *= 2;
        dq->data = (Vector2*)realloc(dq->data, dq->capacity * sizeof(Vector2));
    }
    
    dq->data[dq->size] = item;
    dq->size++;
}

void 
pop_back(Deque* dq) 
{
    if(dq->size < 1)
        return;
    dq->size--;

}

Vector2 get(Deque* dq, int index) {
    return dq->data[index];
}

#endif
