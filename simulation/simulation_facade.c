#include "simulation_facade.h"
#include "simulation.h"

int start_simulation(int simulation_time, Function_u u_t, Function_b b_t) {

    for (int t = 0; t <= simulation_time; t++) {
        Matrix *u_matrix = u_t(t);

        Matrix *b_matrix = b_t(u_matrix->data[1][0], t);

        simulate(t, b_matrix, u_matrix);

        destroy_matrix(u_matrix);
        destroy_matrix(b_matrix);
    }

    return 1;
}
