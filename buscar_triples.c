#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Función para verificar si un número es cuadrado perfecto
int esCuadradoPerfecto(int n) {
    if (n < 0) return 0;
    int raiz = (int)sqrt(n);
    return (raiz * raiz == n);
}

// Función para verificar si todos los pares en un cuarteto suman cuadrados perfectos
int verificarCuarteto(int a, int b, int c, int d) {
    return esCuadradoPerfecto(a + b) &&
           esCuadradoPerfecto(a + c) &&
           esCuadradoPerfecto(a + d) &&
           esCuadradoPerfecto(b + c) &&
           esCuadradoPerfecto(b + d) &&
           esCuadradoPerfecto(c + d);
}

// Función para imprimir un cuarteto válido
void imprimirCuarteto(int a, int b, int c, int d) {
    printf("Cuarteto encontrado: (%d, %d, %d, %d)\n", a, b, c, d);
    printf("  %d + %d = %d = %d²\n", a, b, a+b, (int)sqrt(a+b));
    printf("  %d + %d = %d = %d²\n", a, c, a+c, (int)sqrt(a+c));
    printf("  %d + %d = %d = %d²\n", a, d, a+d, (int)sqrt(a+d));
    printf("  %d + %d = %d = %d²\n", b, c, b+c, (int)sqrt(b+c));
    printf("  %d + %d = %d = %d²\n", b, d, b+d, (int)sqrt(b+d));
    printf("  %d + %d = %d = %d²\n", c, d, c+d, (int)sqrt(c+d));
    printf("\n");
}

// Función para buscar triples donde todos los pares suman cuadrados perfectos
void buscarTriples(int limite) {
    printf("Buscando triples (a, b, c) donde a+b, a+c, y b+c son cuadrados perfectos...\n");
    printf("Límite de búsqueda: %d\n\n", limite);
    
    int encontrados = 0;
    
    for (int a = 1; a <= limite; a++) {
        for (int b = a + 1; b <= limite; b++) {
            // Verificar si a + b es cuadrado perfecto
            if (!esCuadradoPerfecto(a + b)) continue;
            
            for (int c = b + 1; c <= limite; c++) {
                // Verificar si a + c es cuadrado perfecto
                if (!esCuadradoPerfecto(a + c)) continue;
                
                // Verificar si b + c es cuadrado perfecto
                if (!esCuadradoPerfecto(b + c)) continue;
                
                // Si llegamos aquí, encontramos un triple válido
                printf("Triple encontrado: (%d, %d, %d)\n", a, b, c);
                printf("  %d + %d = %d = %d²\n", a, b, a+b, (int)sqrt(a+b));
                printf("  %d + %d = %d = %d²\n", a, c, a+c, (int)sqrt(a+c));
                printf("  %d + %d = %d = %d²\n", b, c, b+c, (int)sqrt(b+c));
                printf("\n");
                encontrados++;
            }
        }
    }
    
    if (encontrados == 0) {
        printf("No se encontraron triples válidos en el rango dado.\n");
        printf("Esto es esperado ya que tales triples son extremadamente raros.\n\n");
        
        // Buscar pares que al menos funcionen
        printf("Buscando pares que sumen cuadrados perfectos:\n");
        int paresEncontrados = 0;
        for (int a = 1; a <= limite && paresEncontrados < 10; a++) {
            for (int b = a + 1; b <= limite && paresEncontrados < 10; b++) {
                if (esCuadradoPerfecto(a + b)) {
                    printf("  %d + %d = %d = %d²\n", a, b, a+b, (int)sqrt(a+b));
                    paresEncontrados++;
                }
            }
        }
    } else {
        printf("Total de triples encontrados: %d\n", encontrados);
    }
}

// Función para buscar cuartetos donde todos los pares suman cuadrados perfectos
void buscarCuartetos(int limite) {
    printf("Buscando cuartetos (a, b, c, d) donde todos los pares suman cuadrados perfectos...\n");
    printf("Límite de búsqueda: %d\n\n", limite);
    
    int encontrados = 0;
    
    for (int a = 1; a <= limite; a++) {
        printf("Procesando a = %d...\n", a);
        
        for (int b = a + 1; b <= limite; b++) {
            // Optimización: verificar a+b primero
            if (!esCuadradoPerfecto(a + b)) continue;
            
            for (int c = b + 1; c <= limite; c++) {
                // Optimización: verificar a+c y b+c
                if (!esCuadradoPerfecto(a + c)) continue;
                if (!esCuadradoPerfecto(b + c)) continue;
                
                for (int d = c + 1; d <= limite; d++) {
                    // Verificar todas las condiciones restantes
                    if (verificarCuarteto(a, b, c, d)) {
                        imprimirCuarteto(a, b, c, d);
                        encontrados++;
                    }
                }
            }
        }
    }
    
    if (encontrados == 0) {
        printf("No se encontraron cuartetos válidos en el rango dado.\n");
        printf("Los cuartetos válidos son extremadamente raros y requieren números muy grandes.\n\n");
        
        // Buscar triples como alternativa
        printf("Buscando triples válidos como alternativa:\n");
        int triplesEncontrados = 0;
        for (int a = 1; a <= limite && triplesEncontrados < 5; a++) {
            for (int b = a + 1; b <= limite && triplesEncontrados < 5; b++) {
                if (!esCuadradoPerfecto(a + b)) continue;
                for (int c = b + 1; c <= limite && triplesEncontrados < 5; c++) {
                    if (esCuadradoPerfecto(a + c) && esCuadradoPerfecto(b + c)) {
                        printf("  Triple: (%d, %d, %d)\n", a, b, c);
                        printf("    %d+%d=%d=%d², %d+%d=%d=%d², %d+%d=%d=%d²\n", 
                               a, b, a+b, (int)sqrt(a+b),
                               a, c, a+c, (int)sqrt(a+c),
                               b, c, b+c, (int)sqrt(b+c));
                        triplesEncontrados++;
                        break;
                    }
                }
            }
        }
    } else {
        printf("Total de cuartetos encontrados: %d\n", encontrados);
    }
}

int main(int argc, char *argv[]) {
    int limite = 100; // Límite por defecto
    int modo = 3; // Por defecto buscar triples
    
    if (argc > 1) {
        limite = atoi(argv[1]);
        if (limite <= 0) {
            printf("Error: El límite debe ser un número positivo.\n");
            return 1;
        }
    }
    
    if (argc > 2) {
        modo = atoi(argv[2]);
        if (modo != 3 && modo != 4) {
            printf("Error: El modo debe ser 3 (triples) o 4 (cuartetos).\n");
            return 1;
        }
    }
    
    printf("=== BUSCADOR DE CUADRADOS PERFECTOS ===\n\n");
    
    if (modo == 3) {
        printf("Modo: TRIPLES\n");
        buscarTriples(limite);
    } else {
        printf("Modo: CUARTETOS\n");
        printf("NOTA: Los cuartetos válidos son extremadamente raros.\n");
        printf("Es posible que no se encuentren en rangos pequeños.\n\n");
        buscarCuartetos(limite);
    }
    
    return 0;
}
