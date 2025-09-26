/*input: lista

obj1: generar estructura de valores alcanzables.

paso1: tomo cada elemento de la lista como pivote y veo con cuales otros puedo hacer pareja que sumen cuadrado perfecto.
paso2: y anoto así: [x0, x1, x2, ..., xn] luego para el siguiente [y0, x1, x2, ..., xi] Podria hacer una lista grande bivariada, indicando si el elemento es pivote o si es complementar.

ejemplo de lista [x0(piv), x1, x2, ..., xn, y0(piv), x1, x2, ..., xi]

Paso3: si hay un elemento que este en la lista input pero que no esté en la lista de valores alcanzables ACABAR Y RETORNAR solución = 0.
Obj1 concluido

obj2: Revisar todos los caminos. Restricción: Caminos deben ser de largo n y deben respetar la cantidad original de cada elemento lista de input a lo largo del camino(Lista dinamica de restricción).

paso1: Echar a la pila todos los elementos pivote de la lista de valores alcanzables.
paso2: Quito 1, registro sus alcances en una pila, quito 1 de la pila y así voy. Por cada vez quito 1 verifico la restricción(restando las apariciones para no tener que registrar el camino. A principio, guardar camino para hacer testing)
paso3: si el camino es de largo n y pasó todas las restricciones, imprimirlo, sumar 1 a la solución y limpiar la lista del camino.

sino, no hacer nada y seguir con el siguiente elemento de la pila hasta que todas esten vacias.
obj2 concluido: imprimir la cantidad de caminos encontrados.
*/
//estructura para la fila bivariada
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
//PILA


#include <stdbool.h>

// Define the maximim capacity of the stack
#define MAX_SIZE 100

// Define a structure for the stack


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
    
    return head;      // ✅ Retornar el primer nodo
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

typedef struct {
    unsigned int arr[MAX_SIZE];  // Solo valores, no punteros
    unsigned char tipos[MAX_SIZE]; // Array paralelo para los tipos
    int top;        
} Stack;

void initialize(Stack *stack) {
    // Set top index to -1 to indicate an empty stack
    stack->top = -1;  
}
unsigned char isFull(Stack *stack) {
    // If top is MAX_SIZE - 1, the stack is full
    if(stack->top == MAX_SIZE - 1)
        return '1';
    else        
        return '0';  
}
// Function to check if the stack is empty
unsigned char isEmpty(Stack *stack) {
    // If top is -1, the stack is empty
    if(stack->top == -1)
        return '1';
    else
        return '0';  
}

void push(Stack *stack, unsigned int valor, unsigned char tipo) {
    if (isFull(stack)=='1') {
        //printf("Stack Overflow\n");
        return;
    }
    stack->top++;
    stack->arr[stack->top] = valor;
    stack->tipos[stack->top] = tipo;
}

unsigned int pop(Stack *stack, unsigned char *tipo) {
    if (isEmpty(stack)=='1') {
        //printf("Stack Underflow\n");
        return 0;
    }
    unsigned int valor = stack->arr[stack->top];
    *tipo = stack->tipos[stack->top];
    stack->top--;
    return valor;
}


unsigned char is_perfect_square(unsigned int x){//funcion que chequea si un numero es cuadrado perfecto
    unsigned int i;
    for(i = 0; i*i <= x; i = i + 1){
        if(i*i == x)
            return '1';
    }
    return '0';
}

void ReadData(unsigned int arrayEntrada[], unsigned int n){//lectura de input
    for (int i = 0; i <n; i = i + 1){
        scanf("%u", &arrayEntrada[i]);
        //veamos qué elementos se leyeron
        printf("Elementos leidos: %u ", arrayEntrada[i]);
    }
}

