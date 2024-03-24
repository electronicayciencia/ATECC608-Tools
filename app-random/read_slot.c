/* 
 * Clear read 32 bits of slot data
 */


#include <stdio.h>   // printf
#include <stdlib.h>  // atoi

#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    uint8_t slotid;
    uint8_t slot_data[32];

    if (argc != 2) {
        puts("Read clear the first 32 bits from a data slot");
        printf("Usage %s <SlotID>\n", argv[0]);
        printf("Where <SlotID> is the slot number (0-15). Must be readable.\n");
        printf("Ex.: %s 8\n", argv[0]);
        exit(2);
    }

    slotid = atoi(argv[1]);

    if (slotid < 0 || slotid > 15) {
        puts("Slot id not valid.");
        exit(2);
    }

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    // Try to read data slot contents
    status = atcab_read_bytes_zone(ATCA_ZONE_DATA, slotid, 0, slot_data, 32);
    
    if (status != ATCA_SUCCESS) {
        printf("MAC error: %d (is secret?)\n", status);
        exit(1);
    }

    printhex(NULL, slot_data, 32, "");
}

