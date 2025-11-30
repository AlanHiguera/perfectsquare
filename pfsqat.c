//estructura para la fila bivariada
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <math.h>
//#include "listaEnlazada.h"
//#include "avlpath.h"
//#include "Stack.h"
//#include "tipStack.h"
#include <stdbool.h>
#include <gmp.h>  


unsigned char flag = '0'; //flag que indica si la lista de alcanzabilidad se puede permutar o no
//--------------------------Estructuras de datos--------------------------//
//--------------------------Lista enlazada--------------------------//
struct L_enlazada {
    unsigned int posicion;
    struct L_enlazada *next;
};

// Funciones para L_enlazada
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
// Deletea el primero de la lista
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
//---------------------------avlpath--------------------------------//
// Definimos el struct para el nodo del AVLPATH
struct AVLPATH {
    unsigned int key;              
    struct AVLPATH* left;
    struct AVLPATH* right;
    unsigned int height;
};

unsigned int getHeightPath(struct AVLPATH* n){
    if (n == NULL){
        return 0;
    }
    return n->height;
}

struct AVLPATH* createAVLPath(unsigned int key){
    struct AVLPATH* node = (struct AVLPATH*)calloc(1, sizeof(struct AVLPATH));
    node->key = key;
    node->height = 1;
    return node;
}

int maxPath(int a, int b){ 
    if(a > b) {
        return a; 
    } else {
        return b; 
    }
}

// Funcion para obtener el factor de balance del AVLPATH
int getBalanceFactorPath(struct AVLPATH* n){
    if (n == NULL)
        return 0;
    return getHeightPath(n->left) - getHeightPath(n->right);
}

// Rotacion a la derecha
struct AVLPATH* rightRotatePath(struct AVLPATH* y){
    struct AVLPATH* x = y->left;
    struct AVLPATH* T2 = x->right;

    // Se hace la rotacion
    x->right = y;
    y->left = T2;

    // Actualizar alturas
    y->height = maxPath(getHeightPath(y->left), getHeightPath(y->right)) + 1;
    x->height = maxPath(getHeightPath(x->left), getHeightPath(x->right)) + 1;

    return x;
}

// Rotacion a la izquierda
struct AVLPATH* leftRotatePath(struct AVLPATH* x){
    struct AVLPATH* y = x->right;
    struct AVLPATH* T2 = y->left;

    // Se hace la rotacion
    y->left = x;
    x->right = T2;

    // Actualizar alturas
    x->height = maxPath(getHeightPath(x->left), getHeightPath(x->right)) + 1;
    y->height = maxPath(getHeightPath(y->left), getHeightPath(y->right)) + 1;

    return y;
}

// Funcion para insertar una key en el AVLPATH
struct AVLPATH* insertPath_AVL(struct AVLPATH* node, unsigned int key){
    // Se hace la insercion normal de BST
    if (node == NULL)
        return createAVLPath(key);

    if (key < node->key)
        node->left = insertPath_AVL(node->left, key);
    else if (key > node->key)
        node->right = insertPath_AVL(node->right, key);
    else // llaves duplicadas no permitidas
        return node;

    // 2. Actualizar la altura de este nodo ancestro
    node->height = 1 + maxPath(getHeightPath(node->left), getHeightPath(node->right));

    // 3. Obtener el factor de balance de este nodo ancestro para
    // verificar si este nodo se volvió desequilibrado
    int balance = getBalanceFactorPath(node);

    // 4. Si el nodo se vuelve desequilibrado, entonces hay 4 casos

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

    // Retornamos el nodo
    return node;
}

// Funcion para realizar un recorrido inorder del arbol AVLPATH
void inOrderPath(struct AVLPATH* root){
    if (root != NULL) {
        inOrderPath(root->left);
        printf("%u ", root->key);
        inOrderPath(root->right);
    }
}

