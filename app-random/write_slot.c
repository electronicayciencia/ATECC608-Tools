/* 
 * Write 32 bytes of data to any slot.
 */


#include <string.h>  // strlen, memcpy
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "device_cfg.h"

#define SLOT 6

#define BLOCK_LEN 32

int main(void) {
    ATCA_STATUS status;
    size_t start, data_length, offset_bytes;
    uint8_t buffer[ATCA_BLOCK_SIZE] = {0};

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }


    
    status = atcab_write_bytes_zone(ATCA_ZONE_DATA, SLOT, 0, buffer, ATCA_BLOCK_SIZE);

    if (status != ATCA_SUCCESS) {
        printf("Slot write error: %d\n", status);
        exit(1);
    }

    puts("Ok");

}

