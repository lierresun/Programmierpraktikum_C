#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ERR() ({ fprintf(stderr, "Usage: %s <N>\n", argv[0]); exit(1); })

static double* sequence(size_t N, double r) {
    double* a = malloc(N * sizeof(double));
    if (!a) { perror("malloc"); exit(1); }
    double val = 1.0;
    for (size_t i = 0; i < N; i++) {
        a[i] = val;
        val *= r;
    }
    return a;
}

static double sum_ascending(double* a, size_t N) {
    double s = 0.0;
    //!TODO1
    for (size_t i = 0; i < N; i++) {
        s += a[i];
    }
    return s;
}

static double sum_descending(double* a, size_t N) {
    double s = 0.0;
    //!TODO2
    for (size_t i = N; i-- > 0; ) {
        s += a[i];
    }
    return s;
}

static double kahan(double* X, size_t N) {
    double S = 0.0, E = 0.0, Y = 0.0, Z = 0.0;
    size_t i;

    //!TODO3
    for (size_t i = 0; i < N; i++) {
        Y = X[i] - E;      
        Z = S + Y;         
        E = (Z - S) - Y;   
        S = Z;             
    }

    return S;
}

static double exact_sum(double r, size_t N) { return (1.0 - pow(r, (double)(N))) / (1.0 - r); }

int main(int argc, char* argv[]) {
    size_t N;
    if (argc > 2) { ERR(); }
    if (argc == 1) {
        N = 100; //!TODO
    } else {
        long tmp = atol(argv[1]);
        if (tmp <= 0) { ERR(); }
        N = (size_t)(tmp);
    }

    double r = 0.7;
    
    /*
    printf("N;Ascending Error;Descending Error;Kahan Error\n\r");
    for (int M = 10; M <= N; M = M + 1) {
    int N = M;
    */

    double* a = sequence(N, r);

    double s_up   = sum_ascending(a, N);
    double s_down = sum_descending(a, N);
    double s_kahan = kahan(a, N);
    double S = exact_sum(r, N);

    free(a);

    printf("Ascending sum:\t  \033[1;33m%.17g\033[0m\t\033[1;31m[Error: %3.3e]\033[0m\n\r", s_up, s_up - S);
    printf("Descending sum:\t  \033[1;33m%.17g\033[0m\t\033[1;31m[Error: %3.3e]\033[0m\n\r", s_down, s_down - S);
    printf("Kahan sum:\t  \033[1;33m%.17g\033[0m\t\033[1;31m[Error: %3.3e]\033[0m\n\r", s_kahan, s_kahan - S);
    printf("\n\rActual sum:\t  \033[1;32m%.17g\033[0m\n\r", S);

    //printf("%d;%.17g;%.17g;%.17g\n\r", N, s_up - S, s_down - S, s_kahan - S);

    //}

    return 0;
}
