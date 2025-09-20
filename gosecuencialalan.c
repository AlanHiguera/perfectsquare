#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"

#define MASTER  0


struct Node {
    int valor;// pueden ser tanto ficha como posición.
    char marca;// auxilia para saber si una posicion ya fue visitada o a quien le pertenece la celda.
    struct Node *next;
};
struct Node *DeleteFirst(struct Node *p){
    struct Node *q;
    if (p != NULL) {
        q = p->next;
        free(p);
        return q;
    }
    return NULL;
}

struct Node *KillAll(struct Node *p){
    while (p != NULL)
        p = DeleteFirst(p);
    return NULL;
}

void PrintList(struct Node *p, unsigned int dim){
    unsigned int i;
    i = 0;
    while (p != NULL) {
        printf("[ %d, %c]",p->valor, p->marca);
        p = p->next;
        i = i + 1;
        if(i == dim){
            printf("\n");
            i = 0;
        }
    }
    printf("\n\n\n");
}

void PrintListList(struct Node *p){//funcion para imprimir la matriz estirada
    while (p != NULL) {
        printf("[ %d, %c ]",p->valor, p->marca);
        p = p->next;
    }
    printf("\n\n\n");
}

unsigned int esta_en_la_lista(struct Node *p, unsigned int x) {//Funcion auxiliar para lista.
    if (p == NULL)
        return 0;
    else {
        while (p != NULL) {
            if (p->valor == x)
                return 1;
            p = p->next;
        }
        return 0;
    }
}

struct Node *insertar(struct Node *p, unsigned int x, unsigned char y) {//Función de manipulación de lista en la cual se guardarán los nodos visitados.
    struct Node *q, *l;
    if (p == NULL) { // p is an empty list
        q = calloc(1, sizeof(struct Node));
        q->valor = x;
        q->marca = y;
        q->next = NULL;
        p = q;
    } else {
        q = calloc(1, sizeof(struct Node));
        q->valor = x;
        q->marca = y;
        q->next = NULL;
        l = p;
        while (l->next != NULL)
            l = l->next;
        l->next = q;
    }
    return p;
}
void ReadData(int dim, struct Node **p){//lectura de input
    int i;
    char aux[dim];
    for (i = 0; i <dim; i = i + 1){
        scanf("%s", aux);
        printf("aux: %s\n", aux);
        for(int k = 0; k < dim; k++){
            printf("aux[k]: %c\n", aux[k]);
            *p = insertar(*p, aux[k] - '0', 0);
        }
    }
}

unsigned int enesimo_elemento(struct Node *p, unsigned int n){//Función que retorna el enesimo elemento de la lista.
    unsigned int i;
    i = 0;
    while ((p) != NULL) {
        if (i == n)
            return (p)->valor;
        (p)= (p)->next;
        i = i + 1;
    }
    return 0;
}

unsigned int marca_enesimo_elemento(struct Node *p, unsigned int n){//Función que retorna la marca del enesimo elemento de la lista.
    unsigned int i;
    i = 0;
    while ((p) != NULL) {
        if (i == n)
            return (p)->marca;
        (p)= (p)->next;
        i = i + 1;
    }
    return 0;
}
unsigned char hay_elementos_sin_marca(struct Node *p){//Función que chequea si hay elementos sin marcar en la lista.
    while (p != NULL) {
        if (p->marca == '0')
            return 1;
        p = p->next;
    }
    return 0;
}
unsigned int primera_posicion_sin_marca(struct Node *p){//Función que retorna la primera posición sin marcar en la lista.
    unsigned int i;
    i = 0;
    while (p != NULL) {
        printf("marca: %c \n", p->marca);
        if(p->marca == '0'){
            printf("el elemento %d tiene marca %c\n", p->valor, p->marca);
            return p->valor;
        }
        p = p->next;
        i = i + 1;
    }
}

void marcar_posicion(struct Node *p, char v){//Función que marca una posición en la lista.
    unsigned int i;
    i = 0;
    unsigned int contador;
    contador = 0;
    while ((p)!= NULL) {
        if ((p)->marca=='0' && i == 0){
            (p)->marca = v;
            i = 1;
            return;
        }
        contador = contador + 1;
        (p)= (p)->next;
    }
}