struct Node *pares_perfectos(struct Node *p, unsigned int arrayEntrada[], unsigned int n){//funcion que genera la lista de valores alcanzables
    unsigned int i;
    unsigned int j;
    for(i = 0; i < n; i = i + 1){
        p = insertar(p, arrayEntrada[i], '0');//inserto el pivote
        for(j = 0; j < n; j = j + 1){
            if(i != j && is_perfect_square(arrayEntrada[i] + arrayEntrada[j])=='1'){//si no es el mismo elemento y si la suma es cuadrado perfecto
                //printf("cuadrado perfecto encontrado entre %u y %u\n", arrayEntrada[i], arrayEntrada[j]);
                p = insertar(p, arrayEntrada[j], '1');//inserto el complemento
            }
        }//si no tiene complementos, habrá un pivote seguido de otro. Asi, detectaremos que no hay solucion.

    }
    return p;
}

struct Node *pares_perfectosMatriz(struct Node *p, unsigned int arrayEntrada[], unsigned int n){//funcion que genera la lista de valores alcanzables
    unsigned int i;
    unsigned int j;
    for(i = 0; i < n; i = i + 1){
        p = insertar(p, arrayEntrada[i], '0');//inserto el pivote
        for(j = 0; j < n; j = j + 1){
            if(i != j && is_perfect_square(arrayEntrada[i] + arrayEntrada[j]) == 1){//si no es el mismo elemento y si la suma es cuadrado perfecto
                p = insertar(p, arrayEntrada[j], '1');//inserto el complemento
            }
        }//si no tiene complementos, habrá un pivote seguido de otro. Asi, detectaremos que no hay solucion.

    }
    return p;
}

//Apariciones dirá si el valor que estamos intentando agregar al camino ya apareció la cantidad máxima de veces
unsigned char apariciones(unsigned int valor, unsigned int arrayEntrada[], unsigned int n, struct Node *listaCamino){
    //veremos cuantas veces aparece en la lista camino
    //veremos cuantas veces aparece en la lista de entrada
    // CantidadEnCamino < CantidadEnEntrada -> puedo agregar
    unsigned int contadorCamino, contadorEntrada;
    contadorCamino = 0;
    contadorEntrada = 0;
    
    
    while(listaCamino != NULL){
        if(listaCamino->valor == valor){
            contadorCamino = contadorCamino + 1;
        }
        listaCamino = listaCamino->next;
    }

    for(int i = 0; i < n; i = i + 1){
        if(arrayEntrada[i] == valor){
            contadorEntrada = contadorEntrada + 1;
        }
    }

    if(contadorCamino < contadorEntrada){
        //printf("se puede agregar el valor: %u\n ya que aparecen en camino %u veces y en entrada %u veces\n", valor, contadorCamino, contadorEntrada);
        return '0'; //puedo agregar
    }
    else{
        return '1'; //no puedo agregar
    }
}

unsigned int alcance(struct Node *listaAlcanzabilidad, unsigned int valor, unsigned int arrayEntrada[], unsigned int n, struct Node *listaCamino){//funcion que chequea si un valor ya esta en el camino
    printf("entra a alcance\n");
    unsigned int contador, flag; // hay que agregar la revision del camino para respetar la cantidad de elementos que hay en la lista original de input
    contador = 0;
    flag = 0;

    //printf("  alcance del valor: %u \n ", valor);

    while(listaAlcanzabilidad != NULL){
        if(listaAlcanzabilidad->valor == valor && listaAlcanzabilidad->tipo == '0'){
            listaAlcanzabilidad = listaAlcanzabilidad->next;
            while(listaAlcanzabilidad->tipo == '1'){
                //printf("valor a chequear en alcance %u\n", listaAlcanzabilidad->valor);
                if(apariciones(listaAlcanzabilidad->valor, arrayEntrada, n, listaCamino)=='0'){
                    contador = contador + 1;
                    //CHEQUEAR APARICIONES!!!
                }
                listaAlcanzabilidad = listaAlcanzabilidad->next;
            }
            //printf(" contador1 %u\n", contador);
            return contador;
        }
        //printf(" contador2%u\n", contador);
        return contador;
    }
    //printf(" contador3 %u\n", contador);
    return contador;
}

