/* 
 * gcc -I../cryptoauthlib/lib -L../cryptoauthlib/lib -lcryptoauth chip_info.c
 *
 */

#include <string.h>  // strlen
#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "device_cfg.h"

/* Versatile hex printer */
void printhex(const char *label, char *buffer, size_t len, const char *spacer) {
    printf("%s: ", label);

    for (int i = 0; i < len; i++)
        printf("%02x%s", buffer[i], spacer);

    puts("");
}


int main(void) {
    ATCA_STATUS status;

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
    bool config_locked;
    atcab_get_zone_size(ATCA_ZONE_CONFIG, 0, &config_size);
    atcab_is_config_locked(&config_locked);	
    printf("  Config: %d (locked: %s)\n", config_size, config_locked ? "yes" : "no");

    size_t otp_size;
    atcab_get_zone_size(ATCA_ZONE_OTP, 0, &otp_size);
    printf("  OTP: %d\n", otp_size);
    
    size_t data_size;
    bool data_locked;
    atcab_is_data_locked(&data_locked);	
    atcab_get_zone_size(ATCA_ZONE_DATA, 0, &data_size);
    printf("  Data: %d (locked: %s)\n", data_size, data_locked ? "yes" : "no");


    // Serial number
    uint8_t sn[ATCA_SERIAL_NUM_SIZE];
    if ((status = atcab_read_serial_number(sn)) != ATCA_SUCCESS) {
        printf("Error reading Serial Number: %d\n", status);
    }
    else {
        printhex("SN", sn, ATCA_SERIAL_NUM_SIZE, "");
    }
    puts("");

    // Read config data
    //  88 bytes for ATSHA devices, 128 bytes for ATECC devices and 48 bytes for Trust Anchor devices.
    uint8_t config_data[128];

    if (atcab_read_config_zone(config_data) != ATCA_SUCCESS) {
        printf("Error reading config\n");
        exit(1);
    }
    else {

        uint8_t buffer[128*4];
        size_t buffer_size = sizeof(buffer);
        atcab_bin2hex(config_data, config_size, buffer, &buffer_size);

        puts("");
        puts("Config data:");
        puts(buffer);
        puts("");

    }


    // Value of counters
    uint32_t counter_value = 0;
    atcab_counter(0, 0, &counter_value);
    printf("Counter 0: %6zu\n", counter_value);
    atcab_counter(0, 1, &counter_value);
    printf("Counter 1: %6zu\n", counter_value);
    puts("");


    // Try to read OTP
    uint8_t otp_data[64];
    if (atcab_read_bytes_zone(ATCA_ZONE_OTP, 0, 0, otp_data, 64) == ATCA_SUCCESS) {
        printf("OTP Contents: ");
        for(int i = 0; i < 64; i++) putchar(otp_data[i]);
        puts("");
    }
    else {
        puts("Cannot read OTP");
    }
    puts("");



    // Try to read data slot contents
    for (int i = 0; i <= 15; i++) {
        printf("Slot %2d: ", i);

        uint8_t slot_data[32];
        if (atcab_read_bytes_zone(ATCA_ZONE_DATA, i, 0, slot_data, 32) == ATCA_SUCCESS) {
            for(int i = 0; i < 32; i++) printf("%02x ", slot_data[i]);
            puts("");
        }
        else {
            puts("<secret>");
        }

    }
}

