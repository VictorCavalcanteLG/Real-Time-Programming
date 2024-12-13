#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

// Constructor
Matrix *create_matrix(int rows, int cols, double data[][cols]);

// Destructor
void destroy_matrix(Matrix *matrix);

// Methods
Matrix *add_matrices(Matrix *a, Matrix *b);
Matrix *subtract_matrices(Matrix *a, Matrix *b);
Matrix *multiply_matrices(Matrix *a, Matrix *b);
Matrix *scalar_multiply(Matrix *matrix, double scalar);

Matrix *transpose(Matrix *matrix);
double determinant(Matrix *matrix);
Matrix *invert_matrix_2x2(Matrix *matrix);
Matrix *inverse(Matrix *matrix);
Matrix *create_identity_matrix(int dim);

void print_matrix(Matrix *matrix);

#endif
