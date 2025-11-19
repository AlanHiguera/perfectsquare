#ifndef STACK_H
#define STACK_H
#include "tipStack.h"
struct ElementoPila{
    unsigned char tipo; //0 para no revisado, 1 para revisado
    unsigned int posicion; //posicion en la lista de alcanzabilidad
};

struct Stack{
    unsigned int top;
    unsigned int capacity;
    struct ElementoPila* array;
};

unsigned char isEmpty(struct Stack* stack){
    if(stack->top == 0) {
        return '0';
    } else {
        return '1';
    }
}

struct Stack* createStack(unsigned int capacity){
    struct Stack* stack = (struct Stack*)calloc(1, sizeof(struct Stack));

    stack->capacity = capacity;
    stack->top = 0;

    stack->array = (struct ElementoPila*)calloc(capacity, sizeof(struct ElementoPila));
    return stack;
}

unsigned char isFull(struct Stack* stack){
    if(stack->top == stack->capacity) {
        return '1';
    } else {
        return '0';
    }
}

void push(struct Stack *stack, unsigned char tipo, unsigned int posicion){
    if(isFull(stack)=='1'){
        return;
    }

    struct ElementoPila nuevoElemento;
    nuevoElemento.tipo = tipo;
    nuevoElemento.posicion = posicion;

    stack->top = stack->top + 1;
    stack->array[stack->top] = nuevoElemento;

}

struct ElementoPila pop(struct Stack *stack){
    struct ElementoPila elementoaux;
    elementoaux.tipo = '0'; 
    elementoaux.posicion = 0; 

    if (isEmpty(stack)=='0'){
        return elementoaux;
    }

    elementoaux = stack->array[stack->top];
    stack->top = stack->top - 1;
    return elementoaux;
}

void llenarPila(struct Stack *pila, struct tipStack *tipstackaux){
    unsigned int posicion;
    posicion = 0;
    while(tipIsEmpty(tipstackaux) == '1'){
        posicion = tipPop(tipstackaux);
        push(pila, '0', posicion);
    }
}

void FreeStack(struct Stack* stack){
    if(stack->array != NULL) {
        free(stack->array);
    }
    

    free(stack);
}

void PrintStack(struct Stack *pila){
    struct ElementoPila elemento;

    if(pila == NULL || isEmpty(pila) == '0') {
        return;
    }
    // Recorremos desde el tope hacia abajo sin modificar la pila
    for(int i = pila->top; i >= 1; i = i - 1) {
        struct ElementoPila elemento = pila->array[i];
    }
    printf("\n");
}

#endif