void cambiar_marca(struct Node *p, unsigned int x, char v){//Función que cambia la marca de una posición en la lista.
    while ((p)!= NULL) {
        if ((p)->valor == x){
            (p)->marca = v;
            return;
        }
        (p)= (p)->next;
    }
}
struct Node *determinarRegiones(struct Node *p, unsigned int dim){//Funcuón para determinar las regiones en una matriz estirada
    unsigned int i;
    unsigned int elemento;
    unsigned int posicion_inicial;
    unsigned int posicion_sin_marca;
    unsigned int aux;
    unsigned char yasecambio;
    unsigned int q;
    struct Node *regiones = NULL;
    q = 0;

    for(i = 0; i<(dim*dim) ; i = i + 1){//recorremos el tablero 
        elemento = enesimo_elemento(p, i);
        if(elemento == 0 && esta_en_la_lista(regiones, i) == 0){
            printf("i: %d\n", i);
            posicion_inicial = i;
            if(esta_en_la_lista(regiones, i)==0){//Tomamos una celda sin marcar
                regiones = insertar(regiones, i, '0');
            }
            while(hay_elementos_sin_marca(regiones)==1){//mientras haya celdas sin marcar
                posicion_sin_marca = primera_posicion_sin_marca(regiones);
                printf("posicion_sin_marca: %d\n", posicion_sin_marca);
                if(0<=(posicion_sin_marca - dim) && (posicion_sin_marca-dim)<=(dim*dim - 1)){//Seccion condicional para visitar las celdas adyacentes.
                    aux = enesimo_elemento(p, posicion_sin_marca-dim);
                    printf("p->valor: %d\n", aux);
                    if(aux==0 && esta_en_la_lista(regiones, posicion_sin_marca-dim)==0){//si es una celda sin visitar
                        regiones = insertar(regiones, posicion_sin_marca - dim, '0');//vecino de arriba
                    }
                    //Condicionales para ver a quien le pertenece la celda. util para analisis posterior.
                    if(aux==1){
                        cambiar_marca(regiones, posicion_sin_marca, '1');
                        yasecambio = 1;
                    }
                    if(aux==2){
                        cambiar_marca(regiones, posicion_sin_marca, '2');
                        yasecambio = 1;
                    }
                }

                if(0<=(posicion_sin_marca+dim) && posicion_sin_marca+dim<=(dim*dim-1)){//vecino de abajo
                    aux = enesimo_elemento(p, posicion_sin_marca+dim);
                    if(aux==0 && esta_en_la_lista(regiones, posicion_sin_marca+dim)==0 ){
                        printf("p->valor: %d\n", aux);
                        regiones = insertar(regiones, posicion_sin_marca + dim, '0');
                    }
                    if(aux==1){
                        cambiar_marca(regiones, posicion_sin_marca, '1');
                        yasecambio = 1;
                    }
                    if(aux==2){
                        cambiar_marca(regiones, posicion_sin_marca, '2');
                        yasecambio = 1;
                    }
                }
                 if(0<=(posicion_sin_marca + 1) && (posicion_sin_marca +1)/dim==(posicion_sin_marca)/dim && (posicion_sin_marca + 1)<=(dim*dim-1)){//nos cuidamos de los limites
                    aux = enesimo_elemento(p, posicion_sin_marca+1);
                    if(aux==0 && esta_en_la_lista(regiones, posicion_sin_marca+1)==0){
                        printf("p->valor: %d\n", aux);
                        regiones = insertar(regiones, posicion_sin_marca + 1, '0');//vecino derecho, ojo con limites
                    }
                    if(aux==1){
                        cambiar_marca(regiones, posicion_sin_marca, '1');
                        yasecambio = 1;
                    }
                    if(aux==2){
                        cambiar_marca(regiones, posicion_sin_marca, '2');
                        yasecambio = 1;
                    }
                }
                 if(0<=(posicion_sin_marca -1) && (posicion_sin_marca -1)/dim==(posicion_sin_marca)/dim && (posicion_sin_marca -1)<=(dim*dim-1)){
                    aux = enesimo_elemento(p, posicion_sin_marca-1);
                    if(aux==0 && esta_en_la_lista(regiones, posicion_sin_marca-1)==0){
                        printf("p->valor: %d\n", aux);
                        regiones = insertar(regiones, posicion_sin_marca -1, '0');//vecino izquierdo
                    }
                    if(aux==1){
                        cambiar_marca(regiones, posicion_sin_marca, '1');
                        yasecambio = 1;
                    }
                    if(aux==2){
                        cambiar_marca(regiones, posicion_sin_marca, '2');
                        yasecambio = 1;
                    }
                }
                if(yasecambio == 0){
                    marcar_posicion(regiones, 'v');//marca virgen por si la celda no esta adyacente a ninguna ficha
                }
                yasecambio = 0;
                PrintListList(regiones);
                printf("fin de 1 iteracion\n");
                q = q +1;
            }
            regiones = insertar(regiones, -1, '#'); //-1 representa el final de la sublista, asi separo las listas de regiones.}
        }
    }
    PrintList(regiones, dim);
    return regiones;
}
unsigned int contador_de_regiones(struct Node *p){//util para determinar el ganador
    unsigned int contador;
    contador = 0;
    while(p!=NULL){
        if(p->valor == -1)
            contador = contador + 1;
        p = p->next;
    }
    return contador;
}

