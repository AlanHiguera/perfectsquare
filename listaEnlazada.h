#ifndef L_enlazada_H
#define L_enlazada_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct L_enlazada {
    unsigned int posicion;
    struct L_enlazada *next;
};

// Funciones para L_enlazada (lista simple)
struct L_enlazada *insertaLista(struct L_enlazada *p, unsigned int x) {
    struct L_enlazada *q, *l;
    if (p == NULL) { // p is an empty list
        q = calloc(1, sizeof(struct L_enlazada));
        q->posicion = x;
        q->next = NULL;
        p = q;
    } else {
        q = calloc(1, sizeof(struct L_enlazada));
        q->posicion = x;
        q->next = NULL;
        l = p;
        while (l->next != NULL)
            l = l->next;
        l->next = q;
    }
    return p;
}

struct L_enlazada *DeleteFirstLista(struct L_enlazada *p){
    struct L_enlazada *q;
    if (p != NULL) {
        q = p->next;
        free(p);
        return q;
    }
    return NULL;
}

struct L_enlazada *DeleteLastLista(struct L_enlazada *p){
    struct L_enlazada *q;
    struct L_enlazada *head = p; // Guardar referencia al primer nodo

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
    
    q = p->next;
    p->next = NULL;
    free(q);
    return head;
}

struct L_enlazada *KillAllLista(struct L_enlazada *p){
    while (p != NULL)
        p = DeleteFirstLista(p);
    return NULL;
}

void PrintLista(struct L_enlazada *p){
    while (p != NULL) {
        printf("[ %u ] ",p->posicion);
        p = p->next;
    }
    printf("\n");
}

// Función para copiar una lista (crear una nueva lista con los mismos elementos)
struct L_enlazada *copiarLista(struct L_enlazada *original) {
    if (original == NULL) {
        return NULL;
    }
    
    struct L_enlazada *nuevaLista = NULL;
    struct L_enlazada *actual = original;
    
    while (actual != NULL) {
        nuevaLista = insertaLista(nuevaLista, actual->posicion);
        actual = actual->next;
    }
    
    return nuevaLista;
}

#endif // L_enlazada_H