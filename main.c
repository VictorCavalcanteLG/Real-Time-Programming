#include <stdio.h>
#include <math.h>

#include "simulation/simulation_facade.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

Matrix *u_t(double t) {
    Matrix *m;
    
    if (t < 0.0) {
        return NULL;
    } else if (t >= 0.0 && t < 10000.0) {
        double data[2][1] = {{1.0}, {0.2 * M_PI}};
        m = create_matrix(2, 1, data);
    } else {
        double data[2][1] = {{1.0}, {-0.2 * M_PI}};
        m = create_matrix(2, 1, data);
    }
    
    return m;
}

Matrix *b_t(double w, double t) {
    t = t / 1000;
    // printf("%f\t%f\t%f\n", t, w, sin(w * t));
    double dataB[3][2] = {{sin(w * t), 0}, {cos(w * t), 0}, {0, 1}};
    Matrix *b_matrix = create_matrix(3, 2, dataB);
}

int main(int argc, char **argv) {
    int t = start_simulation(20000, u_t, b_t);

    return 0;
}
