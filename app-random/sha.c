/* 
 * gcc -I../cryptoauthlib/lib -L../cryptoauthlib/lib -lcryptoauth sha.c
 *
 * $ echo -en "0123456789" | sha256sum
 * 84d89877f0d4041efb6bf91a16f0248f2fd573e6af05c19f96bedb9f882f7882  -
 *
 */


#include "cryptoauthlib.h"
#include <string.h>  // strlen
#include <stdio.h>   // printf

int main(void) {
    uint8_t digest[32];
    uint8_t message[] = "0123456789";

    ATCAIfaceCfg mycfg_i2c = {
        .iface_type                 = ATCA_I2C_IFACE,
        .devtype                    = ATECC608,
        {
            .atcai2c.address        = (0x60 << 1),
            .atcai2c.bus            = 1, // /dev/i2c-1
            .atcai2c.baud           = 100000,
        },
        .wake_delay                 = 1500,
        .rx_retries                 = 20
    };

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&mycfg_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    if (atcab_sha(strlen(message), message, digest) != ATCA_SUCCESS) {
        printf("SHA error\n");
        exit(1);
    }

    for(int i = 0; i < 32; i++) {
        printf("%02x ", digest[i]);
    }
    printf("\n");

}

