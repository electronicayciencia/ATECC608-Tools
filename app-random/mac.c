/* 
 * Generate a MAC of the TempKey + Slot Key
 */


#include <stdio.h>   // printf
#include <stdlib.h>  // atoi

#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

#define SLOT 3

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    uint8_t slotid;
    uint8_t digest[MAC_SIZE];           // output from MAC calculation

    if (argc != 2) {
        puts("Calculate diversified MAC using KeyID and TempKey");
        printf("Usage %s <SlotID>\n", argv[0]);
        printf("Where <SlotID> is the slot number with the shared secret (0-15).\n");
        printf("Ex.: %s 14\n", argv[0]);
        exit(2);
    }

    slotid = atoi(argv[1]);

    if (slotid < 0 || slotid > 15) {
        puts("Slot id not valid.");
        exit(2);
    }

    //printf("Slot id %d\n", slotid);

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    // Calculate MAC
    status = atcab_mac(MAC_MODE_BLOCK2_TEMPKEY | MAC_MODE_INCLUDE_SN, slotid, NULL, digest);
    
    if (status != ATCA_SUCCESS) {
        printf("MAC error: %d\n", status);
        exit(1);
    }

    printhex(NULL, digest, sizeof(digest), "");
}

