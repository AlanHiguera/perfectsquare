//estructura para la fila bivariada
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <math.h>
#include "listaEnlazada.h"
#include "avlpath.h"

#include <stdbool.h>
#include <gmp.h>  // ✅ Librería para números grandes


unsigned int lenalc = 0;
unsigned char flag = '0'; //flag que indica si la lista de alcanzabilidad se puede permutar o no


struct Node {
    unsigned int valor;
    unsigned char tipo; //0 para pivote 1 para complementario
    struct Node *next;
};

struct Node *insertar(struct Node *p, unsigned int x, unsigned char y) {//Función de manipulación de lista en la cual se guardarán los nodos visitados.
    struct Node *q, *l;
    if (p == NULL) { // p is an empty list
        q = calloc(1, sizeof(struct Node));
        q->valor = x;
        q->tipo = y;
        q->next = NULL;
        p = q;
    } else {
        q = calloc(1, sizeof(struct Node));
        q->valor = x;
        q->tipo = y;
        q->next = NULL;
        l = p;
        while (l->next != NULL)
            l = l->next;
        l->next = q;
    }
    return p;
}

struct Node *DeleteFirst(struct Node *p){
    struct Node *q;
    if (p != NULL) {
        q = p->next;
        free(p);
        return q;
    }
    return NULL;
}

struct Node *DeleteLast(struct Node *p){
    struct Node *q;
    struct Node *head = p; // Guardar referencia al primer nodo

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


struct Node *KillAll(struct Node *p){
    while (p != NULL)
        p = DeleteFirst(p);
    return NULL;
}

void PrintList(struct Node *p){
    while (p != NULL) {
        if(p->tipo == '0')
            printf("\n [ %u, %c]",p->valor, p->tipo);
        else
            printf("[ %u, %c]",p->valor, p->tipo);
        p = p->next;
    }
    printf("\n\n");
}

//Ahora crearemos otro tipode lista para la lista auxiliar que llevará tanto el valor como un unsgined int para la posicion
//en la lista de alcanzabilidad

//-----------------------------------Node tipo Registro: listaCamino y listaaux seran de este tipo------------------------------//
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
//------------------------------Pila------------------------------//
//Esta pila va manejar un struct que va tener el valor, el tipo(para saber si ya fue revisado) y la posicion en lista alcanzabilidad.

struct ElementoPila {
    unsigned int valor;
    unsigned char tipo; //0 para no revisado, 1 para revisado
    unsigned int posicion; //posicion en la lista de alcanzabilidad
};

typedef struct {
    struct ElementoPila *arr;  
    int top;
    int maxSize;              
} Stack;

void initialize(Stack *stack, unsigned int n){
    stack->maxSize = n;  
    stack->arr = calloc(stack->maxSize, sizeof(struct ElementoPila));
    
    /*if (stack->arr == NULL) {
        printf("Error: No se pudo asignar memoria para la pila\n");
        stack->maxSize = 0;
        stack->top = -1;
        return;
    }*/
    
    stack->top = -1;
    printf("Pila inicializada con capacidad para %d elementos\n", stack->maxSize);
}

unsigned char isFull(Stack *stack){
    if(stack->top == stack->maxSize - 1)
        return '1';
    else        
        return '0';  
}

unsigned char isEmpty(Stack *stack){
    if(stack->top == -1)
        return '1';
    else
        return '0';  
}

void push(Stack *stack, unsigned char tipo, unsigned int posicion) {
    if (isFull(stack)=='1') {
        return;
    }

    struct ElementoPila nuevoElemento;
    nuevoElemento.tipo = tipo;
    nuevoElemento.posicion = posicion;

    stack->top = stack->top + 1;
    stack->arr[stack->top] = nuevoElemento;
}

struct ElementoPila pop(Stack *stack){
    struct ElementoPila elementoaux;
    elementoaux.tipo = '0'; 
    elementoaux.posicion = 0; 
    
    if (isEmpty(stack)=='1'){
        printf("Stack Underflow\n");
        return elementoaux;
    }

    elementoaux = stack->arr[stack->top];
    stack->top = stack->top - 1;
    return elementoaux;
}

void llenarPila(Stack *pila, struct L_enlazada *listaaux){
    while(listaaux != NULL){
        //printf("Pushing posicion %u onto stack\n", listaaux->posicion);
        push(pila, '0', listaaux->posicion);
        listaaux = listaaux->next;
    }
}

void PrintStack(Stack *pila){
    struct ElementoPila elemento;
    for(int i = pila->top; i >= 0; i--) {
        elemento = *(pila->arr + i);
        printf("Value %u, Tipo %c, Posicion %u\n", elemento.valor, elemento.tipo, elemento.posicion);
    }
    printf("\n");
}

//---------------------------Funciones principales---------------------------//
//funcion que chequea si un numero es cuadrado perfecto
unsigned char is_perfect_square(unsigned int x){

    unsigned int i;
    if(x%4!=0 && x%4!=1){ // los cuadrados perfectos solo pueden ser congruentes con 0 o 1 modulo 4
        return '0';
    }

    for(i = 0; i*i <= x; i = i + 1){
        if(i*i == x)
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
        Alcance[i] = NULL; // Inicializar la lista
        
        // Buscamos los valores que forman cuadrado perfecto con el elemento i
        for(j = 0; j < n; j = j + 1){
            if(i != j && is_perfect_square(*(arrayEntrada + i) + *(arrayEntrada + j))=='1'){//si no es el mismo elemento y si la suma es cuadrado perfecto
                //printf("cuadrado perfecto encontrado entre %u y %u\n", arrayEntrada[i], arrayEntrada[j]);
                Alcance[i] = insertaLista(*(Alcance+i), j); //inserto el VALOR del complemento
                lenalc = lenalc + 1;
                imposiblepermutar = '1';
            }
        }//si no tiene complementos, habrá un pivote seguido de otro. Asi, detectaremos que no hay solucion.
        if(imposiblepermutar=='0'){
            flag = '1'; //la lista no se puede permutar porque hay un elemento que no forma cuadrado perfecto con ningun otro
            return;
        }
    }
}



//Llenar alcance, vamos a tomar un valor y ver cuales son sus complementarios y echarlos a una lista
struct L_enlazada *llenarAlcance(struct L_enlazada *listaaux, struct L_enlazada *Alcance[], unsigned int posicion, struct AVLPATH *Camino_tree){
    //Con el valor dado, buscamos en el arbol cual es la lista y la guardamos en aux
    //Dado el alcance tendremos que verificar en el arbol camino que estos elementos no esten ahi
    struct L_enlazada *aux = NULL;
    //printf("Llenando alcance para posicion %u\n", posicion);

