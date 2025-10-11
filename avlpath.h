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