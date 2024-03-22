/* 
 *
 */


#include <string.h>  // strlen
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "device_cfg.h"

#define SLOT 3

int main(void) {
    ATCA_STATUS status;
    uint8_t cleartext[ATCA_AES128_BLOCK_SIZE] = "0123456789ABCDEF";
    uint8_t ciphertext[ATCA_AES128_BLOCK_SIZE];

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    printf("Cleartext: ");
    for (int i = 0; i < ATCA_AES128_BLOCK_SIZE; i++) 
        printf("%02x ", cleartext[i]);
    puts("");

    status = atcab_aes_encrypt (SLOT, 0, cleartext, ciphertext);

    if (status != ATCA_SUCCESS) {
        printf("AES error: %d\n", status);
        exit(1);
    }

    printf("Ciphertext: ");
    for (int i = 0; i < ATCA_AES128_BLOCK_SIZE; i++)
        printf("%02x ", ciphertext[i]);
    puts("");

}

