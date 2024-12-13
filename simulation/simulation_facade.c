#include <math.h>
#include <pthread.h>
#include <unistd.h>

#include "simulation_facade.h"
#include "simulation.h"

#define R 0.6 // raio 0.6m
#define alpha1 0.001
#define alpha2 0.001

#define LINEARIZATION_PERIOD 40   // 40ms
#define ROBOT_PERIOD 30 // 30ms
#define CONTROL_PERIOD 50 // 50ms
#define REFERENCE_PERIOD 120 // 120ms
#define REF_MODEL_IN_X_PERIOD 50 // 50ms
#define REF_MODEL_IN_Y_PERIOD 50 // 50ms
#define YF_PERIOD 50     // 50ms
#define ACSITION_PERIOD 0.5        // 1s

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Inclua essa declaração no topo do arquivo simulation_facade.c
Matrix *u_t(double t);
Matrix *b_t(double u_value, double t);

double sim_period, yf_period;

typedef struct {
    Matrix *u;
    Matrix *v;
    Matrix *x;
    Matrix *y;
    double xref, yref; 
    double ymx, ymy;
    double dot_ymx,dot_ymy;
    Matrix *yf;
    pthread_mutex_t lock;
} SharedData;

double constant;
double integrate_function(double x) {
    return constant;
}

double X_ref(double t) {
    t = t / 1000;
    return (5/M_PI) * cos(0.2 * M_PI * t);
}

double Y_ref(double t) {
    t = t / 1000;
    if (t < 10.0) {
        return (5/M_PI)* sin(0.2 * M_PI * t);
    }
    return (-5/M_PI)* sin(0.2 * M_PI * t);
}


Matrix *L_x(double w, double t) {
    t = t / 1000;
    double data_L[2][2] = {{cos(w * t), -R * sin(w * t)}, {sin(w * t), R * cos(w * t)}};
    Matrix *L_matrix = create_matrix(2, 2, data_L);
}

Matrix *D_x3(double w, double t) {
    t = t / 1000;
    double data_d[2][1] = {{R * cos(w * t)}, {R * sin(w * t)}};
    Matrix *d_matrix = create_matrix(2, 1, data_d);
}

// Matrix *Y_m(Matrix *ym_prev){}


////////////////////////////////////////// THREADS /////////////////////////////////////////
void *robot_simulation(void *args) {
    SharedData *shared_data = (SharedData *)args;
    int t = 0;

    while (1) {
        if(t % ROBOT_PERIOD == 0) {
            // printf("aquiiii 1\n");
            // fflush(stdout);
            pthread_mutex_lock(&shared_data->lock);
                // printf("aquiiii 2\n");

                if (shared_data->u) {
                    // printf("aquiiii 3\n");
                    if (shared_data->x) destroy_matrix(shared_data->x);
                    if (shared_data->y) destroy_matrix(shared_data->y);
                    
                    // gerando a matrix B do espaço de estados
                    Matrix *b_matrix = b_t(shared_data->u->data[1][0], t);

                    // gerando a matrix C do espaço de estados
                    double c_data[2][3] = {{1, 0, 0},{0, 1, 0}};
                    Matrix *c_matrix = create_matrix(2, 3, c_data);

                    // gerando a matrix D do espaço de estados
                    Matrix *d_matrix = D_x3(shared_data->u->data[1][0], t);

                    // gerando a função derivada de X (dx) do espaço de estados
                    Matrix *dx_matrix = multiply_matrices(b_matrix, shared_data->u);

                    // Integrando para se obter a matrix X
                    double x_data[dx_matrix->rows][dx_matrix->cols];

                    for (int i = 0; i < dx_matrix->rows; i++) {
                        for (int j = 0; j < dx_matrix->cols; j++) {
                            constant = dx_matrix->data[i][j];
                            x_data[i][j] = midpoint_riemann_sum(integrate_function, t - ROBOT_PERIOD, t, 20);
                        }
                    }

                    Matrix *x_matrix = create_matrix(dx_matrix->rows, dx_matrix->cols, x_data);

                    // Assim é possível se obter Y
                    Matrix *y_matrix = add_matrices(multiply_matrices(c_matrix, x_matrix), d_matrix); 

                    shared_data->x = x_matrix;
                    shared_data->y = y_matrix;

                    // printf("u: %s\n", matrix_to_dstring(shared_data->u)->buf);
                    // printf("v: %s\n", matrix_to_dstring(shared_data->v)->buf);
                    // printf("y: %s\n\n", matrix_to_dstring(shared_data->y)->buf);
                    print_outputs(t, shared_data->u, shared_data->x);
                }
            pthread_mutex_unlock(&shared_data->lock);
        }
        t++;
        usleep(1000); // 50ms delay
    }
    return NULL;
}

