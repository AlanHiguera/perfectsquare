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

#define unsigned int cont 0;

struct NodePila {
    struct Node *nodo;
    struct NodePila *next;
};

struct NodePila *Push(struct NodePila *p, struct Node *nodo) {
    struct NodePila *q;
    q = calloc(1, sizeof(struct NodePila));
    q->nodo = nodo;
    q->next = p;
    return q;
}

struct NodePila *Pop(struct NodePila *p) {
    struct NodePila *q;
    if (p != NULL) {
        q = p->next;
        free(p);
        return q;
    }
    return NULL;
}

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

    if(p->next == NULL){//si solo hay un elemento
        free(p);
        return NULL;
    }
    while (p->next->next != NULL){
        if(p->next == NULL){
            free(p->next->next);
            p->next = NULL;
            return p;
        }
        p = p->next;
    }
    return NULL;
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

unsigned char is_perfect_square(unsigned int x){//funcion que chequea si un numero es cuadrado perfecto
    unsigned int i;
    for(i = 0; i*i <= x; i = i + 1){
        if(i*i == x)
            return 1;
    }
    return 0;
}

void ReadData(unsigned int arrayEntrada[], unsigned int n){//lectura de input
    for (int i = 0; i <n; i = i + 1)
        scanf("%u", &arrayEntrada[i]);
}

struct Node *pares_perfectos(struct Node *p, unsigned int arrayEntrada[], unsigned int n){//funcion que genera la lista de valores alcanzables
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

unsigned char detectar_elemento_sin_complemento(struct Node *p){//funcion que detecta si hay un pivote sin complementos
    while(p != NULL){
        if(p->tipo == '0'){
            if(p->next == NULL || p->next->tipo == '0'){//si el siguiente es nulo o es otro pivote
                return 1;//hay un pivote sin complementos
            }
        }
        p = p->next;
    }
    return 0;//no hay pivotes sin complementos
}
//Apariciones dirá si el valor que estamos intentando agregar al camino ya apareció la cantidad máxima de veces
unsigned char apariciones(struct Node *listaCamino, unsigned int valor, unsigned int arrayEntrada[], unsigned int n){
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
        return '0'; //puedo agregar
    }
    else{
        return '1'; //no puedo agregar
    }
}

unsigned int alcance(struct Node *listaCamino, unsigned int valor, unsigned int arrayEntrada[], unsigned int n){//funcion que chequea si un valor ya esta en el camino
    unsigned int contador, flag; // hay que agregar la revision del camino para respetar la cantidad de elementos que hay en la lista original de input
    contador = 0;
    flag = 0;
    while(listaCamino != NULL){
        if(listaCamino->valor == valor && listaCamino->tipo == '0'){
            while(listaCamino->tipo == '1'){
                if(apariciones(listaCamino, listaCamino->valor, arrayEntrada, n)=='0'){
                    contador = contador + 1;
                }
                listaCamino = listaCamino->next;
            }
            break;
        }
    }
    return contador;
}

//Verificar camino: necesitamos ver que el largo del camino sea n, y además que sea distinto del input del programa
unsigned char VerificarCamino(struct Node *listaCamino, unsigned int arrayEntrada[], unsigned int n){
    unsigned int contador;
    unsigned char esDistinto; //0 si es igual, 1 si es distinto
    esDistinto = '0';
    contador = 0;

    while(listaCamino != NULL){
        contador = contador + 1;

        if(listaCamino->valor != arrayEntrada[contador]){
            esDistinto = '0';
        }
        listaCamino = listaCamino->next;
    }
    if(contador == n && esDistinto=='0'){
        return 1; //camino valido
    }
    else{
        return 0; //camino no valido
    }
}

//Llenar alcance, vamos a tomar un valor y ver cuales son sus complementarios y echarlos a una lista
struct Node *llenarAlcance(struct Node *listaaux, struct Node *listaAlcanzabilidad, unsigned int valor){
    while(listaAlcanzabilidad != NULL){
        if(listaAlcanzabilidad->valor == valor && listaAlcanzabilidad->tipo == '0'){
            while(listaAlcanzabilidad->tipo == '1'){
                insertar(listaaux, listaAlcanzabilidad->valor, '1');
                listaAlcanzabilidad = listaAlcanzabilidad->next;
            }
            if(listaAlcanzabilidad->tipo == '0'){
                return listaaux;
        }
    }
    return NULL;
}

void rec_bfs(struct  Node *listaCamino, struct Node *listaAlcanzabilidad, struct Node *listaaux, unsigned int arrayEntrada[], unsigned int n, unsigned int cont){
    //funcion recursiva que explora las permutaciones posibles como si fuesen caminos en un grafo
    if(alcance(listaAlcanzabilidad, listaCamino->valor, arrayEntrada, n)==0){
        if(VerificarCamino(listaCamino, arrayEntrada, n)=='1'){
            cont = cont + 1;
        }
        printf("Camino verificado\n");
        PrintList(listaCamino);
        DeleteLast(listaCamino);
    }
    else{
        listaaux = llenarAlcance(listaAlcanzabilidad, listaCamino->valor);
        while(listaaux != NULL){
            listaCamino = insertar(listaCamino, listaaux->valor, '1');
            listaaux = DeleteFirst(listaaux);
            rec_bfs(listaCamino, listaAlcanzabilidad, listaaux, arrayEntrada, n, cont);
        }
    }
}



int main(int argc, char *argv[]) {
    //leer los elemetentos de la lista de entrada. Los guardamos en un array estatico.
    struct Node *listaaux = NULL;
    struct Node *listaCamino = NULL;
    unsigned int n;
    unsigned int arrayEntrada[n];
    n = atoi(argv[1]);
    ReadData(arrayEntrada, n);
    //imprimimos el array para ver si esta todo bien
    for (int i = 0; i < n; i = i + 1){
        printf("%u", arrayEntrada[i]);
    }
    printf("\n");

    //Examinamos el array y vemos cuales son los pares que hacen cuadrado perfecto.
    struct Node *listaAlcanzabilidad = NULL;
    listaAlcanzabilidad = pares_perfectos(listaAlcanzabilidad, arrayEntrada, n);
    printf("Lista de valores alcanzables:\n");
    PrintList(listaAlcanzabilidad);

    rec_bfs(listaCamino, listaAlcanzabilidad, listaaux, arrayEntrada, n, cont);

    return 0;
}