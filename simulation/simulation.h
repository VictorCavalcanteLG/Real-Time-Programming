#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdio.h>
#include <math.h>

#include "../adt/integral/integral.h"
#include "../adt/matrix/matrix.h"
#include "../adt/dstring/dstring.h"

// typedef double (*Function)(int);

// Methods
void simulate(int t, Matrix *u_data, Matrix *u_matrix);

#endif
