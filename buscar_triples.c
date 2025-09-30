#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Función para verificar si un número es cuadrado perfecto
int esCuadradoPerfecto(int n) {
    if (n < 0) return 0;
    int raiz = (int)sqrt(n);
    return (raiz * raiz == n);
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

int main(int argc, char *argv[]) {
    int limite = 100; // Límite por defecto
    
    if (argc > 1) {
        limite = atoi(argv[1]);
        if (limite <= 0) {
            printf("Error: El límite debe ser un número positivo.\n");
            return 1;
        }
    }
    
    printf("=== BUSCADOR DE TRIPLES DE CUADRADOS PERFECTOS ===\n\n");
    
    buscarTriples(limite);
    
    return 0;
}
