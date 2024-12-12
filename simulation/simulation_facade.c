#include <math.h>
#include <pthread.h>
#include <unistd.h>

#include "simulation_facade.h"
#include "simulation.h"

#define SIMULATION_PERIOD 30   // 30ms
#define YF_PERIOD 50     // 50ms
#define ACSITION_PERIOD 0.5        // 1s

// Inclua essa declaração no topo do arquivo simulation_facade.c
Matrix *u_t(double t);
Matrix *b_t(double u_value, double t);

double sim_period, yf_period;

typedef struct {
    Matrix *u;
    Matrix *yf;
    pthread_mutex_t lock;
} SharedData;

void *generate_and_simulate(void *args) {
    SharedData *shared_data = (SharedData *)args;
    int t = 0;

    while (1) {
        // Generate u(t)
        if(t % SIMULATION_PERIOD == 0) {

            // Lock shared data to write u
            pthread_mutex_lock(&shared_data->lock);

                Matrix *u_matrix = u_t(t);
                if (shared_data->u) destroy_matrix(shared_data->u);
                shared_data->u = u_matrix;
                // print_outputs(t, shared_data->u, u_matrix);

            pthread_mutex_unlock(&shared_data->lock);

            // Generate b(t) and simulate
            // Matrix *b_matrix = b_t(u_matrix->data[1][0], t*SIMULATION_PERIOD/1000);
            // Matrix *y_matrix = simulate(t*SIMULATION_PERIOD/1000, b_matrix, u_matrix);

            // destroy_matrix(b_matrix);
            // destroy_matrix(y_matrix);
        }
        t++;
        usleep(1000); // 30ms delay
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
                    shared_data->yf = generate_yf(t, b_matrix, shared_data->u);
                    print_outputs(t, shared_data->u, shared_data->yf);
                    // destroy_matrix(b_matrix);
                }
            pthread_mutex_unlock(&shared_data->lock);
        }
        t++;
        usleep(1000); // 50ms delay
    }
    return NULL;
}

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
    sim_period = SIMULATION_PERIOD;
    yf_period = YF_PERIOD;

    pthread_t thread1, thread2, thread3;
    SharedData shared_data;
    shared_data.u = NULL;
    shared_data.yf = NULL;
    pthread_mutex_init(&shared_data.lock, NULL);

    // Create threads
    pthread_create(&thread1, NULL, generate_and_simulate, &shared_data);
    pthread_create(&thread2, NULL, sample_yf, &shared_data);
    pthread_create(&thread3, NULL, print_outputs_routine, &shared_data);

    usleep(simulation_time*1000);

    // Clean up
    pthread_cancel(thread1);
    pthread_cancel(thread2);
    pthread_cancel(thread3);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

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
