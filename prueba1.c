#include<stdio.h>
#include<stdlib.h>

int leer_instancia(const char* nombre_archivo, int* n, int* m, int* C, int** p);
int FFD(int p[], int n, int c);
int FirstFit(int p[], int n, int c);
int LPT(int p[], int n, int m);
void min_C_m(int C_m[]);
void mergesort(int a[], int l, int r);
void merge(int a[], int l, int mid, int r);

int main() {
    int n, m, C;
    int* p;

    if (leer_instancia("Instancia.txt", &n, &m, &C, &p) != 0) {
        return 1; //error al leer
    }

    printf("\n******Cantidad de bins usados en la instancia: [%d]*****", FFD(p, n, C));
    printf("\n******Makespan para la instancia: [%d]*****", LPT(p, n, m));
    free(p); 
    return 0;
}


// se leen datos desde un archivo y los guarda en n, m, C y p
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

int LPT(int p[], int n, int m){
    //mergesort(p, 0, n - 1);

    int C_m[m]; //Tiempo de completés de cada máquina
    int i;
    //Asignar primeros m trabajos a las m maquinas
    for (i = 0; i < m; i++) {
        C_m[m-i-1] = p[i];
        //printf("Trabajo %d con p=%d asignado a la maquina %d\n", i + 1, p[i], m-i );
    }

    //asignar siguientes trabajos 
    for (i = m; i < n; i++) {
        //printf("Asignar trabajo [%d] con p=[%d] a la máquina con carga: [%d]\n", i + 1, p[i], C_m[0]);
        C_m[0] += p[i];
        //printf("%d \n",C_m[0], m);
        min_C_m(C_m);
        //printf("Actualizando cargas:\n");
        //printf("\n\n");
    }

    return C_m[m-1];
}

void min_C_m(int C_m[]) {
    int C = C_m[0];

    int i = 1;
    while (C > C_m[i]) {
        C_m[i - 1] = C_m[i];
        i++;
    }
    C_m[i - 1] = C;
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

