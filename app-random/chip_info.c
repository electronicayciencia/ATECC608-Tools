/* 
 * gcc -I../cryptoauthlib/lib -L../cryptoauthlib/lib -lcryptoauth chip_info.c
 *
 */

//#include <string.h>  // strlen
//#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "device_cfg.h"

int main(void) {

    puts("Library symbols:");
    printf("  ATCA_CA_SUPPORT: %d\n", ATCA_CA_SUPPORT);
    printf("  ATCA_CA2_SUPPORT: %d\n", ATCA_CA2_SUPPORT);
    printf("  ATCA_TA_SUPPORT: %d\n", ATCA_TA_SUPPORT);
#ifdef ATCA_ATECC608_SUPPORT
    puts("  ATCA_ATECC608_SUPPORT");
#endif


    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    // Device type
    ATCADeviceType dev_type = atcab_get_device_type();
    printf("Device type: 0x%02x\n", dev_type);
    printf("  CA:  %s\n", atcab_is_ca_device(dev_type)  ? "yes" : "no");
    printf("  CA2: %s\n", atcab_is_ca2_device(dev_type) ? "yes" : "no");
    printf("  TA:  %s\n", atcab_is_ta_device(dev_type)  ? "yes" : "no");


    // Zone sizes
    puts("Sizes:");

    size_t config_size;
    atcab_get_zone_size(ATCA_ZONE_CONFIG, 0, &config_size);
    printf("  Config: %d\n", config_size);

    size_t otp_size;
    atcab_get_zone_size(ATCA_ZONE_OTP, 0, &otp_size);
    printf("  OTP: %d\n", otp_size);
    
    size_t data_size;
    atcab_get_zone_size(ATCA_ZONE_DATA, 0, &data_size);
    printf("  Data: %d\n", data_size);

    /*
    atcab_bin2hex(digest, sizeof(digest), buffer, &buffer_size);

    puts("");
    puts("SHA256 output:");
    puts(buffer);
    puts("");
    */

}

