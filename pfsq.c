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
#include <math.h>
//PILA


#include <stdbool.h>

// Define the maximim capacity of the stack
#define MAX_SIZE 100
unsigned int lenalc = 0;
unsigned char flag = '0'; //flag que indica si la lista de alcanzabilidad se puede permutar o no
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
    struct ElementoPila arr[MAX_SIZE];  // Solo valores, no punteros
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

void push(Stack *stack, unsigned int valor, unsigned char tipo, unsigned int posicion) {
    if (isFull(stack)=='1') {
        //printf("Stack Overflow\n");
        return;
    }

    struct ElementoPila nuevoElemento;
    
    nuevoElemento.valor = valor;
    nuevoElemento.tipo = tipo;
    nuevoElemento.posicion = posicion;

    stack->top++;
    stack->arr[stack->top] = nuevoElemento;
}

struct ElementoPila pop(Stack *stack) {
    struct ElementoPila elemento;
    if (isEmpty(stack)=='1') {
        printf("Stack Underflow\n");
        return elemento;
    }
    elemento = stack->arr[stack->top];
    stack->top--;
    return elemento;
}

void llenarPila(Stack *pila, struct NodeReg *listaaux, struct NodeReg *listaCamino, unsigned int arrayEntrada[], unsigned int n){
    while(listaaux != NULL){
        push(pila, listaaux->valor, '0', listaaux->posicion);
        listaaux = listaaux->next;
    }
}

