#include<stdio.h>
#include<stdlib.h>

// Prototipos
int leer_instancia(const char* nombre_archivo, int* n, int* m, int* C, int** p);
int FFD(int p[], int n, int c);
int FirstFit(int p[], int n, int c);
void mergesort(int a[], int l, int r);
void merge(int a[], int l, int mid, int r);

int main() {
    int n, m, C;
    int* p;

    if (leer_instancia("Instancia.txt", &n, &m, &C, &p) != 0) {
        return 1; // error al leer
    }

    printf("\n******Cantidad de bins usados en la instancia: [%d]*****", FFD(p, n, C));

    free(p); // liberar memoria dinámica
    return 0;
}

// =================== FUNCIONES ===================

// Lee datos desde archivo y los guarda en n, m, C y p
int leer_instancia(const char* nombre_archivo, int* n, int* m, int* C, int** p) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    if (fscanf(archivo, "%d %d %d", n, m, C) != 3) {
        fprintf(stderr, "Error leyendo cabecera del archivo\n");
        fclose(archivo);
        return 2;
    }

    *p = malloc((*n) * sizeof(int));
    if (!*p) {
        perror("Error al reservar memoria");
        fclose(archivo);
        return 3;
    }

    for (int i = 0; i < *n; i++) {
        if (fscanf(archivo, "%d", &(*p)[i]) != 1) {
            fprintf(stderr, "Error leyendo elemento %d\n", i);
            free(*p);
            fclose(archivo);
            return 4;
        }
        printf("\n\t\t\tp[%d]: %d", i, (*p)[i]);
    }

    fclose(archivo);
    return 0;
}

// First Fit Decreasing
int FFD(int p[], int n, int c) {
    mergesort(p, 0, n - 1);
    return FirstFit(p, n, c);
}

// Algoritmo First Fit
int FirstFit(int p[], int n, int c) {
    int cant_bins = 0;
    int cargas[n]; // como máximo n bins

    cargas[0] = c - p[0];
    cant_bins++;

    for (int i = 1; i < n; i++) {
        int j;
        for (j = 0; j < cant_bins; j++) {
            if (cargas[j] >= p[i]) {
                cargas[j] -= p[i];
                break;
            }
        }
        if (j == cant_bins) {
            cargas[j] = c - p[i];
            cant_bins++;
        }
    }
    return cant_bins;
}

// MergeSort descendente
void mergesort(int a[], int l, int r) {
    if (l < r) {
        int mid = (l + r) / 2;
        mergesort(a, l, mid);
        mergesort(a, mid + 1, r);
        merge(a, l, mid, r);
    }
}

void merge(int a[], int l, int mid, int r) {
    int temp[r - l + 1];
    int i = l, j = mid + 1, k = 0;

    while (i <= mid && j <= r) {
        if (a[i] > a[j]) {
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = a[i++];
    }
    while (j <= r) {
        temp[k++] = a[j++];
    }
    for (i = l; i <= r; i++) {
        a[i] = temp[i - l];
    }
}
