#ifndef SIMULATION_FACADE_H
#define SIMULATION_FACADE_H

#include "../adt/integral/integral.h"
#include "../adt/matrix/matrix.h"
#include "../adt/dstring/dstring.h"

typedef Matrix *(*Function_u)(int);
typedef Matrix *(*Function_b)(double, int);

// Methods
int start_simulation(int simulation_time, Function_u u_t, Function_b b_t);

#endif