#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR(msg) ({fprintf(stderr, "\033[1;31mError:\033[0m %s!\n\r", msg); return 1;})
#define USG(msg) ({fprintf(stderr, "\033[1;31mUsage:\033[0m %s \n\r", msg); return 1;})

int main(int argc, char* argv[]) {
    int m, n, k = 0;
    char* input;

    if (argc < 2) {
        m = 9;
        n = 8;
        char default_riddle[] =
            "XXFFXFOOO"
            "XXXXOXXFX"
            "FOOOXOXXX"
            "OXOXOOOOO"
            "FXFOFXOOO"
            "XOOXFFXXX"
            "FXFOXOOOX"
            "XOOXOOOXO";
        input = default_riddle;
    } else {
        if (argc < 4) { USG("[cols] [rows] [str]"); }
        if (sscanf(argv[1], "%d", &m) != 1 || m <= 0) { ERR("Invalid number of columns"); }
        if (sscanf(argv[2], "%d", &n) != 1 || n <= 0) { ERR("Invalid number of rows"); }
        input = argv[3];
    }

    int N = m * n;
    int len = strlen(input);
    char* data = malloc(N + 1);
    if (!data) { ERR("Allocation failed"); }
    strncpy(data, input, N);
    if (len < N) { memset(data + len, 'O', N - len); }
    data[N] = '\0';
    char grid[n][m];

    //!TODO1
    for (int i = 0; i < n; ++i) {        // i: Zeile
        for (int j = 0; j < m; ++j) {    // j: Spalte
            grid[i][j] = data[i * m + j];
        }
    }
    
    free(data);

    //!TODO2
    for (int i = 0; i < n; ++i) {           
        for (int j = 0; j < m; ++j) {       
            if (j + 2 < m &&
                grid[i][j] == 'F' &&
                grid[i][j + 1] == 'O' &&
                grid[i][j + 2] == 'X') {
                printf("\033[1;32m(%d, %d, H)\033[0m\n\r", i, j);
                return 0;
            }

            if (i + 2 < n &&
                grid[i][j] == 'F' &&
                grid[i + 1][j] == 'O' &&
                grid[i + 2][j] == 'X') {
                printf("\033[1;32m(%d, %d, V)\033[0m\n\r", i, j);
                return 0;
            }
        }
    }

    // Use printf("\033[1;32m(%d, %d, H)\033[0m\n\r", i, j);
    // USe printf("\033[1;32m(%d, %d, V)\033[0m\n\r", i, j);

    printf("\033[1;31mNo Fox contained.\033[0m\n\r");
    return 0;
}
