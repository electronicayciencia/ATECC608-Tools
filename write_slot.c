/* 
 * Write 32 bytes of data to any slot.
 */


#include <string.h>  // strlen
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    uint16_t slot;
    size_t foo;
    uint8_t data[ATCA_BLOCK_SIZE] = {0};

    if (argc != 3) {
        printf("Write a block bytes of data to any slot.\n");
        printf("Usage %s <slot> <data>\n", argv[0]);
        printf("Where:\n");
        printf("    <slot> is the key slot (0-15).\n");
        printf("    <data> is the data to encrypt (%d hex bytes).\n", ATCA_BLOCK_SIZE);
        printf("Ex.: %s 3 0000000000000000000000000000000000000000000000000000000000000000\n", argv[0]);
        exit(2);
    }


    slot = atoi(argv[1]);

    if (slot < 0 || slot > 15) {
        puts("Slot must be between 0 and 15.");
        exit(2);
    }
    
    if (strlen(argv[2]) != 2*ATCA_BLOCK_SIZE) {
        printf("Data non valid. Must be %d bytes hex.\n", ATCA_BLOCK_SIZE);
        exit(2);
    }

    foo = ATCA_BLOCK_SIZE;
    status = atcab_hex2bin(argv[2], 2*ATCA_BLOCK_SIZE, data, &foo);

    if (status != ATCA_SUCCESS) {
        printf("Malformed hex data: %d\n", status);
        exit(2);
    }
    

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    status = atcab_write_bytes_zone(ATCA_ZONE_DATA, slot, 0, data, ATCA_BLOCK_SIZE);

    if (status != ATCA_SUCCESS) {
        printf("Slot write error: %d\n", status);
        exit(1);
    }

    puts("Ok");

}

