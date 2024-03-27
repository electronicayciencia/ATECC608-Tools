/* 
 * Generate a random nonce with a challenge.
 */


#include <string.h>  // strlen
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    size_t binsize = NONCE_NUMIN_SIZE;
    uint8_t num_in[NONCE_NUMIN_SIZE] = {0}; // timestamp or random or counter
    uint8_t rand_out[RANDOM_NUM_SIZE];  // output from nonce

    if (argc != 2 || strlen(argv[1]) != 2*NONCE_NUMIN_SIZE) {
        printf("Usage %s <challenge>\n", argv[0]);
        printf("Where <challenge> is %d bytes of hex random data or counter.\n", NONCE_NUMIN_SIZE);
        printf("Ex.: %s 0000000000000000000000000000000000000000\n", argv[0]);
        exit(2);
    }

    status = atcab_hex2bin(argv[1], 2*NONCE_NUMIN_SIZE, num_in, &binsize);

    if (status != ATCA_SUCCESS) {
        printf("Malformed challenge: %d\n", status);
        exit(1);
    }

    //printhex(NULL, num_in, NONCE_NUMIN_SIZE, "");

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    // Generate random Nonce
    status = atcab_nonce_rand(num_in, rand_out);

    if (status != ATCA_SUCCESS) {
        printf("Nonce error: %d\n", status);
        exit(1);
    }

    printhex(NULL, rand_out, sizeof(rand_out), "");
}

