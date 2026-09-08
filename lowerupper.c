#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERR(msg) ({fprintf(stderr, "\033[1;31mError:\033[0m %s!\n\r", msg); return 1;})
#define USG(msg) ({fprintf(stderr, "\033[1;31mUsage:\033[0m %s \n\r", msg); return 1;})
#define MAXSTRLEN 1024

void str_to_lower(const char* text, char* result) {
    //!TODO1
    for (size_t i = 0; text[i] != '\0'; ++i) {
        char c = text[i];
        if (c >= 'A' && c <= 'Z') {
            result[i] = c + ('a' - 'A');   
        } else {
            result[i] = c;
        }
    }
    result[strlen(text)] = '\0';
}

void str_to_upper(const char* text, char* result) {
    //!TODO2
    for (size_t i = 0; text[i] != '\0'; ++i) {
        char c = text[i];
        if (c >= 'a' && c <= 'z') {
            result[i] = c - ('a' - 'A');   
        } else {
            result[i] = c;
        }
    }
    result[strlen(text)] = '\0';
}

int main(int argc, char* argv[]) {
    char text[MAXSTRLEN] = "HaLLo WeLt 123!"; //!TODO3

    if (argc > 1) {
        text[0] = '\0';
        for (int i = 1; i < argc; ++i) {
            if (strlen(text) + strlen(argv[i]) + 2 > sizeof(text)) {
                ERR("Input too long");
            }
            strcat(text, argv[i]);
            if (i < argc - 1) {
                strcat(text, " ");
            }
        }
    }

    char lower[MAXSTRLEN];
    char upper[MAXSTRLEN];

    str_to_lower(text, lower);
    str_to_upper(text, upper);

    printf("%s\n\r", lower);
    printf("%s\n\r", upper);

    return 0;
}
