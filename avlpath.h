// AVL para el camino, solo contiene keys, sin listas enlazadas
#ifndef AVLPATH_H
#define AVLPATH_H

#include <stdio.h>
#include <stdlib.h>

struct AVLPATH {
    unsigned int key;              
    struct AVLPATH* left;
    struct AVLPATH* right;
    int height;
};

int getHeightPath(struct AVLPATH* n)
{
    if (n == NULL)
        return 0;
    return n->height;
}

// Function to create a new AVLPATH (solo key, sin lista)
struct AVLPATH* createAVLPath(unsigned int key)
{
    struct AVLPATH* node = (struct AVLPATH*)calloc(1, sizeof(struct AVLPATH));
    node->key = key;
    node->height = 1; // New node is initially added at leaf
    return node;
}

// Utility function to get the maximum of two integers
int maxPath(int a, int b) { return (a > b) ? a : b; }

// Function to get balance factor of AVLPATH
int getBalanceFactorPath(struct AVLPATH* n)
{
    if (n == NULL)
        return 0;
    return getHeightPath(n->left) - getHeightPath(n->right);
}

// Right rotation function
struct AVLPATH* rightRotatePath(struct AVLPATH* y)
{
    struct AVLPATH* x = y->left;
    struct AVLPATH* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = maxPath(getHeightPath(y->left), getHeightPath(y->right)) + 1;
    x->height = maxPath(getHeightPath(x->left), getHeightPath(x->right)) + 1;

    return x;
}

// Left rotation function
struct AVLPATH* leftRotatePath(struct AVLPATH* x)
{
    struct AVLPATH* y = x->right;
    struct AVLPATH* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = maxPath(getHeightPath(x->left), getHeightPath(x->right)) + 1;
    y->height = maxPath(getHeightPath(y->left), getHeightPath(y->right)) + 1;

    return y;
}

// Function to insert a key into AVLPATH tree (solo key, sin lista)
struct AVLPATH* insertPath_AVL(struct AVLPATH* node, unsigned int key)
{
    // 1. Perform standard BST insertion
    if (node == NULL)
        return createAVLPath(key);

    if (key < node->key)
        node->left = insertPath_AVL(node->left, key);
    else if (key > node->key)
        node->right = insertPath_AVL(node->right, key);
    else // Equal keys are not allowed in BST
        return node;

    // 2. Update height of this ancestor node
    node->height = 1 + maxPath(getHeightPath(node->left), getHeightPath(node->right));

    // 3. Get the balance factor of this ancestor node to
    // check whether this node became unbalanced
    int balance = getBalanceFactorPath(node);

    // 4. If the node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotatePath(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotatePath(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotatePath(node->left);
        return rightRotatePath(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotatePath(node->right);
        return leftRotatePath(node);
    }

    // Return the (unchanged) node pointer
    return node;
}

// Function to perform inorder traversal of AVLPATH tree (solo keys)
void inOrderPath(struct AVLPATH* root)
{
    if (root != NULL) {
        inOrderPath(root->left);
        printf("%u ", root->key);  // Solo imprimir la key del nodo
        inOrderPath(root->right);
    }
}

// Function to search for a key in AVLPATH tree
unsigned char searchPath(struct AVLPATH* root, unsigned int key)
{
    if (root == NULL) {
        return '0';
    }
    
    if (root->key == key) {
        return '1';
    }

    if (key < root->key)
        return searchPath(root->left, key);
    else
        return searchPath(root->right, key);
}

// Función auxiliar para encontrar el nodo con valor mínimo (más a la izquierda)
struct AVLPATH* findMinPath(struct AVLPATH* node)
{
    while (node->left != NULL)
        node = node->left;
    return node;
}

struct AVLPATH* DeletePath(struct AVLPATH* root, unsigned int key)
{
    // Caso base: árbol vacío
    if (root == NULL) {
        return NULL;
    }

    // Si la clave a eliminar es menor que la clave del nodo actual
    if (key < root->key) {
        root->left = DeletePath(root->left, key);
    }
    // Si la clave a eliminar es mayor que la clave del nodo actual
    else if (key > root->key) {
        root->right = DeletePath(root->right, key);
    }
    // Si encontramos el nodo a eliminar
    else {
        // Caso 1: Nodo hoja (sin hijos)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        
        // Caso 2: Nodo con un solo hijo
        else if (root->left == NULL) {
            struct AVLPATH* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct AVLPATH* temp = root->left;
            free(root);
            return temp;
        }
        
        // Caso 3: Nodo con dos hijos
        else {
            // Encontrar el sucesor inorden (el menor en el subárbol derecho)
            struct AVLPATH* temp = findMinPath(root->right);
            
            // Copiar la clave del sucesor al nodo actual
            root->key = temp->key;
            
            // Eliminar el sucesor
            root->right = DeletePath(root->right, temp->key);
        }
    }

    // Si el árbol tenía solo un nodo, ya se eliminó
    if (root == NULL)
        return root;

    // Actualizar altura del nodo actual
    root->height = 1 + maxPath(getHeightPath(root->left), getHeightPath(root->right));

    // Obtener el factor de balance
    int balance = getBalanceFactorPath(root);

    // Si el nodo se desbalanceó, hay 4 casos de rotación

    // Caso Left Left
    if (balance > 1 && getBalanceFactorPath(root->left) >= 0)
        return rightRotatePath(root);

    // Caso Left Right
    if (balance > 1 && getBalanceFactorPath(root->left) < 0) {
        root->left = leftRotatePath(root->left);
        return rightRotatePath(root);
    }

    // Caso Right Right
    if (balance < -1 && getBalanceFactorPath(root->right) <= 0)
        return leftRotatePath(root);

    // Caso Right Left
    if (balance < -1 && getBalanceFactorPath(root->right) > 0) {
        root->right = rightRotatePath(root->right);
        return leftRotatePath(root);
    }

    // Retornar el nodo (posiblemente modificado)
    return root;
}

// Function to free memory of AVLPATH tree (solo nodos, sin listas)
void freePath_AVL(struct AVLPATH* root)
{
    if (root != NULL) {
        freePath_AVL(root->left);
        freePath_AVL(root->right);
        free(root);
    }
}



#endif // AVLPATH_H