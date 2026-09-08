#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    float x = 0.0;

    if (argc != 2) {
        x = 0.5;
    } else {
        x = atof(argv[1]);
    }

    if (x <= -1 || x >= 1) {
        fprintf(stderr, "Die Reihe konvergiert nur fuer |x| < 1.\n\r");
        return 1;
    }

    float result = 0.0;
    float term = x;

    //!TODO
    float sign = 1.0f;  
    for (int i = 1; i <= 5; i++) {
        result += sign * (term / i);
        term *= x;       
        sign = -sign;    
    }
    printf("ln(%f) = %f\n\r", x + 1, result);

    return 0;
}
