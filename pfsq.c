#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <math.h>
#include "tree.h"
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

//------------------------------Pila------------------------------//
//Esta pila va manejar un struct que va tener el valor, el tipo(para saber si ya fue revisado) y la posicion en lista alcanzabilidad.

struct ElementoPila {
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
        push(pila, '0', listaaux->posicion);
        listaaux = listaaux->next;
    }
}

void PrintStack(Stack *pila){
    struct ElementoPila elemento;
    for(int i = pila->top; i >= 0; i--) {
        elemento = pila->arr[i];
        printf("Tipo %c, Posicion %u\n", elemento.tipo, elemento.posicion);
    }
    printf("\n");
}


//---------------------------Funciones principales---------------------------//
//funcion que chequea si un numero es cuadrado perfecto
unsigned char is_perfect_square(unsigned int x){
    unsigned int i;
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
        //veamos qué elementos se leyeron
        printf("Elementos leidos: %u ", arrayEntrada[i]);
    }
}

//funcion que genera la lista de valores alcanzables
struct AVL *pares_perfectos(struct AVL *p, unsigned int arrayEntrada[], unsigned int n){
    unsigned int i;
    unsigned int j;
    unsigned char imposiblepermutar;
    struct L_enlazada *aux = NULL;
    struct L_enlazada *auxCopia = NULL;
    for(i = 0; i < n; i = i + 1){
        lenalc = lenalc + 1;
        imposiblepermutar = '0';
        aux = NULL; // Resetear aux para cada iteración del bucle externo
        for(j = 0; j < n; j = j + 1){
            if(i != j && is_perfect_square(arrayEntrada[i] + arrayEntrada[j])=='1'){//si no es el mismo elemento y si la suma es cuadrado perfecto
                //printf("cuadrado perfecto encontrado entre %u y %u\n", arrayEntrada[i], arrayEntrada[j]);
                //printf("antes de insertar en lista aux\n");
                //printf("i: %u, j: %u\n", i, j);
                aux = insertaLista(aux, j); // Insertar la posición j, no el valor
                //printf("despues de insertar en lista aux\n");
                lenalc = lenalc + 1;
                imposiblepermutar = '1';
            }
        }
        //si no tiene complementos, habrá un pivote seguido de otro. Asi, detectaremos que no hay solucion.
        if(imposiblepermutar=='0'){
            flag = '1'; //la lista no se puede permutar porque hay un elemento que no forma cuadrado perfecto con ningun otro
            return p;
        }
        // Crear una copia de la lista para el árbol AVL
        auxCopia = copiarLista(aux);
        p = insert_AVL(p, i, auxCopia); //inserto el pivote con su copia de complementos
        // Ahora puedo liberar la lista original de forma segura
        aux = KillAllLista(aux);
    }
    return p;
}
/*
//Apariciones dirá si el valor que estamos intentando agregar al camino ya apareció la cantidad máxima de veces
unsigned char apariciones(unsigned int valor, unsigned int arrayEntrada[], unsigned int n, struct L_enlazada *listaCamino){
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

//Verificar camino: necesitamos ver que el largo del camino sea n, y además que sea distinto del input del programa
unsigned char VerificarCamino(struct L_enlazada *listaCamino, unsigned int arrayEntrada[], unsigned int n){
    unsigned int iterador;
    unsigned char flag; // hay que agregar la revision del camino para respetar la cantidad de elementos que hay en la lista original de input
    iterador = 0;
    flag = '0';
    //printf("entra a verificar camino\n");
    while(iterador < n && listaCamino != NULL){
        //printf("iterador: %u\n", iterador);
        if(listaCamino->valor != arrayEntrada[iterador]){
            flag = '1'; //es distinto
        }
        
        //printf("valor en camino %u y valor en input %u\n", listaCamino->valor, arrayEntrada[iterador]);
        iterador = iterador + 1;
        listaCamino = listaCamino->next;
    }
    return flag;
}
*/
//Verifica si un valor esta o no en la lista
unsigned char Estaen(struct L_enlazada *listaCamino, unsigned int posicion){
    while(listaCamino != NULL){
        if(listaCamino->posicion == posicion){
            //printf("La posicion %u ya esta en la lista camino\n", posicion);
            return '1';
        }
        listaCamino = listaCamino->next;
    }
    return '0';
}


//Llenar alcance, vamos a tomar un valor y ver cuales son sus complementarios y echarlos a una lista
struct L_enlazada *llenarAlcance(struct L_enlazada *listaaux, struct AVL *Alcanzabilidad, unsigned int posicion, struct AVLPATH *Camino_tree){
    //Con el valor dado, buscamos en el arbol cual es la lista y la guardamos en aux
    //Dado el alcance tendremos que verificar en el arbol camino que estos elementos no esten ahi
    struct L_enlazada *aux = NULL;
    aux = getNodeList(Alcanzabilidad, posicion);

    //buscamos que no esten en el arbol camino
    while(aux != NULL){
        if(searchPath(Camino_tree, aux->posicion) == '0'){ //si no existe en el arbol camino
            listaaux = insertaLista(listaaux, aux->posicion);
        }
        aux = aux->next;
    }
    KillAllLista(aux);
    return listaaux;
}


unsigned int posicionDeValor(unsigned int valor, struct Node *listaAlcanzabilidad){
    unsigned int posicion;
    posicion = 0;
    while(listaAlcanzabilidad != NULL){
        if(listaAlcanzabilidad->valor == valor && listaAlcanzabilidad->tipo == '0'){
            return posicion;
        }
        posicion = posicion + 1;
        listaAlcanzabilidad = listaAlcanzabilidad->next;
    }
    return posicion; 
}

