/* 
 * Authenticate slot 8 using a shared secret and check that it starts by "EyC Original".
 */


#include <string.h>  // strlen, memcpy
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "device_cfg.h"


#include "host/atca_host.h"


#define SECRET_SLOT 6

#define ORIGINAL "EyC Original."

#define ATCA_SOURCEFLAG_RANDOM  (0)
#define ATCA_SOURCEFLAG_INPUT   (1)

ATCA_STATUS symmetric_authenticate(uint8_t slot, const uint8_t *master_key, const uint8_t *rand_number);


/* Versatile hex printer */
void printhex(const char *label, char *buffer, size_t len, const char *spacer) {
    printf("%s: ", label);
    
    for (int i = 0; i < len; i++)
        printf("%02x%s", buffer[i], spacer);

    puts("");
}


int main(void) {
    ATCA_STATUS status;
    uint8_t num_in[NONCE_NUMIN_SIZE] = {0}; // timestamp or random or counter
    uint8_t rand_out[NONCE_RSP_SIZE_LONG];  // output from nonce
    uint8_t digest[MAC_RSP_SIZE];           // output from MAC calculation
    uint8_t master_key[ATCA_BLOCK_SIZE] = {0};

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

//
//    // Generate random Nonce
//    status = atcab_nonce_rand(num_in, rand_out);
//
//    if (status != ATCA_SUCCESS) {
//        printf("Nonce error: %d\n", status);
//        exit(1);
//    }
//
//    printhex("Random", rand_out, sizeof(rand_out), "");
//
//    /*
//    TempKey should be now the SHA256 of:
//      - 32 bytes RandOut
//      - 20 bytes NumIn from input stream
//      - 1 byte Opcode (always 0x16)
//      - 1 byte Mode (0x00)
//      - 1 byte LSB of Param2 (should always be 0x00)
//    */
//
//    status = atcab_mac(MAC_MODE_BLOCK2_TEMPKEY, SECRET_SLOT, NULL, digest);
//    
//    if (status != ATCA_SUCCESS) {
//        printf("MAC error: %d\n", status);
//        exit(1);
//    }
//
//    printhex("MAC", digest, sizeof(digest), "");
//
//    /*
//    MAC consists in SHA256 of:
//      - 32 bytes Slot<KeyID>
//      - 32 bytes TempKey
//      - 1 byte Opcode (always 0x08)
//      - 1 byte Mode
//      - 2 bytes KeyID
//      - 8 bytes Zeros
//      - 3 bytes Zeros
//      - 1 byte  SN<8> 0x01
//      - 4 bytes zeros
//      - 2 bytes SN<0:1> 0x01 0x23
//      - 2 bytes zeros
//    */
//
//    puts("Ok");


    if (symmetric_authenticate(SECRET_SLOT, master_key, num_in) == ATCA_SUCCESS) {
        puts("Match");
    }
    else {
        puts("Not match");
    }

}


/** \brief Function which does the authentication between the host and device.
 *  \param[in]  slot                The slot number used for the symmetric authentication.
 *  \param[in]  master_key          The master key used for the calculating the symmetric key.
 *  \param[in]  rand_number         The 20 byte rand_number from the host.
 *  \return ATCA_SUCCESS on successful authentication, otherwise an error code.
 */

ATCA_STATUS symmetric_authenticate(uint8_t slot, const uint8_t *master_key, const uint8_t *rand_number)
{
    ATCA_STATUS status;
    uint8_t sn[ATCA_SERIAL_NUM_SIZE];
    uint8_t symmetric_key[ATCA_KEY_SIZE];
    atca_temp_key_t temp_key, temp_key_derive;
    uint8_t rand_out[RANDOM_NUM_SIZE];
    atca_nonce_in_out_t nonce_params;
    atca_mac_in_out_t mac_params;
    uint8_t host_mac[MAC_SIZE];
    uint8_t device_mac[MAC_SIZE];
    struct atca_derive_key_in_out derivekey_params;

    do
    {
        // Read serial number for host-side MAC calculations
        if ((status = atcab_read_serial_number(sn)) != ATCA_SUCCESS)
        {
            break;
        }

        // Setup nonce command
        memset(&temp_key, 0, sizeof(temp_key));
        memset(&nonce_params, 0, sizeof(nonce_params));
        nonce_params.mode = NONCE_MODE_SEED_UPDATE;
        nonce_params.zero = 0;
        nonce_params.num_in = rand_number;
        nonce_params.rand_out = rand_out;
        nonce_params.temp_key = &temp_key;

        // Create random nonce
        if ((status = atcab_nonce_rand(nonce_params.num_in, rand_out)) != ATCA_SUCCESS)
        {
            break;
        }

        // Calculate nonce in host
        if ((status = atcah_nonce(&nonce_params)) != ATCA_SUCCESS)
        {
            break;
        }

        memset(&temp_key_derive, 0, sizeof(temp_key_derive));
        temp_key_derive.valid = 1;
        memcpy(temp_key_derive.value, sn, sizeof(sn)); // 32 bytes TempKey ( SN[0:8] with padded 23 zeros used in symmetric key calculation)

        // Parameters used deriving the symmetric key
        derivekey_params.mode = 0;
        derivekey_params.target_key_id = slot;
        derivekey_params.parent_key = master_key;
        derivekey_params.sn = sn;
        derivekey_params.target_key = symmetric_key;
        derivekey_params.temp_key = &temp_key_derive;

        // calculate the symmetric_diversified_key
        if ((status = atcah_derive_key(&derivekey_params)) != ATCA_SUCCESS)
        {
            break;
        }

        // Setup MAC command
        memset(&mac_params, 0, sizeof(mac_params));
        mac_params.mode = MAC_MODE_BLOCK2_TEMPKEY | MAC_MODE_INCLUDE_SN; // Block 1 is a key, block 2 is TempKey
        mac_params.key_id = slot;
        mac_params.challenge = NULL;
        mac_params.key = symmetric_key;
        mac_params.otp = NULL;
        mac_params.sn = sn;
        mac_params.response = host_mac;
        mac_params.temp_key = &temp_key;

        // Generate the MAC command from the device
        if ((status = atcab_mac(mac_params.mode, mac_params.key_id, mac_params.challenge, device_mac)) != ATCA_SUCCESS)
        {
            break;
        }

        // Calculate the MAC on the host side
        if (( status = atcah_mac(&mac_params)) != ATCA_SUCCESS)
        {
            break;
        }

        //Check whether the MAC calculated on host is same as that generated from the device
        if (memcmp(device_mac, host_mac, 32) != 0)
        {
            status = ATCA_CHECKMAC_VERIFY_FAILED;
        }

    }
    while (0);

    return status;
}

