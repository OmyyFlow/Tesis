#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int leer_instancia(const char* nombre_archivo, int* n, int* m, int* C, int** p);
int FFD(int p[], int n, int c);
int FirstFit(int p[], int n, int c);
int LPT(int p[], int n, int m);
void min_C_m(int C_m[]);
void mergesort(int a[], int l, int r);
void merge(int a[], int l, int mid, int r);

int main() {
    FILE* lista = fopen("lista_instancias.txt", "r");
    if (!lista) {
        perror("No se pudo abrir lista_instancias.txt");
        return 1;
    }

    char nombre_archivo[100];
    int n, m, C;
    int* p;

    while (fscanf(lista, "%s", nombre_archivo) == 1) {
        printf("\n===== Procesando %s =====\n", nombre_archivo);

        if (leer_instancia(nombre_archivo, &n, &m, &C, &p) != 0) {
            fprintf(stderr, "Error al leer %s\n", nombre_archivo);
            continue; // pasa al siguiente archivo
        }

        printf("Cantidad de bins usados en la instancia: [%d]\n", FFD(p, n, C));
        printf("Makespan para la instancia: [%d]\n", LPT(p, n, m));

        free(p);
    }

    fclose(lista);
    return 0;
}
