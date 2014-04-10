#include <err.h>
#include <stdio.h>
#include <stdlib.h>

long fib(int n);

int main(int argc, char *argv[]) {
    double sum = 0.0;
    int i, n, max;
    if (argc != 3)
	errx(EXIT_FAILURE, "expecting n and max");
    n = atoi(argv[1]);
    max = atoi(argv[2]);
#pragma omp parallel for private(i) schedule(dynamic, 1)
    for (i = 0; i < n; i++) {
#pragma omp atomic
        sum += fib(rand() % max);
    }
    printf("sum = %lf\n", sum);
    return EXIT_SUCCESS;
}

long fib(int n) {
    if (n <= 1)
        return 1;
    else
        return fib(n-1) + fib(n-2);
}

