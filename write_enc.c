/* 
 * Write data using a write key.
 */


#include <string.h>  // strlen
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    size_t foo;
    uint16_t slot, keyid;
    uint8_t data[ATCA_BLOCK_SIZE]   = {0};
    uint8_t key[ATCA_BLOCK_SIZE]    = {0};
    uint8_t numin[NONCE_NUMIN_SIZE] = {0};
    
    if (argc != 6) {
        printf("Write data using a write key.\n");
        printf("Usage %s <slot> <data> <keyid> <key> <numin>\n", argv[0]);
        printf("Where:\n");
        printf("    <slot> is the destination slot (0-15)\n");
        printf("    <data> is the data to write (%d hex bytes)\n", ATCA_BLOCK_SIZE);
        printf("    <keyid> is the key slot (0-15)\n");
        printf("    <key> is the writing key (%d hex bytes)\n", ATCA_BLOCK_SIZE);
        printf("    <numin> is the challenge for nonce (%d hex bytes)\n",NONCE_NUMIN_SIZE);
        exit(2);
    }

    /* Parameters validation */
    slot = atoi(argv[1]);
    if (slot < 0 || slot > 15) {
        puts("Slot must be between 0 and 15.");
        exit(2);
    }

    keyid = atoi(argv[3]);
    if (keyid < 0 || keyid > 15) {
        puts("KeyID must be between 0 and 15.");
        exit(2);
    }

    if (strlen(argv[2]) != 2*ATCA_BLOCK_SIZE) {
        printf("Data not valid. Must be %d bytes hex.\n", ATCA_BLOCK_SIZE);
        exit(2);
    }

    if (strlen(argv[4]) != 2*ATCA_BLOCK_SIZE) {
        printf("Key not valid. Must be %d bytes hex.\n", ATCA_BLOCK_SIZE);
        exit(2);
    }

    if (strlen(argv[5]) != 2*NONCE_NUMIN_SIZE) {
        printf("Nonce challenge not valid. Must be %d bytes hex.\n", NONCE_NUMIN_SIZE);
        exit(2);
    }

    foo = ATCA_BLOCK_SIZE;
    status = atcab_hex2bin(argv[2], 2*ATCA_BLOCK_SIZE, data, &foo);

    if (status != ATCA_SUCCESS) {
        printf("Malformed Data: %d\n", status);
        exit(2);
    }
    
    foo = ATCA_BLOCK_SIZE;
    status = atcab_hex2bin(argv[4], 2*ATCA_BLOCK_SIZE, key, &foo);

    if (status != ATCA_SUCCESS) {
        printf("Malformed Key: %d\n", status);
        exit(2);
    }

    foo = NONCE_NUMIN_SIZE;
    status = atcab_hex2bin(argv[5], 2*NONCE_NUMIN_SIZE, numin, &foo);

    if (status != ATCA_SUCCESS) {
        printf("Malformed Nonce challenge: %d\n", status);
        exit(2);
    }


    /* Command */

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    status = atcab_write_enc(slot, 0, data, key, keyid, numin); 

    if (status != ATCA_SUCCESS) {
        printf("Write encrypted error: %d\n", status);
        exit(1);
    }

    puts("Ok");
}

