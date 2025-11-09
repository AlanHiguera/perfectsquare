#ifndef TIPSTACK_H
#define TIPSTACK_H
// Stack auxiliar que almacena el Alcance temporalmente.

#include <stdio.h>
#include <stdlib.h>

struct tipStack {
    unsigned int top;
    unsigned int capacity;
    unsigned int* array;
};

unsigned int tipIsEmpty(struct tipStack* stack) {
    if(stack->top == 0) {
        return '0';
    } else {
        return '1';
    }
}

// Function to create a stack of given capacity.
struct tipStack* tipCreateStack(unsigned int capacity) {
    struct tipStack* stack = (struct tipStack*)calloc(1, sizeof(struct tipStack));

    stack->capacity = capacity;
    stack->top = 0;

    stack->array = (unsigned int*)calloc(capacity, sizeof(unsigned int));
    return stack;
}

unsigned int tipIsFull(struct tipStack* stack) {
    if(stack->top == stack->capacity) {
        return '1';
    } else {
        return '0';
    }
}

void tipPush(struct tipStack* stack, unsigned int item) {
    if (tipIsFull(stack)=='1'){
        return;
    }
    stack->array[stack->top] = item;
    stack->top = stack->top + 1;
}

unsigned int tipPop(struct tipStack* stack) {
    if(tipIsEmpty(stack)!='0'){
        stack->top = stack->top - 1;
        return stack->array[stack->top];
    }
}


void tipFreeStack(struct tipStack* stack) {
    if(stack->array != NULL) {
        free(stack->array);
    }
    
    // Liberar la estructura del stack
    free(stack);
}

void tipPrintStack(struct tipStack* stack) {
    if(stack == NULL || tipIsEmpty(stack) == '0') {
        return;
    }
    
    // Recorremos desde el tope hacia abajo sin modificar la pila
    for(int i = stack->top - 1; i >= 0; i = i - 1) {
    }
    printf("\n");
}


#endif // TIPSTACK_H