#ifndef TREE_H
#define TREE_H

// Incluir librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "listaEnlazada.h"

// Declaración de la estructura AVL con unsigned int key y lista enlazada
struct AVL {
    unsigned int key;              // ✅ Cambiado a unsigned int
    struct AVL* left;
    struct AVL* right;
    int height;
    struct NodeReg *listaAlcance;  // ✅ Agregado punto y coma faltante
};

int getHeight(struct AVL* n)
{
    if (n == NULL)
        return 0;
    return n->height;
}

// Function to create a new AVL
struct AVL* createAVL(unsigned int key, struct NodeReg* lista)
{
    struct AVL* AVL = (struct AVL*)calloc(1, sizeof(struct AVL));
    AVL->key = key;
    AVL->listaAlcance = lista;  // Asignar la lista proporcionada
    AVL->height = 1; // New AVL is initially added at leaf
    return AVL;
}

// Utility function to get the maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }

// Function to get balance factor of a AVL
int getBalanceFactor(struct AVL* n)
{
    if (n == NULL)
        return 0;
    return getHeight(n->left) - getHeight(n->right);
}

// Right rotation function
struct AVL* rightRotate(struct AVL* y)
{
    struct AVL* x = y->left;
    struct AVL* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height
        = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height
        = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Left rotation function
struct AVL* leftRotate(struct AVL* x)
{
    struct AVL* y = x->right;
    struct AVL* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height
        = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height
        = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Function to insert a key into AVL tree
struct AVL* insert(struct AVL* AVL, unsigned int key, struct NodeReg* lista)
{
    // 1. Perform standard BST insertion
    if (AVL == NULL)
        return createAVL(key, lista);

    if (key < AVL->key)
        AVL->left = insert(AVL->left, key, lista);
    else if (key > AVL->key)
        AVL->right = insert(AVL->right, key, lista);
    else // Equal keys are not allowed in BST - pero podríamos actualizar la lista
        return AVL;

    // 2. Update height of this ancestor AVL
    AVL->height = 1
                   + max(getHeight(AVL->left),
                         getHeight(AVL->right));

    // 3. Get the balance factor of this ancestor AVL to
    // check whether this AVL became unbalanced
    int balance = getBalanceFactor(AVL);

    // 4. If the AVL becomes unbalanced, then there are 4
    // cases

    // Left Left Case
    if (balance > 1 && key < AVL->left->key)
        return rightRotate(AVL);

    // Right Right Case
    if (balance < -1 && key > AVL->right->key)
        return leftRotate(AVL);

    // Left Right Case
    if (balance > 1 && key > AVL->left->key) {
        AVL->left = leftRotate(AVL->left);
        return rightRotate(AVL);
    }

    // Right Left Case
    if (balance < -1 && key < AVL->right->key) {
        AVL->right = rightRotate(AVL->right);
        return leftRotate(AVL);
    }

    // Return the (unchanged) AVL pointer
    return AVL;
}

// Function to perform preorder traversal of AVL tree
void inOrder(struct AVL* root)
{
    if (root != NULL) {
        inOrder(root->left);
        printf("%u ", root->key);  // Imprimir la key del nodo
        printf("Lista: ");
        PrintListReg(root->listaAlcance);  // Imprimir la lista enlazada separadamente
        inOrder(root->right);
    }
}

// ✅ Función para agregar elementos a la lista del nodo
struct AVL* insertInNodeList(struct AVL* root, unsigned int key, unsigned int valor, unsigned int posicion)
{
    if (root == NULL)
        return NULL;
        
    if (key == root->key) {
        // Agregar elemento a la lista enlazada de este nodo
        root->listaAlcance = insertaReg(root->listaAlcance, valor, posicion);
        return root;
    }
    
    if (key < root->key)
        return insertInNodeList(root->left, key, valor, posicion);
    else
        return insertInNodeList(root->right, key, valor, posicion);
}

// ✅ Función para obtener la lista de un nodo específico
struct NodeReg* getNodeList(struct AVL* root, unsigned int key)
{
    if (root == NULL)
        return NULL;
        
    if (key == root->key)
        return root->listaAlcance;
    
    if (key < root->key)
        return getNodeList(root->left, key);
    else
        return getNodeList(root->right, key);
}

// ✅ Función para liberar memoria del árbol incluyendo las listas
void freeAVL(struct AVL* root)
{
    if (root != NULL) {
        freeAVL(root->left);
        freeAVL(root->right);
        
        // Liberar la lista enlazada del nodo
        if (root->listaAlcance != NULL) {
            root->listaAlcance = KillAllReg(root->listaAlcance);
        }
        
        free(root);
    }
}


#endif // TREE_H