// Funcion para buscar una key en el arbol AVLPATH
unsigned char searchPath(struct AVLPATH* root, unsigned int key){
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

// Funcion auxiliar para encontrar el nodo con valor minimo (mas a la izquierda)
struct AVLPATH* findMinPath(struct AVLPATH* node)
{
    while (node->left != NULL)
        node = node->left;
    return node;
}

struct AVLPATH* DeletePath(struct AVLPATH* root, unsigned int key){
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
    if (root == NULL){
        return root;
    }
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
    if (balance < -1 && getBalanceFactorPath(root->right) <= 0){
        return leftRotatePath(root);
    }
    // Caso Right Left
    if (balance < -1 && getBalanceFactorPath(root->right) > 0){
        root->right = rightRotatePath(root->right);
        return leftRotatePath(root);
    }

    // Retornar el nodo (posiblemente modificado)
    return root;
}

// Funcion para liberar la memoria del arbol AVLPATH
void freePath_AVL(struct AVLPATH* root){
    if(root != NULL){
        freePath_AVL(root->left);
        freePath_AVL(root->right);
        free(root);
    }
}
//---------------------------tipStack--------------------------------//

struct tipStack {
    unsigned int top;
    unsigned int capacity;
    unsigned int* array;
};

unsigned int tipIsEmpty(struct tipStack* stack) {
    if(stack->top == 0) {
        return '0';
    } else {
        return '1';
    }
}

// Function to create a stack of given capacity.
struct tipStack* tipCreateStack(unsigned int capacity) {
    struct tipStack* stack = (struct tipStack*)calloc(1, sizeof(struct tipStack));

    stack->capacity = capacity;
    stack->top = 0;

    stack->array = (unsigned int*)calloc(capacity, sizeof(unsigned int));
    return stack;
}

unsigned int tipIsFull(struct tipStack* stack) {
    if(stack->top == stack->capacity) {
        return '1';
    } else {
        return '0';
    }
}

void tipPush(struct tipStack* stack, unsigned int item) {
    if (tipIsFull(stack)=='1'){
        return;
    }
    stack->array[stack->top] = item;
    stack->top = stack->top + 1;
}

unsigned int tipPop(struct tipStack* stack) {
    if(tipIsEmpty(stack)!='0'){
        stack->top = stack->top - 1;
        return stack->array[stack->top];
    }
}


void tipFreeStack(struct tipStack* stack) {
    if(stack->array != NULL) {
        free(stack->array);
    }
    
    // Liberar la estructura del stack
    free(stack);
}

void tipPrintStack(struct tipStack* stack) {
    if(stack == NULL || tipIsEmpty(stack) == '0') {
        return;
    }
    
    // Recorremos desde el tope hacia abajo sin modificar la pila
    for(int i = stack->top - 1; i >= 0; i = i - 1) {
    }
    printf("\n");
}

//---------------------------Stack--------------------------------//
struct ElementoPila{
    unsigned char tipo; //0 para no revisado, 1 para revisado
    unsigned int posicion; //posicion en la lista de alcanzabilidad
};
// definimos la estructura de la pila
struct Stack{
    unsigned int top;
    unsigned int capacity;
    struct ElementoPila* array;
};
// Funcion para chequear si la pila esta vacia
unsigned char isEmpty(struct Stack* stack){
    if(stack->top == 0) {
        return '0';
    } else {
        return '1';
    }
}
// Funcion para crear una pila de capacidad dada
struct Stack* createStack(unsigned int capacity){
    struct Stack* stack = (struct Stack*)calloc(1, sizeof(struct Stack));

    stack->capacity = capacity;
    stack->top = 0;

    stack->array = (struct ElementoPila*)calloc(capacity, sizeof(struct ElementoPila));
    return stack;
}

// Funcion para chequear si la pila esta llena
unsigned char isFull(struct Stack* stack){
    if(stack->top == stack->capacity) {
        return '1';
    } else {
        return '0';
    }
}
// Funcion para agregar un elemento a la pila
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
//Funcion para quitar un elemento de la pila
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
// Funcion para llenar la pila con los elementos de la tipStack auxiliar(contiene el alcance temporal)
void llenarPila(struct Stack *pila, struct tipStack *tipstackaux){
    unsigned int posicion;
    posicion = 0;
    while(tipIsEmpty(tipstackaux) == '1'){
        posicion = tipPop(tipstackaux);
        push(pila, '0', posicion);
    }
}
// Funcion para liberar la memoria de la pila
void FreeStack(struct Stack* stack){
    if(stack->array != NULL) {
        free(stack->array);
    }
    

    free(stack);
}

// Funcion para imprimir la pila
void PrintStack(struct Stack *pila){
    struct ElementoPila elemento;
    
    if(pila == NULL || isEmpty(pila) == '0') {
        return;
    }
    // Recorremos desde el tope hacia abajo sin modificar la pila
    for(int i = pila->top; i >= 1; i = i - 1){
        elemento = pila->array[i];
        printf("Tipo: %c, Posicion: %u\n", elemento.tipo, elemento.posicion);
    }
    printf("\n");
}
//---------------------------Funciones principales---------------------------//
//funcion que chequea si un numero es cuadrado perfecto
unsigned char is_perfect_square(unsigned int x){

    unsigned int i;
    i = sqrt(x);

    if(i*i == x){
        return '1';
    }
    
    if(((i+1)*(i+1)) == x){ // solo por si hay error de redondeo
        return '1';
    }
    
    return '0';
}

//lectura de input
void ReadData(unsigned int arrayEntrada[], unsigned int n){
    for (int i = 0; i <n; i = i + 1){
        scanf("%u", &arrayEntrada[i]);
    }
}

//funcion que genera la lista de valores alcanzables
void pares_perfectos(struct L_enlazada *Alcance[], unsigned int arrayEntrada[], unsigned int n){
    unsigned int i;
    unsigned int j;
    unsigned char imposiblepermutar;
    
    for(i = 0; i < n; i = i + 1){
        imposiblepermutar = '0';
        *(Alcance + i) = NULL;
        
        // Buscamos los valores que forman cuadrado perfecto con el elemento i
        for(j = 0; j < n; j = j + 1){
            if(i != j && is_perfect_square(*(arrayEntrada + i) + *(arrayEntrada + j))=='1'){//si no es el mismo elemento y si la suma es cuadrado perfecto
                //printf("cuadrado perfecto encontrado entre %u y %u\n", arrayEntrada[i], arrayEntrada[j]);
                *(Alcance + i) = insertaLista(*(Alcance+i), j); //inserto el VALOR del complemento
                imposiblepermutar = '1';
            }
        }//si no tiene complementos, habrá un pivote seguido de otro. Asi, detectaremos que no hay solucion.
        if(imposiblepermutar=='0'){
            flag = '1'; //la lista no se puede permutar porque hay un elemento que no forma cuadrado perfecto con ningun otro
            return;
        }
    }
}



//Llenar alcance, vamos a tomar un valor y ver cuales son sus complementos y echarlos a una lista
struct tipStack *llenarAlcance(struct tipStack *stackaux, struct L_enlazada *Alcance[], unsigned int posicion, struct AVLPATH *Camino_tree){
    //Con el valor dado, buscamos en el arbol cual es la lista y la guardamos en aux
    //Dado el alcance tendremos que verificar en el arbol camino que estos elementos no esten ahi
    struct L_enlazada *aux = NULL;

    aux = *(Alcance + posicion);
    //buscamos que no esten en el arbol camino
    while(aux != NULL){
        if(searchPath(Camino_tree, aux->posicion) == '0'){ //si no existe en el arbol camino
            tipPush(stackaux, aux->posicion);
        }
        aux = aux->next;
    }
    return stackaux;
}


//Revisa si el input ya es un camino valido, asi descontamos un camino de cont
unsigned char RevisaInput(unsigned int arrayEntrada[], unsigned int n){
    unsigned int i;
    unsigned char valido;
    valido = '1';
    for(i = 0; i < n-1; i = i + 1){
        if(is_perfect_square((*(arrayEntrada + i)) + (*(arrayEntrada + i + 1)))=='0'){
            valido = '0';//si no es cuadrado perfecto entonces no es valido
            return valido;
        }
    }
    return valido;
}



int main(int argc, char *argv[]){
    //leer los elementos de la lista de entrada. Los guardamos en un array estatico.
    struct tipStack *stackaux;
    struct ElementoPila elementoPilaAux;
    struct ElementoPila verificador;
    struct Stack *pila;

    struct AVLPATH *Camino_tree = NULL;

    char *str;
    unsigned int len;
    unsigned char init;
    unsigned int seguimiento;
    unsigned char modo;
    //unsigned int posicion;
    unsigned int n;
    mpz_t cont;
    
    //posicion = 0;
    seguimiento = 0;
    init = '1';
    len = 0; 
    mpz_init(cont);
    mpz_set_ui(cont, 0);
    if (strcmp(argv[2],"S") == 0)
	    modo = '0';
    if (strcmp(argv[2],"V") == 0)
        modo = '1';
    n = atoi(argv[1]);
    unsigned int arrayEntrada[n];
    ReadData(arrayEntrada, n);
    stackaux = tipCreateStack(n+1);
    pila = createStack(n*n);
    //definimos array de listas alcance que va a contener para cada posición i los valores alcanzables desde arrayEntrada[i]
    struct L_enlazada *Alcance[n];
    
    pares_perfectos(Alcance, arrayEntrada, n);

    if(flag == '1'){
        printf("Hay un elemento que no forma cuadrado perfecto con ningun otro.\n");
        printf("Cantidad de caminos encontrados: 0\n");
        return 0;
    }
    
    //tomamos el tiempo de ejecucion
    clock_t start, end;
    double cpu_time_used;
    start = clock();


    if(flag == '0'){ //Ejecutamos el algoritmo de backtracking
        for (unsigned int i = 0; i < n; i = i + 1){
        push(pila, '0', i);
        while(isEmpty(pila) != '0' || init =='0'){
            elementoPilaAux = pop(pila);//quitamos de la pila
            Camino_tree = insertPath_AVL(Camino_tree, elementoPilaAux.posicion); //ingresamos al camino
            len = len +1;

            stackaux = llenarAlcance(stackaux, Alcance, elementoPilaAux.posicion, Camino_tree); //calculamos el alcance y lo guardamos en lista auxiliar

            push(pila, '1', elementoPilaAux.posicion); //vuelvo a poner k en la pila para marcarlo como revisado dsp

            if(tipIsEmpty(stackaux) != '0'){//llenamos la pila con el alcance
                llenarPila(pila, stackaux);

            }//verificamos si la listaaux es vacía, significa que llegamos al final de una rama de computo.
            else{
                elementoPilaAux.tipo = '1'; 
                if(len==n){//chequamos el largo del camino para saber si es valido o no      
                    mpz_add_ui(cont, cont, 1);
                }
                if(modo=='1'){
                    printf("Camino encontrado: ");
                }
                while(isEmpty(pila)!='0'){//despues de verificar borramos elementos de la pila y del camino hasta encontrar la sgte rama de computo
                    verificador = pop(pila);
                    if(verificador.tipo == '0') {
                        push(pila, '0', verificador.posicion);
                        break;
                    }
                    else{
                        if(modo=='1'){
                            printf("%u ", arrayEntrada[verificador.posicion]);
                        }
                        Camino_tree = DeletePath(Camino_tree, verificador.posicion);
                        len = len - 1;
                    }
                }
                if(modo=='1'){
                  printf("\n");
                }

            }
            init = '1';
            seguimiento = seguimiento + 1;

        }
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo: %f seg\n", cpu_time_used);
    //Limpiamos la memoria que utilizamos
    freePath_AVL(Camino_tree);
    for(int i = 0; i < n; i = i + 1){
        KillAllLista(*(Alcance + i));
    }
    tipFreeStack(stackaux);
    FreeStack(pila);
    //Revisamos si tenemos que restar 1 por si el input ya viene como un camino valido
    if(RevisaInput(arrayEntrada, n)=='1'){
        mpz_sub_ui(cont, cont, 1); 
    }

    printf("Caminos encontrados: ");
    //Por ultimo resolvemos el problema del tipo de dato e imprimimos la respuesta   
    str = mpz_get_str(NULL, 10, cont);
    printf("%s\n", str);
    free(str);
    mpz_clear(cont);
    return 0;
}