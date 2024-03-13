// gcc -I../cryptoauthlib/lib -L../cryptoauthlib/lib -lcryptoauth -o random random.c
//

#include <stdio.h>
#include "cryptoauthlib.h"
#include "atca_config.h"

int main(void) {
    uint8_t random_number[32];
    uint8_t buffer[1024];
    size_t buffer_size = sizeof(buffer);

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&atecc608_i2c) != ATCA_SUCCESS) {
       printf("Init error\n");
       exit(1);
    }

    atcab_random(random_number); // get a random number from the chip

    atcab_bin2hex(random_number, sizeof(random_number), buffer, &buffer_size);

    puts("");
    puts("Random output:");
    puts(buffer);
    puts("");
}

