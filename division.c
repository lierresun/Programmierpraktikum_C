#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define ERR(arg) ({ fprintf(stderr, "Error: invalid argument %s\n\r", arg); exit(1); })
#define ZERO_ERR() ({ fprintf(stderr, "Error: division by zero is not allowed\n\r"); exit(1); })

#define QUOTIENT 0
#define REMAINDER 1

uint8_t div8(uint8_t a, uint8_t b, uint8_t result[2]) {
    if (b == 0) { ZERO_ERR(); }

    result[QUOTIENT] = 0;
    result[REMAINDER] = a;

    //!TODO
    uint16_t rem = 0;          
    uint8_t  q   = 0;

    for (int i = 7; i >= 0; --i) {
        rem = (uint16_t)((rem << 1) | ((a >> i) & 1u));

        if (rem >= b) {
            rem -= b;
            q = (uint8_t)((q << 1) | 1u);   
        } else {
            q = (uint8_t)(q << 1);          
        }
    }

    result[QUOTIENT]  = q;
    result[REMAINDER] = (uint8_t)rem;

    
    return result[QUOTIENT];
}

int main(int argc, char* argv[]) {
    uint8_t a = 0, b = 0;
    uint8_t result[2];
    int temp;

    if (argc != 3) {
        a = 37; //!TODO1
        b = 5;  //!TODO2
    } else {
        if (sscanf(argv[1], "%d", &temp) != 1 || temp < 0 || temp > 255) { ERR(argv[1]); }
        a = (uint8_t)(temp);
        if (sscanf(argv[2], "%d", &temp) != 1 || temp < 0 || temp > 255) { ERR(argv[2]); }
        b = (uint8_t)(temp);
    }

    div8(a, b, result);

    printf("%u R: %u\n\r", result[QUOTIENT], result[REMAINDER]);

    return 0;
}
