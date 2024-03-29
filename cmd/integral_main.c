#include <stdio.h>

#include "../adt/integral/integral.h"

double func1(double x) {
    return x*x*x + 2*x + 1 ;
}

int main() {
    puts("Test ADT Integral");

    double integral1 = left_riemann_sum(func1, 0, 10, 30);
    printf("left riemann sum = %f\n", integral1);

    double integral2 = trapezoidal_riemann_sum(func1, 0, 10, 30);
    printf("trapezoidal riemann sum = %f\n", integral2);

    double integral3 = midpoint_riemann_sum(func1, 0, 10, 30);
    printf("midpoint riemann sum = %f\n", integral3);

    double integral4 = simpson_rule(func1, 0, 10, 30);
    printf("simpson rule = %f\n", integral4);

    return 0;
}