//Revisa si el input ya es un camino valido, asi descontamos un camino de cont
unsigned char RevisaInput(unsigned int arrayEntrada[], unsigned int n){
    unsigned int i;
    unsigned char valido;
    valido = '1';
    for(i = 0; i < n-1; i = i + 1){
        if(is_perfect_square(arrayEntrada[i] + arrayEntrada[i+1])=='0'){
            valido = '0';//si no es perfect square entonces no es valido
            return valido;
        }
    }
    return valido;
}

unsigned int factorialiterativo(unsigned int j){
    unsigned int resultado;
    resultado = 1;
    for(unsigned int i = 2; i <= j; i = i + 1){
        resultado = resultado * i;
    }
    return resultado;
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
    struct L_enlazada *listaCamino = NULL;
    struct ElementoPila elementoPilaAux;
    struct ElementoPila verificador;
    Stack pila;

    struct AVL *alcanzabilidad_tree = NULL; // Árbol AVL para la lista de alcanzabilidad
    struct AVLPATH *Camino_tree = NULL;

    unsigned int len;
    unsigned char init;
    unsigned int seguimiento;
    unsigned int posicion;
    mpz_t cont;  // ✅ Variable para números grandes
    unsigned int n;
    unsigned char op;

    posicion = 0;
    seguimiento = 0;
    init = '1';
    len = 0; 
    mpz_init(cont);  // ✅ Inicializar la variable mpz
    op = argv[2][0];
    
    n = atoi(argv[1]);
    unsigned int arrayEntrada[n];
    initialize(&pila, n*n);
    ReadData(arrayEntrada, n);
    
    //veamos el array entrada
    printf("Array de entrada:\n");
    for (unsigned int i = 0; i < n; i = i + 1){
        printf("%u ", arrayEntrada[i]);
    }
    printf("\n");


    alcanzabilidad_tree = pares_perfectos(alcanzabilidad_tree, arrayEntrada, n);

    printf("Arbol de valores alcanzables(InOrden):\n");
    inOrder(alcanzabilidad_tree);
    printf("lenalc = %u\n", lenalc);
    
    if(flag == '1'){
        printf("Hay un elemento que no forma cuadrado perfecto con ningun otro.\n");
        printf("Cantidad de caminos encontrados: 0\n");
        return 0;
    }
    /*if(flag == '0' && lenalc==n*n && op == '0'){//Ejecutamos el computo de factorial
        //peor caso
        unsigned int factorial_n = factorialiterativo(n);
        mpz_add_ui(cont, cont, factorial_n);  // ✅ Sumar usando GMP
    }*/

    //tomamos el tiempo de ejecucion
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    if(flag == '0' && ((lenalc == n*n && op == '1') || lenalc!=n*n)){ //Ejecutamos el algoritmo de pila
        for (unsigned int i = 0; i < n; i = i + 1){

        push(&pila, '0', i);//SOLO IMPORTA LA POSICION

        while(isEmpty(&pila) == '0' || init =='0'){
            elementoPilaAux = pop(&pila);

            //listaCamino = insertaLista(listaCamino, elementoPilaAux.posicion);
            Camino_tree = insertPath_AVL(Camino_tree, elementoPilaAux.posicion);
            len = len +1;
            
            //PrintListReg(listaCamino);
            //llenar el alcance desde k
            listaaux = llenarAlcance(listaaux, alcanzabilidad_tree, elementoPilaAux.posicion, Camino_tree);

            push(&pila, '1', elementoPilaAux.posicion); //vuelvo a poner k en la pila para marcarlo como revisado dsp


            if(listaaux != NULL){
                llenarPila(&pila, listaaux);
                listaaux = KillAllLista(listaaux);
            }

            else if(listaaux == NULL && elementoPilaAux.tipo=='0'){
                elementoPilaAux.tipo = '1'; 
                if(len==n){      
                    mpz_add_ui(cont, cont, 1);  // ✅ Incrementar usando GMP
                    //printf("------------------Camino verificado-----------------\n");
                    //PrintLista(listaCamino);
                }
                while(isEmpty(&pila)=='0'){
                    verificador = pop(&pila);
                    if(verificador.tipo == '0') {
                        push(&pila, '0', verificador.posicion); // Volver a poner si no es '1'
                        break;
                    }
                    else{
                        //listaCamino = DeleteLastLista(listaCamino);
                        Camino_tree = DeletePath(Camino_tree, verificador.posicion);
                        len = len - 1;
                    }
                }
            }
            //printf("listaCamino------");
            //PrintListReg(listaCamino);
            //PrintStack(&pila);
            init = '1';
            seguimiento = seguimiento + 1;

        }
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecucion: %f segundos\n", cpu_time_used);
    KillAllLista(listaCamino);
    freePath_AVL(Camino_tree);
    free_AVL(alcanzabilidad_tree);

    destroyStack(&pila);
    //Revisamos si tenemos que restar 1 por si el input ya viene como un camino valido
    if(RevisaInput(arrayEntrada, n)=='1'){
        mpz_sub_ui(cont, cont, 1);  // ✅ Restar usando GMP
    }
    
    printf("Cantidad de caminos encontrados: ");
    mpz_out_str(stdout, 10, cont);  // ✅ Imprimir número grande
    printf("\n");
    
    mpz_clear(cont);  // ✅ Liberar memoria de GMP
    return 0;
}
