#include "integral.h"

double left_riemann_sum(Function f, double a, double b, int n) {
    double result = 0.0;
    double delta_x = (b - a) / n;

    for (int i = 0; i < n; i++) {
        double x_i = a + i * delta_x;
        result += f(x_i) * delta_x;
    }

    return result;
}

double trapezoidal_riemann_sum(Function f, double a, double b, int n) {
    double result = 0.0;
    double delta_x = (b - a) / n;

    result += (f(a) + f(b)) / 2.0;

    for (int i = 1; i < n; i++) {
        double x_i = a + i * delta_x;
        result += f(x_i);
    }

    result *= delta_x;

    return result;
}

double midpoint_riemann_sum(Function f, double a, double b, int n) {
    double result = 0.0;
    double delta_x = (b - a) / n;

    for (int i = 0; i < n; i++) {
        double x_i = a + (i + 0.5) * delta_x;
        result += f(x_i);
    }

    result *= delta_x;

    return result;
}

double simpson_rule(Function f, double a, double b, int n) {
    if (n % 2 != 0) {
        return 0.0;
    }

    double result = 0.0;
    double delta_x = (b - a) / n;

    result += f(a) + f(b);

    for (int i = 1; i < n; i++) {
        double x_i = a + i * delta_x;
        result += (i % 2 == 0) ? 2.0 * f(x_i) : 4.0 * f(x_i);
    }

    result *= delta_x / 3.0;

    return result;
}
