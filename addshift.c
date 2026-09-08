#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    uint8_t a = 0;
    uint8_t b = 0;
    uint8_t result = 0;

    if (argc != 3) {
        a = 13;
        b = 11;
    } else {
        a = (uint8_t)(atoi(argv[1]));
        b = (uint8_t)(atoi(argv[2]));
    }

    //!TODO
    for (int i = 0; i < 8; i++) {
        if (b & 1) {          
            result += a;      
        }
        a <<= 1;              
        b >>= 1;              
    }
    printf("%d\n\r", (int)(result));

    return 0;
}