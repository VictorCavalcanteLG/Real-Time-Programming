#include <stdio.h>

#include "../adt/matrix/matrix.h"


int main() {
    puts("Test ADT Matrix");

    double data1[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    double data2[3][4] = {{1.0, 2.0, 3.0, 4.0}, {4.0, 5.0, 6.0, 7.0}, {4.0, 5.0, 6.0, 7.0}};
    double data3[2][3] = {{4.0, 2.0, 5.0}, {1.0, 4.0, 6.0}};
    double data4[4][3] = {{4.0, 4.0, 9.0}, {3.0, 2.0, 6.0}, {8.0, 12.0, 5.0}, {7.0, 9.0, 6.0}};
    double data5[3][3] = {{4.0, 4.0, 9.0}, {3.0, 2.0, 6.0}, {8.0, 12.0, 5.0}};

    Matrix *m1 = create_matrix(2, 3, data1);
    Matrix *m2 = create_matrix(3, 4, data2);
    Matrix *m3 = create_matrix(2, 3, data3);
    Matrix *m4 = create_matrix(4, 3, data4);
    Matrix *m5 = create_matrix(3, 3, data5);

    // Imprimir ou manipular a matriz
    print_matrix(m1);
    print_matrix(m2);

    Matrix *m_add = add_matrices(m1, m3);
    if (m_add != NULL)
        print_matrix(m_add);
    else
        printf("matrizes de tamanhos diferentes\n");

    Matrix *m_sub = subtract_matrices(m1, m3);
    if (m_add != NULL)
        print_matrix(m_sub);
    else
        printf("matrizes de tamanhos diferentes\n");

    Matrix *m_mul = multiply_matrices(m2, m4);
    if (m_mul != NULL)
        print_matrix(m_mul);
    else
        printf("Matrizes de tamanhos incopativeis para a multiplicacao\n");

    Matrix *m_trans = transpose(m_mul);
    if (m_trans != NULL)
        print_matrix(m_trans);
    else
        printf("Erro de alocação de memória\n");

    double dert = determinant(m5);
    printf("determinante = %f\n", dert);

    // Liberar memória
    destroy_matrix(m1);
    destroy_matrix(m2);
    destroy_matrix(m_add);
    destroy_matrix(m_sub);
    destroy_matrix(m_mul);
    destroy_matrix(m_trans);

    return 0;
}