//estructura para la fila bivariada
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <math.h>
#include "listaEnlazada.h"
#include "avlpath.h"
#include "Stack.h"
#include "tipStack.h"
#include <stdbool.h>
#include <gmp.h>  


unsigned char flag = '0'; //flag que indica si la lista de alcanzabilidad se puede permutar o no
//---------------------------Funciones principales---------------------------//
//funcion que chequea si un numero es cuadrado perfecto
unsigned char is_perfect_square(unsigned int x){

    unsigned int i;
    i = sqrt(x);

    if(i*i == x){
        return '1';
    }
    
    if((i+1)*(i+1) == x){
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
            valido = '0';//si no es perfect square entonces no es valido
            return valido;
        }
    }
    return valido;
}



int main(int argc, char *argv[]){
    //leer los elemetentos de la lista de entrada. Los guardamos en un array estatico.
    struct tipStack *stackaux;
    struct ElementoPila elementoPilaAux;
    struct ElementoPila verificador;
    struct Stack *pila;

    struct AVLPATH *Camino_tree = NULL;

    char *str;
    unsigned int len;
    unsigned char init;
    unsigned int seguimiento;
    //unsigned int posicion;
    unsigned int n;
    mpz_t cont;
    
    //posicion = 0;
    seguimiento = 0;
    init = '1';
    len = 0; 
    mpz_init(cont);
    mpz_set_ui(cont, 0);
    
    n = atoi(argv[1]);
    unsigned int arrayEntrada[n];
    ReadData(arrayEntrada, n);
    stackaux = tipCreateStack(n+1);
    pila = createStack(n*n);
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
                while(isEmpty(pila)!='0'){//despues de verificar borramos elementos de la pila y del camino hasta encontrar la sgte rama de computo
                    verificador = pop(pila);
                    if(verificador.tipo == '0') {
                        push(pila, '0', verificador.posicion);
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
    tipFreeStack(stackaux);
    FreeStack(pila);
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