char determinar_opinion(struct Node *p, unsigned int i){//recorrer la lista y dependiendo de las marcas 'v', '1' o '2' determinar de quien es
    char opinion;
    opinion = 'v';
    unsigned int contador = 0;
    unsigned char bandera = 0;
    while (p != NULL && bandera == 0 && contador <= i) {
        if (p->valor == -1)
            contador = contador +1;

        if (contador == i && p->marca != 'v' && p->marca != '#') {
            if (opinion == 'v') {
                opinion = p->marca; //Primera opinion no neutra
            } 
            if (opinion != p->marca && opinion != 'v') {
                opinion = '3'; //cambiar si hay mas de una opinion en una sublista
                bandera = 1;
            }
        }
        p = p->next;
    }
    return opinion;
}

void cambiar_opiniones(struct Node *p, unsigned int i, char nueva_opinion){//en caso de que haya que cambiar de opinion
    unsigned int contador = 0;
    while (p != NULL && contador <= i) {
        if (p->valor == -1){
            contador = contador +1;
        }
        if (contador == i && p->valor != -1) {
            p->marca = nueva_opinion;
        }
        p = p->next;
    }
}

void asignar_pertenencia(struct Node *regiones){//Utilizamos las funciones anteriores para determinar la pertenencia
    unsigned int cuantas_regiones_hay;
    cuantas_regiones_hay = contador_de_regiones(regiones);
    char opinion;
    for (unsigned int i = 0; i < cuantas_regiones_hay; i = i +1) {
        opinion = determinar_opinion(regiones, i);
        cambiar_opiniones(regiones, i, opinion);
    }
}
unsigned int ganador(struct Node *p){//Determinamos el ganador contando celdas
    unsigned int p1;
    unsigned int p2;
    unsigned int p3;
    p1 = 0;
    p2 = 0;
    p3 = 0;
    while(p!=NULL){
        if(p->marca == '1')
            p1 = p1 + 1;
        if(p->marca == '2')
            p2 = p2 + 1;
        if(p->marca == '3')
            p3 = p3 + 1;
        p = p->next;
    }
    printf("p1: %d, p2: %d, p3: %d\n", p1, p2, p3);
    if(p1>p2){
        return 1;
    }
    if(p2>p1){
        return 2;
    }
    if(p1==p2){
        return 3;
    }
}
int main(int argc, char *argv[]) {
    unsigned int dim;
    struct Node *matriz_estirada = NULL;
    unsigned int modo;
    unsigned int vencedor;

    if (strcmp(argv[3],"-S") == 0)
	    modo = 0;
    if (strcmp(argv[3],"-V") == 0)
        modo = 1;
    dim = atoi(argv[1]);
    scanf("%d", &dim);
    ReadData(dim, &matriz_estirada);//leemos el tablero
    matriz_estirada = determinarRegiones(matriz_estirada, dim);//determinamos las regiones
    asignar_pertenencia(matriz_estirada);//asignamos pertenencia
    PrintList(matriz_estirada, dim);
    vencedor = ganador(matriz_estirada);//determinamos el vencedor
    if(vencedor == 1){
        printf("El ganador es el jugador 1\n");
    }
    if(vencedor == 2){
        printf("El ganador es el jugador 2\n");
    }
    if(vencedor == 3){
        printf("Empate\n");
    }
}