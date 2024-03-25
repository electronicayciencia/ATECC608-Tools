/* 
 * Get two hexadecimal strings and return the xor.
 */


#include <string.h>  // strlen
#include <stdio.h>   // printf
#include <stdlib.h>  // exit

int a2v(char c) {
    if ((c >= '0') && (c <= '9')) return c - '0';
    if ((c >= 'a') && (c <= 'f')) return c - 'a' + 10;
    if ((c >= 'A') && (c <= 'F')) return c - 'A' + 10;
    else {
        fflush(stdout);
        fprintf(stderr, "\nBad hexadecimal character '%c'.\n", c);
        exit(2);
    }
}

char v2a(int c) {
    const char hex[] = "0123456789abcdef";
    return hex[c];
}

int main(int argc, const char **argv) {
    int i;  

    if (argc != 3) {
        printf("Get two hexadecimal strings and return the xor.\n");
        printf("Usage %s <hextringA> <hextringB>\n", argv[0]);
        printf("Ex.: %s a1f6258c877d5fcd8964484538bfc92c a1f6258c877d5fcd8964484538bfc92c\n", argv[0]);
        exit(1);
    }

    if (strlen(argv[2]) != strlen(argv[1])) {
        fprintf(stderr, "Both strings must be the same length.\n");
        exit(1);
    }


    for (i = 0; i < strlen(argv[1]); i++) {
        int a = a2v(argv[1][i]);
        int b = a2v(argv[2][i]);
        int x = a^b;
        putchar(v2a(x));
    }

    puts("");
}