//Verificar camino: necesitamos ver que el largo del camino sea n, y además que sea distinto del input del programa
unsigned char VerificarCamino(struct Node *listaCamino, unsigned int arrayEntrada[], unsigned int n){
    unsigned int contador;
    contador = 0;
    while(listaCamino != NULL){
        contador = contador + 1;

        if(listaCamino->valor != arrayEntrada[contador]){
            return '1'; //es distinto
        }
        listaCamino = listaCamino->next;
    }
    return '0';//igual al input
}

//Llenar alcance, vamos a tomar un valor y ver cuales son sus complementarios y echarlos a una lista
struct Node *llenarAlcance(struct Node *listaaux, struct Node *listaAlcanzabilidad, unsigned int valor, struct Node *listaCamino, unsigned int arrayEntrada[], unsigned int n){
    while(listaAlcanzabilidad != NULL){
        if(listaAlcanzabilidad->valor == valor && listaAlcanzabilidad->tipo == '0'){
            listaAlcanzabilidad = listaAlcanzabilidad->next;
            while(listaAlcanzabilidad != NULL && listaAlcanzabilidad->tipo == '1'){
                //printf("valor a insertar en lista aux %u\n", listaAlcanzabilidad->valor);
                if(apariciones(listaAlcanzabilidad->valor, arrayEntrada, n, listaCamino)=='0'){
                    listaaux = insertar(listaaux, listaAlcanzabilidad->valor, '0');
                }
                listaAlcanzabilidad = listaAlcanzabilidad->next;
            }
            if(listaAlcanzabilidad == NULL || listaAlcanzabilidad->tipo == '0'){
                //printf("Lista de alcance:\n");
                //PrintList(listaaux);
                return listaaux;
            }
        }
        listaAlcanzabilidad = listaAlcanzabilidad->next;  // ✅ También agregar esto
    }
    return listaaux;
}

/*void rec_bfs(struct  Node *listaCamino, struct Node *listaAlcanzabilidad, struct Node *listaaux, unsigned int arrayEntrada[], unsigned int n, unsigned int cont){
    //funcion recursiva que explora las permutaciones posibles como si fuesen caminos en un grafo

    printf("Recursion iniciada\n");
    if(alcance(listaAlcanzabilidad, listaaux->valor, arrayEntrada, n, listaCamino)==0){//el alcance hay que marcarlo, para revisar los demás
        printf("sale de alcance\n");
        if(VerificarCamino(listaCamino, arrayEntrada, n)=='1'){
            cont = cont + 1;
        }
        printf("Camino verificado\n");
        PrintList(listaCamino);
        DeleteLast(listaCamino);
    }
    else{
        listaaux = llenarAlcance(listaaux, listaAlcanzabilidad, listaCamino->valor);
        printf("sale de llenar alcance\n");
        while(listaaux != NULL){
            listaCamino = insertar(listaCamino, listaaux->valor, '1');
            listaaux = DeleteFirst(listaaux);
            rec_bfs(listaCamino, listaAlcanzabilidad, listaaux, arrayEntrada, n, cont);
        }
    }
}
*/

void llenarPila(Stack *pila, struct Node *listaaux, struct Node *listaCamino, unsigned int arrayEntrada[], unsigned int n){
    while(listaaux != NULL){
        push(pila, listaaux->valor, '0');
        listaaux = listaaux->next;
    }
}

