#ifndef LISTAENLAZADA_H
#define LISTAENLAZADA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct NodeReg {
    unsigned int valor;
    unsigned int posicion;
    struct NodeReg *next;
};

struct NodeReg *insertaReg(struct NodeReg *p, unsigned int x, unsigned int y) {//Función de manipulación de lista en la cual se guardarán los nodos visitados.
    struct NodeReg *q, *l;
    if (p == NULL) { // p is an empty list
        q = calloc(1, sizeof(struct NodeReg));
        q->valor = x;
        q->posicion = y;
        q->next = NULL;
        p = q;
    } else {
        q = calloc(1, sizeof(struct NodeReg));
        q->valor = x;
        q->posicion = y;
        q->next = NULL;
        l = p;
        while (l->next != NULL)
            l = l->next;
        l->next = q;
    }
    return p;
}

struct NodeReg *DeleteFirstReg(struct NodeReg *p){
    struct NodeReg *q;
    if (p != NULL) {
        q = p->next;
        free(p);
        return q;
    }
    return NULL;
}

struct NodeReg *DeleteLastReg(struct NodeReg *p){
    struct NodeReg *q;
    struct NodeReg *head = p; // Guardar referencia al primer nodo

    // Verificar que la lista no esté vacía
    if(p == NULL){
        return NULL;
    }

    // Si solo hay un elemento
    if(p->next == NULL){
        free(p);
        return NULL;
    }
    
    // Encontrar el penúltimo nodo
    while (p->next->next != NULL){
        p = p->next;
    }
    
    // Ahora p apunta al penúltimo nodo
    q = p->next;      // Guardar referencia al último nodo
    p->next = NULL;   // Desconectar el último nodo
    free(q);          // Liberar el último nodo
    
    return head;      // Retornar el primer nodo
}


struct NodeReg *KillAllReg(struct NodeReg *p){
    while (p != NULL)
        p = DeleteFirstReg(p);
    return NULL;
}

void PrintListReg(struct NodeReg *p){
    while (p != NULL) {
        printf("[ %u, %u] ",p->valor, p->posicion);
        p = p->next;
    }
    printf("\n");
}
#endif // LISTAENLAZADA_H