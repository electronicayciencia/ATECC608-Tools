/* 
 * gcc -I../cryptoauthlib/lib -L../cryptoauthlib/lib -lcryptoauth sha.c
 *
 * $ echo -en "0123456789" | sha256sum
 * 84d89877f0d4041efb6bf91a16f0248f2fd573e6af05c19f96bedb9f882f7882  -
 *
 */


#include <string.h>  // strlen
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(void) {
    uint8_t message[] = "0123456789";
    uint8_t digest[32];
    uint8_t buffer[1024];
    size_t buffer_size = sizeof(buffer);

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    if (atcab_sha(strlen(message), message, digest) != ATCA_SUCCESS) {
        printf("SHA error\n");
        exit(1);
    }

    atcab_bin2hex(digest, sizeof(digest), buffer, &buffer_size);

    puts("");
    puts("SHA256 output:");
    puts(buffer);
    puts("");

}

