
#include "simulation.h"

Matrix *simulate_yf(Matrix *u, double D) {
    Matrix *y = simulate(u); // Original simulation
    double x3 = y->data[2][0]; // Extract x3 (orientation)
    double offset_data[3][1] = {
        {0.5 * D * cos(x3)},
        {0.5 * D * sin(x3)},
        {0}
    };
    Matrix *offset = create_matrix(3, 1, offset_data);
    Matrix *y_f = add_matrices(y, offset);
    destroy_matrix(offset);
    destroy_matrix(y);
    return y_f;
}
