/* 
 * Create a new key from a parent, itself and tempkey.
 */


#include <string.h>  // strlen
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    uint8_t mode;
    uint16_t keyid;
    uint8_t mac[DERIVE_KEY_MAC_SIZE] = {0};

    if (argc < 3 || argc > 4) {
        printf("Usage %s <keyid> <mode> [mac]\n", argv[0]);
        printf("Where:\n");
        printf("    <keyid> is the target key slot (0-15).\n");
        printf("    <mode> is 0 if Tempkey source is random nonce and 1 otherwise.\n");
        printf("    <mac> is an optional MAC to validate operation (%d hexadecimal bytes).\n", DERIVE_KEY_MAC_SIZE);
        printf("Ex.: %s 5 1 0000000000000000000000000000000000000000000000000000000000000000\n", argv[0]);
        exit(2);
    }
    
    keyid = atoi(argv[1]);

    if (keyid < 0 || keyid > 15) {
        puts("KeyID must be between 0 and 15.");
        exit(2);
    }

    mode = atoi(argv[2]);
    
    if (mode != 0 && mode != 1) {
        puts("Mode must be 0 or 1.");
        exit(2);
    }

    mode <<= 2;
    
    if (argc == 4) {
        if (strlen(argv[3]) != 2*DERIVE_KEY_MAC_SIZE) {
            printf("MAC length must be %d hex digits.\n", DERIVE_KEY_MAC_SIZE);
            exit(2);
        }

        size_t macsize = DERIVE_KEY_MAC_SIZE;
        status = atcab_hex2bin(argv[3], strlen(argv[3]), mac, &macsize);

        if (status != ATCA_SUCCESS) {
            printf("Malformed MAC: %d\n", status);
            exit(1);
        }

    }

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    // Issue command
    status = atcab_derivekey(mode, keyid, mac);

    if (status != ATCA_SUCCESS) {
        printf("DeriveKey error: %d\n", status);
        exit(1);
    }

    puts("Ok");
}

