/* 
 * Perform a Check MAC command that can also be used to authorize a key.
 */


#include <string.h>  // strlen
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

int main(int argc, const char **argv) {
    ATCA_STATUS status;
    size_t foo;
    uint16_t key_id;
    uint8_t challenge[CHECKMAC_CLIENT_CHALLENGE_SIZE];
    uint8_t response[CHECKMAC_CLIENT_RESPONSE_SIZE];
    uint8_t other_data[CHECKMAC_OTHER_DATA_SIZE];
    
    if (argc != 4) {
        printf("Perform a Check MAC command that can also be used to authorize a key.\n");
        printf("Usage %s <KeyId> <mac> <other_data>\n", argv[0]);
        printf("Where:\n");
        printf("    <KeyId> is the key slot (0-15)\n");
//        printf("    <challenge> is the challenge used in the Nonce call (%d hex bytes).\n", CHECKMAC_CLIENT_CHALLENGE_SIZE);
        printf("    <mac> is the expected MAC or the client response (%d hex bytes).\n", CHECKMAC_CLIENT_RESPONSE_SIZE);
        printf("    <other_data> is aditional data like the serial number (%d hex bytes).\n", CHECKMAC_OTHER_DATA_SIZE);
        exit(2);
    }

    key_id = atoi(argv[1]);

    /* Parameters validation */
    if (key_id < 0 || key_id > 15) {
        puts("Key ID must be between 0 and 15.");
        exit(2);
    }

//    if (strlen(argv[2]) != 2*CHECKMAC_CLIENT_CHALLENGE_SIZE) {
//        printf("Challenge not valid. Must be %d bytes hex.\n", CHECKMAC_CLIENT_CHALLENGE_SIZE);
//        exit(2);
//    }

    if (strlen(argv[2]) != 2*CHECKMAC_CLIENT_RESPONSE_SIZE) {
        printf("MAC not valid. Must be %d bytes hex.\n", CHECKMAC_CLIENT_RESPONSE_SIZE);
        exit(2);
    }

    if (strlen(argv[3]) != 2*CHECKMAC_OTHER_DATA_SIZE) {
        printf("Data not valid. Must be %d bytes hex.\n", CHECKMAC_OTHER_DATA_SIZE);
        exit(2);
    }

    /* Parameters input */
//    foo = CHECKMAC_CLIENT_CHALLENGE_SIZE;
//    status = atcab_hex2bin(argv[2], 2*CHECKMAC_CLIENT_CHALLENGE_SIZE, challenge, &foo);
//
//    if (status != ATCA_SUCCESS) {
//        printf("Malformed challenge data: %d\n", status);
//        exit(2);
//    }
    
    foo = CHECKMAC_CLIENT_RESPONSE_SIZE;
    status = atcab_hex2bin(argv[2], 2*CHECKMAC_CLIENT_RESPONSE_SIZE, response, &foo);

    if (status != ATCA_SUCCESS) {
        printf("Malformed MAC data: %d\n", status);
        exit(2);
    }
    
    foo = CHECKMAC_OTHER_DATA_SIZE;
    status = atcab_hex2bin(argv[3], 2*CHECKMAC_OTHER_DATA_SIZE, other_data, &foo);

    if (status != ATCA_SUCCESS) {
        printf("Malformed other data: %d\n", status);
        exit(2);
    }


    /* Command */

    //printhex("Cleartext", cleartext, textsize, "");

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    status = atcab_checkmac(CHECKMAC_MODE_BLOCK2_TEMPKEY, key_id, challenge, response, other_data);

    if (status != ATCA_SUCCESS) {
        printf("Check MAC error: %d\n", status);
        exit(1);
    }

    puts("Ok");
}

