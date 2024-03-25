#include <stdio.h>
#include <math.h>

#include "simulate/simulate.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

Matrix *u_t(int t) {
    Matrix *m;
    
    if (t < 0) {
        return NULL;
    } else if (t >= 0 && t < 10) {
        double data[2][1] = {{1.0}, {0.2 * M_PI}};
        m = create_matrix(2, 1, data);
    } else {
        double data[2][1] = {{1.0}, {-0.2 * M_PI}};
        m = create_matrix(2, 1, data);
    }
    

    return m;
}

int main(int argc, char **argv) {
    

    for (int t = 0; t < 21; t++) {
        Matrix *u_matrix = u_t(t);

        double dataB[3][2] = {{sin(u_matrix->data[1][0] * t), 0}, {cos(u_matrix->data[1][0] * t), 0}, {0, 1}};
        Matrix *b_matrix = create_matrix(3, 2, dataB);

        simulate(t, b_matrix, u_matrix);

        destroy_matrix(u_matrix);
        destroy_matrix(b_matrix);
    }

    return 0;
}
