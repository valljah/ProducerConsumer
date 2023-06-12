#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include "linkedlist.h"
#include "ansi_colors.h"

#define CONSUMER_N 4
#define PRODUCER_N 4
#define PRODUCED_PER_THREAD 10
#define DELAY ((rand() % 500000000L) + 100000000L)

node_t *global_list = NULL;
int length_list = 0;
pthread_cond_t global_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t global_mut = PTHREAD_MUTEX_INITIALIZER;

int is_prime(unsigned int n) {
    if (n == 2) return 1;
    if (n % 2 == 0 || n < 2) return 0;

    for (int i = 3; i < sqrt(n) + 1; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

void *producer_fun(void *args) {
    int *id = (int *) args;
    struct timespec current, end, sleep_duration;
    int rand_value;

    while (1) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &current);
        srand(current.tv_nsec);

        pthread_mutex_lock(&global_mut);

        rand_value = rand() % 100000 + 1;
        global_list = insert_front(global_list, rand_value);
        length_list++;

        pthread_mutex_unlock(&global_mut);
        pthread_cond_signal(&global_cond);

        sleep_duration.tv_nsec = DELAY;
        nanosleep(&sleep_duration, NULL);

        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        long time_taken = (end.tv_sec - current.tv_sec) * 1000L + (end.tv_nsec - current.tv_nsec) / 1000000L;
        printf(BHBLU"Producer "CRESET BHCYN"%d"CRESET " took " BHMAG"%ld ms"CRESET " to produce " BYEL"%d\n"CRESET,
               *id, time_taken, rand_value);
        fflush(stdout);
    }

    return (void *) id;
}

void *consumer_fun(void *args) {
    int *id = (int *) args;
    int current_value;
    while (1) {
        if (length_list == 0) continue;
        pthread_mutex_lock(&global_mut);
        pthread_cond_wait(&global_cond, &global_mut);

        current_value = pop_back(global_list);
        length_list--;

        pthread_mutex_unlock(&global_mut);
        printf(BHCYN"\tConsumer "CRESET BBLU"%d"CRESET": " BMAG"%d"CRESET " is %s\n",
               *id, current_value,
               (is_prime(current_value)) ? BHGRN"prime"CRESET : BHRED"not prime"CRESET);
        fflush(stdout);
    }
    return (void *) id;
}

int main() {
    pthread_t consumers[CONSUMER_N];
    pthread_t producers[PRODUCER_N];

    for (int i = 0; i < PRODUCER_N; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&producers[i], NULL, &producer_fun, id);
    }
    for (int i = 0; i < CONSUMER_N; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&consumers[i], NULL, &consumer_fun, id);
    }

    for (int i = 0; i < PRODUCER_N; i++) {
        int *result;
        pthread_join(producers[i], (void **) &result);
        free(result);
    }

    for (int i = 0; i < CONSUMER_N; i++) {
        int *result;
        pthread_join(consumers[i], (void **) &result);
        free(result);
    }
}
