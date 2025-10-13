//en este archivo vamos a probar las funciones del arbol AVL y lista enlazada
#include "tree.h"  // Ya incluye listaEnlazada.h
#include "avlpath.h"
#include <stdio.h>
int main() {
    //Chequeo de si estamos borrando bien los nodos del arbol
    struct AVLPATH* root = NULL;
    root = insertPath_AVL(root, 3);
    root = insertPath_AVL(root, 1);
    root = insertPath_AVL(root, 4);
    root = insertPath_AVL(root, 2);
    root = insertPath_AVL(root, 0);
    inOrderPath(root);
    printf("\n");
    root = DeletePath(root, 1); // Borrar nodo hoja
    inOrderPath(root);
    printf("\n");
    return 0;
}