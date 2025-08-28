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
    FILE* lista = fopen("Lista_Instancias.txt", "r");
    if (!lista) {
        perror("No se pudo abrir lista_instancias.txt");
        return 1;
    }

    char nombre_archivo[100];
    int n, m, C;
    int* p;
    int FFD_eval = 0;
    int FFD_count = 0;
    int FFD_fact = 0;
    int LPT_eval = 0;
    int LPT_count = 0;
    int LPT_fact = 0;
    int instance_count = 0;

    while (fscanf(lista, "%s", nombre_archivo) == 1) {
        printf("\n===== Procesando %s =====\n", nombre_archivo);

        if (leer_instancia(nombre_archivo, &n, &m, &C, &p) != 0) {
            fprintf(stderr, "Error al leer %s\n", nombre_archivo);
            continue; // pasa al siguiente archivo
        }
        instance_count++;
        mergesort(p, 0, n - 1);//Order the p's
        
        if(C > 0 || m > 0){
            if(C > 0){
                //APPLY FFD
                int FFD_result = FFD(p, n, C);
                FFD_eval++;

                if(m > 0){
                    FFD_count++;
                    if(FFD_result <= m){
                        //FEASIBLE
                        FFD_fact++;
                    }else{
                        //NOT FEASIBLE
                    }
                }else{
                    //NOT COMPARABLE
                }
            }else{
                //CANNOT APPLY FFD
            }

            if(m > 0){
                //APPLY LPT
                int LPT_result = LPT(p, n, m);
                LPT_eval++;
                if(C > 0){
                    LPT_count++;
                    if(LPT_result <= C){
                        //FEASIBLE
                        LPT_fact++;
                    }else{
                        //NOT FEASIBLE
                    }
                }else{
                    //NOT COMPARABLE
                }

            }else{
                //NOT APPLY LPT
            }

        }else{
            printf("\nLa instancia [%s] no puede ser procesada con FFD ni LPT ", nombre_archivo);
        }

        printf("\n");
        
        free(p);
    }

    printf("\nInstancias evaluadas: [%d]", instance_count);
    printf("\nSe evaluaron [%d] instancias con FFD y [%d] con LPT, de un total de [%d] Instancias", FFD_eval, LPT_eval, instance_count);
    printf("\nInstancias factibles con FFD: [%d]\tNo facibles:[%d]", FFD_fact, (FFD_count-FFD_fact));
    printf("\nInstancias factibles con LPT: [%d]\tNo facibles:[%d]", LPT_fact, (LPT_count-LPT_fact));

    fclose(lista);
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
        //printf("\n\t\t\tp[%d]: %d", i, (*p)[i]);
    }

    fclose(archivo);
    return 0;
}

// First Fit Decreasing
int FFD(int p[], int n, int c) {
    //mergesort(p, 0, n - 1);
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