void PrintStack(Stack *pila){

    Stack *p = pila;
    printf("Stack contents (top to bottom):\n");
    for(int i = p->top; i >= 0; i--) {
        printf("Value %u, Tipo %c\n", p->arr[i], p->tipos[i]);
    }
    printf("\n");
}
int main(int argc, char *argv[]){
    //leer los elemetentos de la lista de entrada. Los guardamos en un array estatico.
    struct Node *listaaux = NULL;
    struct Node *listaCamino = NULL;
    struct Node *listaAlcanzabilidad = NULL;

    unsigned int cont = 0;
    unsigned int n;
    Stack pila;
    
    initialize(&pila); 

    n = atoi(argv[1]);
    unsigned int arrayEntrada[n];
    ReadData(arrayEntrada, n);
    //imprimimos el array para ver si esta todo bien
    for (int i = 0; i < n; i = i + 1){
        printf("%u", arrayEntrada[i]);
    }
    printf("\n");

    //Examinamos el array y vemos cuales son los pares que hacen cuadrado perfecto.
    
    listaAlcanzabilidad = pares_perfectos(listaAlcanzabilidad, arrayEntrada, n);
    printf("Lista de valores alcanzables:\n");
    PrintList(listaAlcanzabilidad);

    //Inicializamos la lista y el stack con el primer elemento solo para probar


    unsigned int k_valor;
    unsigned char k_tipo;
    
    unsigned int popvaloraux;
    unsigned char poptipoaux;

    unsigned int len;
    unsigned char init;
    unsigned int seguimiento;
    seguimiento = 0;
    init = '1';
    len = 0; 
    for (unsigned int i = 0; i < n; i = i + 1){
        //printf("Inicia ciclo for elemento = %u\n", arrayEntrada[i]);
        push(&pila, arrayEntrada[i], '0');
        while(isEmpty(&pila) == '0' || init =='0'){
            //printf("seguimiento %u\n", seguimiento);
            k_valor = pop(&pila, &k_tipo);
            //printf("Nodo k sacado de la pila: %u\n", k_valor);
            
            listaCamino = insertar(listaCamino, k_valor, '0');
            len = len +1;
            
            //llenar el alcance desde k
            listaaux = llenarAlcance(listaaux, listaAlcanzabilidad, k_valor, listaCamino, arrayEntrada, n);
            
            push(&pila, k_valor, '1'); //vuelvo a poner k en la pila para marcarlo como revisado dsp
            //printf("Lista camino actual:\n");
            //PrintList(listaCamino);

            if(listaaux != NULL){
                //printf(" listaaux no es nula\n");
                llenarPila(&pila, listaaux, listaCamino, arrayEntrada, n);
                //printf("pila despues de llenar pila:\n");
                listaaux = KillAll(listaaux);
                //PrintStack(&pila);
            }

            
            else {//(listaaux == NULL && k_tipo=='0'){
                k_tipo = '1'; 
                if(len==n && VerificarCamino(listaCamino, arrayEntrada, n)=='1'){
                    printf("llego a len = n\n");
                    cont = cont + 1;//imprime camino y verifica len y la solucion
                    printf("------------------Camino verificado-----------------\n");
                    PrintList(listaCamino);
                    printf("------------------Camino verificado-----------------\n");
                }
                while(isEmpty(&pila)=='0'){
                    popvaloraux = pop(&pila, &poptipoaux);
                    //printf("valor quitado: %u\n", popvaloraux);
                    if(poptipoaux == '0') {
                        //printf("valor: %u es de tipo %c \n", popvaloraux, poptipoaux);
                        push(&pila, popvaloraux, '0'); // Volver a poner si no es '1'
                        break;
                    }
                    else{
                        //printf("se quita el valor: %u de la lista camino \n", popvaloraux);
                        listaCamino = DeleteLast(listaCamino);
                        //printf("Lista camino despues de eliminar:\n");
                        //PrintList(listaCamino);
                        len = len - 1;
                    }
                }
            }
            init = '1';
            seguimiento = seguimiento + 1;

        }
    }


    printf("Cantidad de caminos encontrados: %u\n", cont);
    return 0;
}

    //printf("lista alcanzabilidad:\n");
    //printf(" %u\n", listaAlcanzabilidad->valor);
    
    /*
    listaCamino = insertar(listaCamino, listaAlcanzabilidad->valor, '0');//inserto el primer pivote en la lista camino
    printf("Lista camino inicial:\n");
    PrintList(listaCamino);

    listaaux = insertar(listaaux, listaCamino->valor, '0');//inserto el primer pivote en la lista aux
    rec_bfs(listaCamino, listaAlcanzabilidad, listaaux, arrayEntrada, n, cont);
    printf("Cantidad de caminos encontrados: %u\n", cont);
    */
    