void PrintStack(Stack *pila){

    Stack *p = pila;
    struct ElementoPila elemento;
    for(int i = p->top; i >= 0; i--) {
        elemento = p->arr[i];
        printf("Value %u, Tipo %c, Posicion %u\n", elemento.valor, elemento.tipo, elemento.posicion);
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
struct Node *pares_perfectos(struct Node *p, unsigned int arrayEntrada[], unsigned int n){
    unsigned int i;
    unsigned int j;
    unsigned char imposiblepermutar;
    for(i = 0; i < n; i = i + 1){
        p = insertar(p, arrayEntrada[i], '0');//inserto el pivote
        lenalc = lenalc + 1;
        imposiblepermutar = '0';
        for(j = 0; j < n; j = j + 1){
            if(i != j && is_perfect_square(arrayEntrada[i] + arrayEntrada[j])=='1'){//si no es el mismo elemento y si la suma es cuadrado perfecto
                //printf("cuadrado perfecto encontrado entre %u y %u\n", arrayEntrada[i], arrayEntrada[j]);
                p = insertar(p, arrayEntrada[j], '1');//inserto el complemento
                lenalc = lenalc + 1;
                imposiblepermutar = '1';
            }
        }//si no tiene complementos, habrá un pivote seguido de otro. Asi, detectaremos que no hay solucion.
        if(imposiblepermutar=='0'){
            flag = '1'; //la lista no se puede permutar porque hay un elemento que no forma cuadrado perfecto con ningun otro
            return p;
        }
    }
    return p;
}

//Apariciones dirá si el valor que estamos intentando agregar al camino ya apareció la cantidad máxima de veces
unsigned char apariciones(unsigned int valor, unsigned int arrayEntrada[], unsigned int n, struct NodeReg *listaCamino){
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
unsigned char VerificarCamino(struct NodeReg *listaCamino, unsigned int arrayEntrada[], unsigned int n){
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

//Verifica si un valor esta o no en la lista
unsigned char Estaen(struct NodeReg *listaCamino, unsigned int posicion){
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
struct NodeReg *llenarAlcance(struct NodeReg *listaaux, struct Node *listaAlcanzabilidad, unsigned int valor, struct NodeReg *listaCamino, unsigned int arrayEntrada[], unsigned int n, unsigned int len){
    unsigned int posicion;
    posicion = 0;
    while(listaAlcanzabilidad != NULL){
        if(listaAlcanzabilidad->valor == valor && listaAlcanzabilidad->tipo == '0'){
            listaAlcanzabilidad = listaAlcanzabilidad->next;
            posicion = posicion + 1;
            while(listaAlcanzabilidad != NULL && listaAlcanzabilidad->tipo == '1'){
                printf("posicion: %u \n", posicion);
                if(apariciones(listaAlcanzabilidad->valor, arrayEntrada, n, listaCamino)=='0' && Estaen(listaCamino, posicion)=='0'){ 
                    listaaux = insertaReg(listaaux, listaAlcanzabilidad->valor, posicion);
                    //printf("inserta en lista aux el valor %u de la posicion: %u\n", listaAlcanzabilidad->valor, posicion);
                }
                
                listaAlcanzabilidad = listaAlcanzabilidad->next;
                posicion = posicion + 1;
            }
            if(listaAlcanzabilidad == NULL || listaAlcanzabilidad->tipo == '0'){
                //printf("Lista de alcance:\n");
                //PrintList(listaaux);
                return listaaux;
            }
        }
        listaAlcanzabilidad = listaAlcanzabilidad->next;
        posicion = posicion + 1;
    }
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

int main(int argc, char *argv[]){
    //leer los elemetentos de la lista de entrada. Los guardamos en un array estatico.
    struct NodeReg *listaaux = NULL;
    struct NodeReg *listaCamino = NULL;
    struct Node *listaAlcanzabilidad = NULL;
    struct ElementoPila elementoPilaAux;
    struct ElementoPila verificador;
    Stack pila;

    unsigned int len;
    unsigned char init;
    unsigned int seguimiento;
    unsigned int posicion;
    unsigned int cont;
    unsigned int n;
    unsigned char op;


    posicion = 0;
    seguimiento = 0;
    init = '1';
    len = 0; 
    cont = 0;
    op = argv[2][0];
    initialize(&pila); 

    n = atoi(argv[1]);
    unsigned int arrayEntrada[n];
    ReadData(arrayEntrada, n);

    //printf("array entrada:");
    //imprimimos el array para ver si esta todo bien
    //for (int i = 0; i < n; i = i + 1){
     //   printf("%u", arrayEntrada[i]);
    //}
    //printf("\n");

    //revisamos si el input es camino valido


    //Examinamos el array y vemos cuales son los pares que hacen cuadrado perfecto.
    
    listaAlcanzabilidad = pares_perfectos(listaAlcanzabilidad, arrayEntrada, n);

    printf("Lista de valores alcanzables:\n");
    PrintList(listaAlcanzabilidad);

    //printf("lenalc = %u\n", lenalc);

    if(flag == '1'){
        printf("Hay un elemento que no forma cuadrado perfecto con ningun otro.\n");
        printf("Cantidad de caminos encontrados: 0\n");
        return 0;
    }
    if(flag == '0' && lenalc==n*n && op == '0'){//Ejecutamos el computo de factorial
        //peor caso
        cont = cont + factorialiterativo(n);
    }

    if(flag == '0' && ((lenalc == n*n && op == '1') || lenalc!=n*n)){ //Ejecutamos el algoritmo de pila
        for (unsigned int i = 0; i < n; i = i + 1){
        //printf("Inicia ciclo for elemento = %u\n", arrayEntrada[i]);
        posicion = posicionDeValor(arrayEntrada[i], listaAlcanzabilidad);
        push(&pila, arrayEntrada[i], '0', posicion);
        //printf("Pila en la itracion i: %u\n", arrayEntrada[i]);

        while(isEmpty(&pila) == '0' || init =='0'){
            //printf("seguimiento %u\n", seguimiento);
            elementoPilaAux = pop(&pila);
            //printf("Nodo k sacado de la pila: %u\n", k_valor);

            listaCamino = insertaReg(listaCamino, elementoPilaAux.valor, elementoPilaAux.posicion);
            len = len +1;
            
            //PrintListReg(listaCamino);
            //llenar el alcance desde k
            listaaux = llenarAlcance(listaaux, listaAlcanzabilidad, elementoPilaAux.valor, listaCamino, arrayEntrada, n, len);
            //printf("Lista de alcance:------------\n");
            //PrintListReg(listaaux);

            push(&pila, elementoPilaAux.valor, '1', elementoPilaAux.posicion); //vuelvo a poner k en la pila para marcarlo como revisado dsp
            //printf("Lista camino actual:\n");
            //PrintList(listaCamino);

            if(listaaux != NULL){
                //printf(" listaaux no es nula\n");
                llenarPila(&pila, listaaux, listaCamino, arrayEntrada, n);
                //printf("pila despues de llenar pila:\n");
                listaaux = KillAllReg(listaaux);
                //PrintStack(&pila);
            }

            else if(listaaux == NULL && elementoPilaAux.tipo=='0'){
                //printf("Pila antes de verificar camino:\n");
                //PrintStack(&pila);
                elementoPilaAux.tipo = '1'; 
                if(len==n /*&& VerificarCamino(listaCamino, arrayEntrada, n)=='1'*/){
                    //printf("llego a len = n\n");

                    cont = cont + 1;//imprime camino y verifica len y la solucion
                    printf("------------------Camino verificado-----------------\n");
                    PrintListReg(listaCamino);
                    printf("-----------------------------------\n");
                }
                while(isEmpty(&pila)=='0'){
                    verificador = pop(&pila);
                    //printf("valor quitado: %u\n", popvaloraux);
                    if(verificador.tipo == '0') {
                        //printf("valor: %u es de tipo %c \n", popvaloraux, poptipoaux);
                        push(&pila, verificador.valor, '0', verificador.posicion); // Volver a poner si no es '1'
                        break;
                    }
                    else{
                        //printf("se quita el valor: %u de la lista camino \n", popvaloraux);
                        listaCamino = DeleteLastReg(listaCamino);
                        //printf("Lista camino despues de eliminar:\n");
                        //PrintList(listaCamino);
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
    KillAllReg(listaCamino);
    KillAll(listaAlcanzabilidad);
    //Revisamos si tenemos que restar 1 por si el input ya viene como un camino valido
    if(RevisaInput(arrayEntrada, n)=='1'){
        cont = cont - 1;
    }
    printf("Cantidad de caminos encontrados: %u\n", cont);
    return 0;
}
