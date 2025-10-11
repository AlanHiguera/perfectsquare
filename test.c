//en este archivo vamos a probar las funciones del arbol AVL
#include "tree.h"  // Ya incluye listaEnlazada.h, no necesitas incluirlo dos veces

int main() {
    struct AVL* root = NULL;
    
    // Crear listas para diferentes nodos
    struct NodeReg* lista1 = NULL;
    struct NodeReg* lista2 = NULL;
    
    // ✅ insertaReg retorna la lista modificada, hay que asignarla
    lista1 = insertaReg(lista1, 2, 1);
    lista1 = insertaReg(lista1, 3, 2);
    printf("Lista1 inicial:\n");
    PrintListReg(lista1);
    
    lista2 = insertaReg(lista2, 4, 1);
    lista2 = insertaReg(lista2, 5, 2);
    printf("Lista2 inicial:\n");
    PrintListReg(lista2);
    
    // ✅ Insertar nodos con key y lista asociada
    root = insert(root, 10, lista1);  // Nodo con key=10 y lista1
    root = insert(root, 20, lista2);  // Nodo con key=20 y lista2
    
    // Crear una tercera lista para el nodo 30
    struct NodeReg* lista3 = NULL;
    lista3 = insertaReg(lista3, 600, 1);
    lista3 = insertaReg(lista3, 700, 2);
    
    root = insert(root, 30, lista3);  // Nodo con key=30 y lista3

    // Imprimir el recorrido en orden del árbol AVL
    printf("Recorrido en orden del árbol AVL:\n");
    inOrder(root);
    printf("\n");

    // Liberar memoria del árbol AVL (ya incluye las listas)
    freeAVL(root);
    
    // NOTA: No necesitas liberar lista1, lista2, lista3 manualmente
    // porque freeAVL ya las libera cuando libera cada nodo

    return 0;
}