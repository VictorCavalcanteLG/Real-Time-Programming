#ifndef SIMULATE_H
#define SIMULATE_H

#include <stdio.h>
#include <math.h>

#include "../integral/integral.h"
#include "../matrix/matrix.h"
#include "../dstring/dstring.h"

// Methods
void simulate(int t, Matrix *b_matrix, Matrix *u_matrix);

#endif
