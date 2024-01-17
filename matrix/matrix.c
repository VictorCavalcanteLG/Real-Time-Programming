#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

// Constructor
Matrix *create_matrix(int rows, int cols, double data[][cols]) {
    Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
    if (matrix == NULL) {
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;

    matrix->data = (double **)malloc(rows * sizeof(double *));
    if (matrix->data == NULL) {
        free(matrix);
        return NULL;
    }

    for (int i = 0; i < rows; ++i) {
        matrix->data[i] = (double *)malloc(cols * sizeof(double));
        if (matrix->data[i] == NULL) {
            destroy_matrix(matrix);
            return NULL;
        }

        for (int j = 0; j < cols; ++j) {
            matrix->data[i][j] = data[i][j];
        }
    }

    return matrix;
}

// Destructor
void destroy_matrix(Matrix *matrix) {
    if (matrix != NULL) {
        for (int i = 0; i < matrix->rows; ++i) {
            free(matrix->data[i]);
        }
        free(matrix->data);
        free(matrix);
    }
}

// Methods
Matrix *add_matrices(Matrix *a, Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return NULL;
    }

    Matrix *new_matrix = (Matrix *)malloc(sizeof(Matrix));

    new_matrix->rows = a->rows;
    new_matrix->cols = a->cols;

    new_matrix->data = (double **)malloc(a->rows * sizeof(double *));
    if (new_matrix->data == NULL) {
        free(new_matrix);
        return NULL;
    }

    for(int i = 0; i < a->rows; i++) {
        new_matrix->data[i] = (double *)malloc(a->cols * sizeof(double));
        if (new_matrix->data[i] == NULL) {
            destroy_matrix(new_matrix);
            return NULL;
        }

        for(int j = 0; j < a->cols; j++) {
            new_matrix->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }

    return new_matrix;
}

Matrix *subtract_matrices(Matrix *a, Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return NULL;
    }

    Matrix *new_matrix = (Matrix *)malloc(sizeof(Matrix));

    new_matrix->rows = a->rows;
    new_matrix->cols = a->cols;

    new_matrix->data = (double **)malloc(a->rows * sizeof(double *));
    if (new_matrix->data == NULL) {
        free(new_matrix);
        return NULL;
    }

    for(int i = 0; i < a->rows; i++) {
        new_matrix->data[i] = (double *)malloc(a->cols * sizeof(double));
        if (new_matrix->data[i] == NULL) {
            destroy_matrix(new_matrix);
            return NULL;
        }

        for(int j = 0; j < a->cols; j++) {
            new_matrix->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }

    return new_matrix;
}

Matrix *multiply_matrices(Matrix *a, Matrix *b) {
    if (a->cols != b->rows) {
        return NULL;
    }

    Matrix *new_matrix = (Matrix *)malloc(sizeof(Matrix));
    if (new_matrix == NULL) {
        return NULL;
    }

    new_matrix->rows = a->rows;
    new_matrix->cols = b->cols;

    new_matrix->data = (double **)malloc(new_matrix->rows * sizeof(double *));
    if (new_matrix->data == NULL) {
        free(new_matrix);
        return NULL;
    }

    for (int i = 0; i < new_matrix->rows; i++) {
        new_matrix->data[i] = (double *)malloc(new_matrix->cols * sizeof(double));
        if (new_matrix->data[i] == NULL) {
            destroy_matrix(new_matrix);  // Lidar com erro de alocação para cada linha
            return NULL;
        }

        for (int j = 0; j < new_matrix->cols; j++) {
            new_matrix->data[i][j] = 0.0;

            for (int k = 0; k < a->cols; k++) {
                new_matrix->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }

    return new_matrix;
}

Matrix *transpose(Matrix *matrix) {
    Matrix *new_matrix = (Matrix *)malloc(sizeof(Matrix));
    if (new_matrix == NULL) {
        return NULL;
    }

    new_matrix->rows = matrix->cols;
    new_matrix->cols = matrix->rows;

    new_matrix->data = (double **)malloc(new_matrix->rows * sizeof(double *));
    if (new_matrix->data == NULL) {
        destroy_matrix(new_matrix);
        return NULL;
    }

    for (int i = 0; i < new_matrix->rows; i++) {
        new_matrix->data[i] = (double *)malloc(new_matrix->cols * sizeof(double));
        if (new_matrix->data[i] == NULL) {
            destroy_matrix(new_matrix);
            return NULL;
        }

        for (int j = 0; j < new_matrix->cols; j++) {
            new_matrix->data[i][j] = matrix->data[j][i];
        }
    }

    return new_matrix;
}

double determinant(Matrix *matrix) {
    if (matrix == NULL || matrix->rows != matrix->cols) {
        fprintf(stderr, "Erro: Matriz inválida.\n");
        exit(EXIT_FAILURE);
    }

    double det = 0.0;

    if (matrix->rows == 1) {
        return matrix->data[0][0];
    }

    if (matrix->rows == 2) {
        return matrix->data[0][0] * matrix->data[1][1] - matrix->data[0][1] * matrix->data[1][0];
    }

    double m_aux[matrix->rows - 1][matrix->cols - 1];

    for (int j = 0; j < matrix->cols; ++j) {
        Matrix *submatrix_ptr = create_matrix(matrix->rows - 1, matrix->cols - 1, m_aux);
        if (submatrix_ptr == NULL) {
            destroy_matrix(matrix);
            fprintf(stderr, "Erro: Falha ao criar submatriz.\n");
            exit(EXIT_FAILURE);
        }

        int sub_i = 0, sub_j = 0;

        for (int i = 1; i < matrix->rows; ++i) {
            sub_j = 0;
            for (int k = 0; k < matrix->cols; ++k) {
                if (k != j) {
                    submatrix_ptr->data[sub_i][sub_j] = matrix->data[i][k];
                    sub_j++;
                }
            }
            sub_i++;
        }

        double cofactor = matrix->data[0][j] * determinant(submatrix_ptr);
        if (j % 2 == 1) {
            cofactor = -cofactor;
        }

        det += cofactor;

        destroy_matrix(submatrix_ptr);
    }

    return det;
}

void print_matrix(Matrix *matrix) {
    printf("[\n");
    for (int i = 0; i < matrix->rows; ++i) {
        for (int j = 0; j < matrix->cols; ++j) {
            printf("%f\t", matrix->data[i][j]);
        }
        printf("\n");
    }
    printf("]\n");
}
