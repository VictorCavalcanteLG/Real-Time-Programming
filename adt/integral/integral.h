#ifndef INTEGRAL_H
#define INTEGRAL_H

// Tipo de função a ser integrada
typedef double (*Function)(double);

// Métodos de integração
double left_riemann_sum(Function f, double inf, double sup, int n);
double trapezoidal_riemann_sum(Function f, double a, double b, int n);
double midpoint_riemann_sum(Function f, double inf, double sup, int n);
double simpson_rule(Function f, double a, double b, int n);

#endif