    aux = Alcance[posicion];
    //buscamos que no esten en el arbol camino
    while(aux != NULL){
        if(searchPath(Camino_tree, aux->posicion) == '0'){ //si no existe en el arbol camino
            listaaux = insertaLista(listaaux, aux->posicion);
        }
        aux = aux->next;
    }
    // No eliminamos aux porque solo es un puntero de recorrido
    //printf("Lista de alcance auxiliar:\n");
    //PrintLista(listaaux);
    return listaaux;
}


//Revisa si el input ya es un camino valido, asi descontamos un camino de cont
unsigned char RevisaInput(unsigned int arrayEntrada[], unsigned int n){
    unsigned int i;
    unsigned char valido;
    valido = '1';
    for(i = 0; i < n-1; i = i + 1){
        //chequeamos los sumandos
        printf("Chequeando si %u + %u es cuadrado perfecto\n", (*(arrayEntrada + i)), (*(arrayEntrada + i + 1)));
        if(is_perfect_square((*(arrayEntrada + i)) + (*(arrayEntrada + i + 1)))=='0'){
            valido = '0';//si no es perfect square entonces no es valido
            return valido;
        }
    }
    return valido;
}

void destroyStack(Stack *stack) {
    if (stack->arr != NULL) {
        free(stack->arr);
        stack->arr = NULL;
    }
    stack->maxSize = 0;
    stack->top = -1;
}

int main(int argc, char *argv[]){
    //leer los elemetentos de la lista de entrada. Los guardamos en un array estatico.
    struct L_enlazada *listaaux = NULL;
    struct L_enlazada *listaAlcanzabilidad = NULL;
    struct ElementoPila elementoPilaAux;
    struct ElementoPila verificador;
    Stack pila;

    struct AVLPATH *Camino_tree = NULL;

    char *str;
    unsigned int len;
    unsigned char init;
    unsigned int seguimiento;
    unsigned int posicion;
    unsigned int n;
    mpz_t cont;

    posicion = 0;
    seguimiento = 0;
    init = '1';
    len = 0; 
    mpz_init(cont);
    mpz_set_ui(cont, 0);
    
    n = atoi(argv[1]);
    unsigned int arrayEntrada[n];
    initialize(&pila, n*n);
    ReadData(arrayEntrada, n);

    //definimos array de listas alcance que va a contener para cada posición i los valores alcanzables desde arrayEntrada[i]
    struct L_enlazada *Alcance[n];
    
    // Inicializamos el array
    for(unsigned int i = 0; i < n; i = i + 1) {
        *(Alcance + i) = NULL;
    }
    
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

        push(&pila, '0', i);

        while(isEmpty(&pila) == '0' || init =='0'){
            elementoPilaAux = pop(&pila);//quitamos de la pila

            Camino_tree = insertPath_AVL(Camino_tree, elementoPilaAux.posicion); //ingresamos al camino
            len = len +1;

            listaaux = llenarAlcance(listaaux, Alcance, elementoPilaAux.posicion, Camino_tree); //calculamos el alcance y lo guardamos en lista auxiliar

            push(&pila, '1', elementoPilaAux.posicion); //vuelvo a poner k en la pila para marcarlo como revisado dsp


            if(listaaux != NULL){//llenamos la pila con el alcance
                llenarPila(&pila, listaaux);
                listaaux = KillAllLista(listaaux);

            }//verificamos si la listaaux es vacía, significa que llegamos al final de una rama de computo.
            else if(listaaux == NULL && elementoPilaAux.tipo=='0'){
                elementoPilaAux.tipo = '1'; 
                if(len==n){//chequamos el largo del camino para saber si es valido o no      
                    mpz_add_ui(cont, cont, 1);

                }
                while(isEmpty(&pila)=='0'){//desoues de verificar borramos elementos de la pila y del camino hasta encontrar la sgte rama de computo
                    verificador = pop(&pila);
                    if(verificador.tipo == '0') {
                        push(&pila, '0', verificador.posicion);
                        break;
                    }
                    else{
                        Camino_tree = DeletePath(Camino_tree, verificador.posicion);
                        len = len - 1;
                    }
                }
            }

            init = '1';
            seguimiento = seguimiento + 1;

        }
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecucion: %f segundos\n", cpu_time_used);
    freePath_AVL(Camino_tree);
    for(int i = 0; i < n; i = i + 1){
        KillAllLista(*(Alcance + i));
    }

    destroyStack(&pila);

    //Revisamos si tenemos que restar 1 por si el input ya viene como un camino valido
    if(RevisaInput(arrayEntrada, n)=='1'){
        mpz_sub_ui(cont, cont, 1); 
    }

    printf("Cantidad de caminos encontrados: ");
    
    str = mpz_get_str(NULL, 10, cont);
    printf("%s\n", str);
    free(str);
    mpz_clear(cont);
    return 0;
}