/* 
 * Write data in slot 8 (clear RW). And fill it with 0.
 */


#include <string.h>  // strlen, memcpy
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "device_cfg.h"

#define SLOT 8
#define MAX_LENGTH 416
#define DATA "EyC Original. 2024/03/22."

#define BLOCK_LEN 32

int main(void) {
    ATCA_STATUS status;
    size_t start, data_length, offset_bytes;

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }


    data_length = strlen(DATA);
    
    for (start = 0; start < MAX_LENGTH; start += BLOCK_LEN) {
        uint8_t buffer[BLOCK_LEN] = {0};

        if (start < data_length) {
            size_t bytes_to_copy = data_length - start;

            if (bytes_to_copy > BLOCK_LEN)
                bytes_to_copy = BLOCK_LEN;

            memcpy(buffer, DATA + start, bytes_to_copy);
        }

        printf("%4d:  ", start);
        for (int i = 0; i <= 31; i++) printf("%02x ", buffer[i]);
        puts("");
        
        status = atcab_write_bytes_zone(ATCA_ZONE_DATA, SLOT, start, buffer, BLOCK_LEN);
    }

    if (status != ATCA_SUCCESS) {
        printf("Slot write error: %d\n", status);
        exit(1);
    }

    puts("Ok");

}

