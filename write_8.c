/* 
 * Write data in slot 8 (clear RW). And fill it with 0.
 */


#include <string.h>  // strlen, memcpy
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

#define SLOT 8
#define MAX_LENGTH 416
//#define DATA "EyC Original. 2024/03/22."

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    size_t start, data_length, offset_bytes;

    if (argc != 2) {
        printf("Write %d bytes to the slot %d.\n", MAX_LENGTH, SLOT);
        printf("Usage %s <string>\n", argv[0]);
        printf("Where:\n");
        printf("    <string> is the data to write (%d bytes).\n", ATCA_BLOCK_SIZE);
        printf("Ex.: %s \"EyC Original. 2024/03/22.\"\n", argv[0]);
        exit(2);
    }

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }


    data_length = strlen(argv[1]);
    
    for (start = 0; start < MAX_LENGTH; start += ATCA_BLOCK_SIZE) {
        uint8_t buffer[ATCA_BLOCK_SIZE] = {0};

        if (start < data_length) {
            size_t bytes_to_copy = data_length - start;

            if (bytes_to_copy > ATCA_BLOCK_SIZE)
                bytes_to_copy = ATCA_BLOCK_SIZE;

            memcpy(buffer, argv[1] + start, bytes_to_copy);
        }

        printf("%4d:  ", start);
        for (int i = 0; i <= 31; i++) printf("%02x ", buffer[i]);
        puts("");
        
        status = atcab_write_bytes_zone(ATCA_ZONE_DATA, SLOT, start, buffer, ATCA_BLOCK_SIZE);

        if (status != ATCA_SUCCESS) {
            printf("Slot write error: %d\n", status);
            exit(1);
        }
    }

    puts("Ok");

}

