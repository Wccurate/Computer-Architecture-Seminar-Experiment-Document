#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 128

void matrix_multiply(double **a, double **b, double **result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    double **a = (double **)malloc(N * sizeof(double *));
    double **b = (double **)malloc(N * sizeof(double *));
    double **result = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        a[i] = (double *)malloc(N * sizeof(double));
        b[i] = (double *)malloc(N * sizeof(double));
        result[i] = (double *)malloc(N * sizeof(double));
    }

    // Initialize matrices with random values
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = rand() / (double)RAND_MAX;
            b[i][j] = rand() / (double)RAND_MAX;
        }
    }

    matrix_multiply(a, b, result);

    printf("Matrix multiplication completed.\n");

    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(a[i]);
        free(b[i]);
        free(result[i]);
    }
    free(a);
    free(b);
    free(result);

    return 0;
}