void *linearization(void *args) {
    SharedData *shared_data = (SharedData *)args;
    int t = 0;

    while (1) {
        // Generate u(t)
        if(t % LINEARIZATION_PERIOD == 0) {

            // Lock shared data to write u
            pthread_mutex_lock(&shared_data->lock);
                if (shared_data->x && shared_data->v) {
                    double w = shared_data->u->data[1][0];
                    if (shared_data->u) destroy_matrix(shared_data->u);

                    Matrix *L_matrix = L_x(w, t);

                    Matrix *inverse_L_matrix = invert_matrix_2x2(L_matrix);

                    shared_data->u = multiply_matrices(inverse_L_matrix, shared_data->v);

                    // print_outputs(t, shared_data->u, u_matrix);
                    // printf("linearization:\t%s\n", matrix_to_dstring(shared_data->v)->buf);
                }
            pthread_mutex_unlock(&shared_data->lock);
        }
        t++;
        usleep(1000); // 30ms delay
    }
    return NULL;
}

void *control_simulation(void *args) {
    SharedData *shared_data = (SharedData *)args;
    int t = 0;

    while (1) {
        if(t % CONTROL_PERIOD == 0) {
            pthread_mutex_lock(&shared_data->lock);

                if (shared_data->y && shared_data->ymx && shared_data->ymy) {
                    
                    if (shared_data->v) destroy_matrix(shared_data->v);
                    double v_t_data[2][1] = {
                        {shared_data->dot_ymx + alpha1*(shared_data->ymx - shared_data->y->data[0][0])},
                        {shared_data->dot_ymy + alpha2*(shared_data->ymy - shared_data->y->data[1][0])}
                    };

                    Matrix *v_t_matrix = create_matrix(2, 1, v_t_data);
                    
                    shared_data->v = v_t_matrix;

                    // printf("control_simulation\t %s", matrix_to_dstring(shared_data->y)->buf);
                }
            
            pthread_mutex_unlock(&shared_data->lock);
        }
        t++;
        usleep(1000); // 50ms delay
    }
    return NULL;
}

// CORRETO
void *ref_model_in_x_simulation(void *args) {
    SharedData *shared_data = (SharedData *)args;
    int t = 0;

    while (1) {
        if(t % REF_MODEL_IN_X_PERIOD == 0) {
            pthread_mutex_lock(&shared_data->lock);

                // constant = shared_data->dot_ymx;
                // double ymx_data = midpoint_riemann_sum(integrate_function, t - REF_MODEL_IN_X_PERIOD, t, 20);

                // constant = shared_data->dot_ymy;
                // double ymy_data = midpoint_riemann_sum(integrate_function, t - REF_MODEL_IN_X_PERIOD, t, 20);

                // shared_data->ymx = ymx_data;
                // shared_data->ymy = ymy_data;

                shared_data->ymx += shared_data->dot_ymx * (REF_MODEL_IN_X_PERIOD / 1000.0);
                shared_data->ymy += shared_data->dot_ymy * (REF_MODEL_IN_X_PERIOD / 1000.0);

                // printf("ref_model_in_x_simulation\t%f\t%f\n", shared_data->ymx, shared_data->ymy);

            pthread_mutex_unlock(&shared_data->lock);
        }
        t++;
        usleep(1000);
    }
    return NULL;
}

// CORRETO
void *ref_model_in_y_simulation(void *args) {
    SharedData *shared_data = (SharedData *)args;
    int t = 0;

    while (1) {
        if(t % REF_MODEL_IN_Y_PERIOD == 0) {
            pthread_mutex_lock(&shared_data->lock);

                shared_data->dot_ymx = alpha1 * (shared_data->xref - shared_data->ymx);
                shared_data->dot_ymy = alpha2 * (shared_data->yref - shared_data->ymy);
                // printf("ref_model_in_y_simulation %f\n", shared_data->xref - shared_data->ymx);

            pthread_mutex_unlock(&shared_data->lock);
        }
        t++;
        usleep(1000);
    }
    return NULL;
}

