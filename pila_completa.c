#include <stdio.h>
#include <stdlib.h>

struct StackAux {
    unsigned int *posiciones;  // Array para múltiples posiciones
    int top;
    int maxSize;              
};

void initialize(struct StackAux *stack, unsigned int n){
    stack->maxSize = n;  
    stack->top = -1;
    stack->posiciones = (unsigned int*)malloc(n * sizeof(unsigned int));
}

unsigned char isFull(struct StackAux *stack){
    if(stack->top == stack->maxSize - 1)
        return '1';
    else        
        return '0';  
}

unsigned char isEmpty(struct StackAux *stack){
    if(stack->top == -1)
        return '1';
    else
        return '0';  
}

void push(struct StackAux *stack, unsigned char tipo, unsigned int posicion) {
    if (isFull(stack)=='1') {
        printf("Stack Overflow\n");
        return;
    }

    stack->top = stack->top + 1;
    stack->posiciones[stack->top] = posicion;    
}

unsigned int pop(struct StackAux *stack){
    if (isEmpty(stack)=='1'){
        printf("Stack Underflow\n");
        return 0;
    }

    unsigned int posicion = stack->posiciones[stack->top];
    stack->top = stack->top - 1;
    return posicion;
}

void PrintStack(struct StackAux *pila){
    if (isEmpty(pila) == '1') {
        printf("Pila vacía\n");
        return;
    }
    
    printf("Contenido de la pila (desde el tope):\n");
    for(int i = pila->top; i >= 0; i = i -1) {
        printf("  Índice %d: Posición %u\n", i, pila->posiciones[i]);
    }
    printf("\n");
}

void cleanup(struct StackAux *stack) {
    free(stack->posiciones);
}

int main() {
    printf("=== PRUEBA DE PILA COMPLETA ===\n\n");
    
    struct StackAux pila;
    initialize(&pila, 10);

    printf("1. Agregando elementos:\n");
    push(&pila, '0', 5);
    printf("   Después de push(5):\n");
    PrintStack(&pila);

    push(&pila, '0', 10);
    printf("   Después de push(10):\n");
    PrintStack(&pila);

    push(&pila, '0', 15);
    printf("   Después de push(15):\n");
    PrintStack(&pila);

    printf("2. Sacando elementos:\n");
    unsigned int pos1 = pop(&pila);
    printf("   Pop devolvió: %u\n", pos1);
    PrintStack(&pila);

    unsigned int pos2 = pop(&pila);
    printf("   Pop devolvió: %u\n", pos2);
    PrintStack(&pila);

    unsigned int pos3 = pop(&pila);
    printf("   Pop devolvió: %u\n", pos3);
    PrintStack(&pila);

    printf("3. Intentando pop en pila vacía:\n");
    unsigned int pos4 = pop(&pila);
    printf("   Pop devolvió: %u\n", pos4);

    cleanup(&pila);
    return 0;
}