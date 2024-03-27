/* 
 * Load tempkey with a given value.
 */


#include <string.h>  // strlen
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    size_t binsize = NONCE_NUMIN_SIZE_PASSTHROUGH;
    uint8_t num_in[NONCE_NUMIN_SIZE_PASSTHROUGH] = {0};

    if (argc != 2 || strlen(argv[1]) != 2*NONCE_NUMIN_SIZE_PASSTHROUGH) {
        printf("Usage %s <value>\n", argv[0]);
        printf("Where <value> is %d bytes of hex random data.\n", NONCE_NUMIN_SIZE_PASSTHROUGH);
        printf("Ex.: %s 0000000000000000000000000000000000000000000000000000000000000000\n", argv[0]);
        exit(2);
    }

    status = atcab_hex2bin(argv[1], 2*NONCE_NUMIN_SIZE_PASSTHROUGH, num_in, &binsize);

    if (status != ATCA_SUCCESS) {
        printf("Malformed data: %d\n", status);
        exit(1);
    }

    //printhex(NULL, num_in, NONCE_NUMIN_SIZE_PASSTHROUGH, "");

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    // Generate random Nonce
    status = atcab_nonce(num_in);

    if (status != ATCA_SUCCESS) {
        printf("Nonce error: %d\n", status);
        exit(1);
    }

    puts("Ok");
}

