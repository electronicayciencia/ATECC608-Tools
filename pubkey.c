/* 
 * Issue a genkey-slot command to calculate the public key from an existing
 * private key in a slot.
 */


#include <stdio.h>   // printf
#include <stdlib.h>  // atoi

#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    uint8_t slotid;
    uint8_t public_key[ATCA_PUB_KEY_SIZE];

    if (argc != 2) {
        puts("Calculate the public key from an existing private key in a slot.");
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

    // Genkey-slot
    status = atcab_get_pubkey(slotid, public_key);
    
    if (status != ATCA_SUCCESS) {
        printf("GenKey error: %d\n", status);
        exit(1);
    }

    puts("Ok");

    puts("Curve is P-256.");
    puts("Public key parameters:");
    printhex("X", public_key, 32, "");
    printhex("Y", public_key+32, 32, "");
}

