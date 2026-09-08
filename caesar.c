#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ERR(msg) ({fprintf(stderr, "\033[1;31mError:\033[0m %s!\n\r", msg); return 1;})
#define USG(msg) ({fprintf(stderr, "\033[1;31mUsage:\033[0m %s \n\r", msg); return 1;})
#define MAXSTRLEN 1024

void caesarEncrypt(int shift, const char* text, char* encryptedText) {
    for (size_t i = 0; text[i] != '\0'; ++i) {
        //!TODO
        int s = ((shift % 26) + 26) % 26;
        unsigned char c = text[i];

        if (c >= 'A' && c <= 'Z') {
            encryptedText[i] = 'A' + ( (c - 'A' + s) % 26 );
        } else if (c >= 'a' && c <= 'z') {
            encryptedText[i] = 'a' + ( (c - 'a' + s) % 26 );
        } else {
            encryptedText[i] = c;
        }
    } encryptedText[strlen(text)] = '\0';
}

int main(int argc, char* argv[]) {
    char text[MAXSTRLEN] = "Test"; //!TODO1
    int shift = 13; //!TODO2

    if (argc > 1) {
        char* endptr;
        shift = (int)strtol(argv[1], &endptr, 10);
        if (*endptr != '\0') {
            ERR("Invalid input for shift");
        }

        if (argc > 2) {
            text[0] = '\0';
            for (int i = 2; i < argc; ++i) {
                strcat(text, argv[i]);
                if (i < argc - 1) {
                    strcat(text, " ");
                }
            }
        }
    }

    char encryptedText[MAXSTRLEN];
    caesarEncrypt(shift, text, encryptedText);
    printf("%s\n\r", encryptedText);

    return 0;
}
