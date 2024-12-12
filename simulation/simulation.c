#include "simulation.h"

#include "../adt/integral/integral.h"
#include "../adt/matrix/matrix.h"
#include "../adt/dstring/dstring.h"

double constant;

#define D 0.5

double integrate_function(double x) {
    return constant;
}

Dstr *matrix_to_dstring(Matrix *m) {
    Dstr *str_matrix = newDstr_char('[');

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            str_matrix = Dstr_cat(str_matrix, newDstr(m->data[i][j]));
            if (i + 1 != m->rows)
                str_matrix = Dstr_cat(str_matrix, newDstr_char(' '));
        }        
    }

    return Dstr_cat(str_matrix, newDstr_char(']'));
}

void print_outputs(double t, Matrix *u_matrix, Matrix *y_matrix) {
    printf("%.6f\t%s\t%s\n", t, matrix_to_dstring(u_matrix)->buf, matrix_to_dstring(y_matrix)->buf);
}


Matrix *simulate(double t, Matrix *b_matrix, Matrix *u_matrix) {
    Matrix *dx_matrix = multiply_matrices(b_matrix, u_matrix);

    double x_data[dx_matrix->rows][dx_matrix->cols];

    for (int i = 0; i < dx_matrix->rows; i++) {
        for (int j = 0; j < dx_matrix->cols; j++) {
            constant = dx_matrix->data[i][j];
            x_data[i][j] = midpoint_riemann_sum(integrate_function, t - sim_period, t, 20);
        }
    }

    Matrix *x_matrix = create_matrix(dx_matrix->rows, dx_matrix->cols, x_data);

    Matrix *identity_matrix = create_identity_matrix(x_matrix->rows); 

    Matrix *y_matrix = multiply_matrices(identity_matrix, x_matrix);

    destroy_matrix(dx_matrix);
    destroy_matrix(x_matrix);
    destroy_matrix(identity_matrix);
    
    return y_matrix;
}

Matrix *submatrix(double w, int t) {
    double submatrix_data[3][3] = {
        {0.5*D*cos(w * t), 0, 0},
        {0, 0.5*D*cos(w * t), 0},
        {0, 0, 1}
    };
    Matrix *subm = create_matrix(3, 3, submatrix_data);
    return subm;
}


Matrix *generate_yf(double t, Matrix *b_matrix, Matrix *u_matrix) {
    Matrix *dx_matrix = multiply_matrices(b_matrix, u_matrix);

    double x_data[dx_matrix->rows][dx_matrix->cols];

    for (int i = 0; i < dx_matrix->rows; i++) {
        for (int j = 0; j < dx_matrix->cols; j++) {
            constant = dx_matrix->data[i][j];
            x_data[i][j] = midpoint_riemann_sum(integrate_function, t - yf_period, t, 10);
        }
    }

    Matrix *x_matrix = create_matrix(dx_matrix->rows, dx_matrix->cols, x_data);

    Matrix *subm = submatrix(u_matrix->data[1][0], t);

    Matrix *subm_x = multiply_matrices(subm, x_matrix);

    Matrix *y_f_matrix = add_matrices(x_matrix, subm_x);

    destroy_matrix(dx_matrix);
    destroy_matrix(x_matrix);
    destroy_matrix(subm);
    destroy_matrix(subm_x);

    return y_f_matrix;
}