// CORRETO
void *generate_references(void *args) {
    SharedData *shared_data = (SharedData *)args;
    int t = 0;

    while (1) {
        if(t % REFERENCE_PERIOD == 0) {
            pthread_mutex_lock(&shared_data->lock);

                shared_data->xref = X_ref(t);
                shared_data->yref = Y_ref(t);

                // printf("generate_references\n");

            pthread_mutex_unlock(&shared_data->lock);
        }
        t++;
        usleep(1000);
    }
    return NULL;
}


void *sample_yf(void *args) {
    SharedData *shared_data = (SharedData *)args;
    int t = 0;

    while (1) {
        if(t % YF_PERIOD == 0) {
            pthread_mutex_lock(&shared_data->lock);

                if (shared_data->u) {
                    // Generate yf(t) using the latest u(t)
                    if (shared_data->yf) destroy_matrix(shared_data->yf);
                    Matrix *b_matrix = b_t(shared_data->u->data[1][0], t);
                    // shared_data->yf = generate_yf(t, b_matrix, shared_data->u);
                    // print_outputs(t, shared_data->u, shared_data->yf);
                    // destroy_matrix(b_matrix);
                }
            pthread_mutex_unlock(&shared_data->lock);
        }
        t++;
        usleep(1000); // 50ms delay
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////

void *print_outputs_routine(void *args){
    SharedData *shared_data = (SharedData *)args;

    // Print outputs every 1 second
    double elapsed_time = 0.0;

    while (1) {
        sleep(1); // Wait for 1 second

        // pthread_mutex_lock(&shared_data->lock);

        // if (shared_data->u && shared_data->yf) {
        //     // Print the current values of u and yf
        //     print_outputs(elapsed_time, shared_data->u, shared_data->yf);
        // }

        // pthread_mutex_unlock(&shared_data->lock);

        // elapsed_time++;
    }

}

int start_simulation(int simulation_time, Function_u u_t, Function_b b_t) {
    sim_period = LINEARIZATION_PERIOD;
    yf_period = YF_PERIOD;

    pthread_t thread1, thread2, thread3, thread4, thread5, thread6;
    SharedData shared_data;
    shared_data.u = create_matrix(2, 1, (double[2][1]){{0}, {0}});
    shared_data.v = create_matrix(2, 1, (double[2][1]){{0}, {0}});
    shared_data.x = create_matrix(3, 1, (double[3][1]){{0}, {0}, {0}});

    // shared_data.u = NULL;
    // shared_data.v = NULL;
    // shared_data.x = NULL;

    double data_y[2][1] = {{0}, {0}};
    shared_data.y = create_matrix(2, 1, data_y);

    shared_data.xref = 0;
    shared_data.yref = 0;
    shared_data.ymx = 0;
    shared_data.ymy = 0;
    shared_data.dot_ymx = 0;
    shared_data.dot_ymy = 0;
    shared_data.yf = NULL;
    pthread_mutex_init(&shared_data.lock, NULL);

    // Create threads ref_model_in_x_simulation
    pthread_create(&thread1, NULL, robot_simulation, &shared_data);
    pthread_create(&thread2, NULL, linearization, &shared_data);
    pthread_create(&thread3, NULL, control_simulation, &shared_data);
    pthread_create(&thread4, NULL, ref_model_in_x_simulation, &shared_data);
    pthread_create(&thread5, NULL, ref_model_in_y_simulation, &shared_data);
    pthread_create(&thread6, NULL, generate_references, &shared_data);

    usleep(simulation_time*1000);

    // Clean up
    pthread_cancel(thread1);
    pthread_cancel(thread2);
    pthread_cancel(thread3);
    pthread_cancel(thread4);
    pthread_cancel(thread5);
    pthread_cancel(thread6);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);

    // Destroy remaining data
    pthread_mutex_destroy(&shared_data.lock);
    if (shared_data.u) destroy_matrix(shared_data.u);
    if (shared_data.yf) destroy_matrix(shared_data.yf);

    // for (double t = 0; t <= simulation_time; t+=yf_period) {
    //     Matrix *u_matrix = u_t(t);

    //     Matrix *b_matrix = b_t(u_matrix->data[1][0], t);

    //     simulate(t, b_matrix, u_matrix);
    //     Matrix *yf_matrix = generate_yf(t, b_matrix, u_matrix);

    //     print_outputs(t, u_matrix, yf_matrix);

    //     destroy_matrix(u_matrix);
    //     destroy_matrix(b_matrix);
    // }
    // return 1;
}
