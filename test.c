//en este archivo vamos a probar las funciones del arbol AVL y lista enlazada
#include "tree.h"  // Ya incluye listaEnlazada.h

int main() {
    struct AVL* root = NULL;
    
    // Crear listas para diferentes nodos
    struct L_enlazada* lista1 = NULL;
    struct L_enlazada* lista2 = NULL;
    
    // ✅ insertaLista solo recibe 1 parámetro (posicion)
    lista1 = insertaLista(lista1, 2);
    lista1 = insertaLista(lista1, 3);
    printf("Lista1 inicial:\n");
    PrintLista(lista1);
    
    lista2 = insertaLista(lista2, 4);
    lista2 = insertaLista(lista2, 5);
    printf("Lista2 inicial:\n");
    PrintLista(lista2);
    
    // ✅ Usar insert_AVL (con sufijo)
    root = insert_AVL(root, 10, lista1);  // Nodo con key=10 y lista1
    root = insert_AVL(root, 20, lista2);  // Nodo con key=20 y lista2
    
    // Crear una tercera lista para el nodo 30
    struct L_enlazada* lista3 = NULL;
    lista3 = insertaLista(lista3, 600);
    lista3 = insertaLista(lista3, 700);
    
    root = insert_AVL(root, 30, lista3);  // Nodo con key=30 y lista3

    // Imprimir el recorrido en orden del árbol AVL
    printf("Recorrido en orden del árbol AVL:\n");
    inOrder(root);
    printf("\n");

    // ✅ Usar free_AVL (con guión bajo)
    free_AVL(root);
    
    // NOTA: No necesitas liberar lista1, lista2, lista3 manualmente
    // porque free_AVL ya las libera cuando libera cada nodo

    return 0;
}