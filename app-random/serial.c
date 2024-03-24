/* 
 * Get the chip's serial number.
 */

#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    uint8_t sn[ATCA_SERIAL_NUM_SIZE];

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    status = atcab_read_serial_number(sn);

    if (status != ATCA_SUCCESS) {
        printf("Nonce error: %d\n", status);
        exit(1);
    }

    printhex(NULL, sn, ATCA_SERIAL_NUM_SIZE, "");
}

