
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "simulation/simulation_facade.h"

#define PERIOD_U 30 // 30ms
#define PERIOD_YF 50 // 50ms

pthread_mutex_t mutex;
Matrix *u_data = NULL;
Matrix *y_f_data = NULL;

void *generate_u(void *args) {
    for (int t = 0; t <= 20; t++) {
        pthread_mutex_lock(&mutex);
        u_data = u_t(t); // Generate u(t)
        pthread_mutex_unlock(&mutex);
        usleep(PERIOD_U * 1000); // Sleep for 30ms
    }
    pthread_exit(NULL);
}

void *generate_yf(void *args) {
    double D = 1.0; // Example diameter of the robot
    for (int t = 0; t <= 20; t++) {
        pthread_mutex_lock(&mutex);
        if (u_data != NULL) {
            Matrix *y = simulate_yf(u_data, D); // Compute y_f(t)
            y_f_data = y;
            print_outputs(t, u_data, y); // Print or save output
            destroy_matrix(y);
        }
        pthread_mutex_unlock(&mutex);
        usleep(PERIOD_YF * 1000); // Sleep for 50ms
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread_u, thread_yf;
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    pthread_create(&thread_u, NULL, generate_u, NULL);
    pthread_create(&thread_yf, NULL, generate_yf, NULL);

    // Wait for threads to finish
    pthread_join(thread_u, NULL);
    pthread_join(thread_yf, NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}
