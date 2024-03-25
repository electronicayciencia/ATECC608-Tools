/* 
 * Issue a gendig-data command with the specified slot.
 */


#include <stdio.h>   // printf
#include <stdlib.h>  // atoi

#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    uint8_t slotid;

    if (argc != 2) {
        puts("Issue a gendig-data command with the specified slot.");
        printf("Usage %s <SlotID>\n", argv[0]);
        printf("Where <SlotID> is the slot number with the data (0-15).\n");
        printf("Ex.: %s 8\n", argv[0]);
        exit(2);
    }

    slotid = atoi(argv[1]);

    if (slotid < 0 || slotid > 15) {
        puts("Slot id not valid.");
        exit(2);
    }

    printf("Slot id %d\n", slotid);

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    // Gendig-data
    status = atcab_gendig(GENDIG_ZONE_DATA, slotid, NULL, 0);
    
    if (status != ATCA_SUCCESS) {
        printf("GenDig error: %d (need nonce?)\n", status);
        exit(1);
    }

    puts("Ok");
}

