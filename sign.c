/* 
 * Sign tempkey using ECDSA algorithm with the private key in the given slot.
 */


#include <stdio.h>   // printf
#include <stdlib.h>  // atoi

#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    uint8_t slotid;
    uint8_t signature[ATCA_SIG_SIZE];

    if (argc != 2) {
        puts("Sign Tempkey using ECDSA algorithm with the private key in the slot.");
        printf("Usage %s <SlotID>\n", argv[0]);
        printf("Where <SlotID> is the slot number with the private key (0-15).\n");
        printf("Ex.: %s 0\n", argv[0]);
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

    // Sign
    uint8_t mode = SIGN_MODE_EXTERNAL | SIGN_MODE_SOURCE_TEMPKEY;
    status = atcab_sign_base(mode, slotid, signature);
    
    if (status != ATCA_SUCCESS) {
        printf("Sign error: %d\n", status);
        exit(1);
    }

    puts("Ok");
    printhex("R", signature, 32, "");
    printhex("S", signature+32, 32, "");
}

