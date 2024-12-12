#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdio.h>
#include <math.h>

#include "../adt/integral/integral.h"
#include "../adt/matrix/matrix.h"
#include "../adt/dstring/dstring.h"

// typedef double (*Function)(int);
extern double sim_period, yf_period;

// Methods
Matrix *simulate(double t, Matrix *u_data, Matrix *u_matrix);
Matrix *generate_yf(double t, Matrix *b_matrix, Matrix *u_matrix);
void print_outputs(double t, Matrix *u_matrix, Matrix *y_matrix);

#endif
