/*
 ========================================================================
 Name        : pthreads.c
 Author      : Geert Jan Bex
 Description : test for pthreads
 ========================================================================
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int rank;
    int sleepTime;
} Params;

void *compute(void *ptr);

int main(int argc, char *argv[]) {
    pthread_t *thread = NULL;
    Params *params = NULL;
    int i, status, sleepTime = 0;
    srand(12345);
    printf("started %s\n", argv[0]);
    if (argc < 2)
        errx(EXIT_FAILURE, "expected argument: nrTreads");
    int nrThreads = atoi(argv[1]);
    if (argc > 2)
        sleepTime = atoi(argv[2]);
    if (!(thread = (pthread_t *) calloc(nrThreads, sizeof(pthread_t))))
        errx(EXIT_FAILURE, "can't allocate threads array");
    if (!(params = (Params *) calloc(nrThreads, sizeof(Params))))
        errx(EXIT_FAILURE, "can't allocate parameter array");
    for (i = 0; i < nrThreads; i++) {
        params[i].rank = i;
        params[i].sleepTime = sleepTime;
        if(status = pthread_create(&thread[i], NULL, compute,
                                   (void *) &params[i]))
            errx(EXIT_FAILURE, "pthread_create returned %d", status);
    }
    for (i = 0; i < nrThreads; i++) {
        pthread_join(thread[i], NULL);
        printf("thread %d finished normally\n", i);
    }
    printf("ended %s\n", argv[0]);
    free(thread);
    free(params);
    pthread_exit(NULL);
}

void *compute(void *ptr) {
    Params *p = (Params *) ptr;
    printf("hello from %d\n", p->rank);
    sleep(p->sleepTime);
    printf("%d done sleeping\n", p->rank);
    pthread_exit(NULL);
}

