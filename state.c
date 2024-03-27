/* 
 * Get the chip's internal status.
 */

#include <stdio.h>   // printf
#include "atca_config.h"
#include "cryptoauthlib.h"
#include "common.h"

/*
Byte 0:
Bit 7: TempKey.NoMacFlag
Bit 6: TempKey.GenKeyData
Bit 5: TempKey.GenDigData
Bit 4: TempKey.SourceFlag
Bits 3–0: TempKey.KeyID

Byte 1:
Bit 7: TempKey.Valid
Bits 6–3: AuthKeyID: The slot ID on which an authorization was performed
Bit 2: AuthValid: A valid authorization sequence has been performed
Bit 1: SRAM RNG: Seed has been updated this power cycle
Bit 0: EEPROM RNG: Seed has been updated this power cycle
*/
typedef struct ATCA_PACKED state_s
{
    uint8_t TempKey_KeyID : 4;
    bool TempKey_SourceFlag : 1;
    bool TempKey_GenDigData : 1;
    bool TempKey_GenKeyData : 1;
    bool TempKey_NoMacFlag  : 1;

    bool EEPROM_RNG : 1;
    bool SRAM_RNG : 1;
    bool AuthValid : 1;
    uint8_t AuthKeyID : 4;
    bool TempKey_Valid : 1;

    uint8_t byte2;
    uint8_t byte3;
} state_t;


int main(int argc, const char **argv) {
    ATCA_STATUS status;
    uint8_t output[INFO_SIZE] = {0};
    state_t *state = (state_t *) output;

    // initialize CryptoAuthLib for an ECC default I2C interface
    if (atcab_init(&cfg_atecc608_i2c) != ATCA_SUCCESS) {
        printf("Init error\n");
        exit(1);
    }

    status = atcab_info_base(INFO_MODE_STATE, 0, output);

    if (status != ATCA_SUCCESS) {
        printf("Nonce error: %d\n", status);
        exit(1);
    }

    //printhex("Info", output, INFO_SIZE, "");

    printf("TempKey.NoMacFlag:  %s\n", state->TempKey_NoMacFlag  ? "1" : "0");
    printf("TempKey.GenKeyData: %s\n", state->TempKey_GenKeyData ? "1" : "0");
    printf("TempKey.GenDigData: %s\n", state->TempKey_GenDigData ? "1" : "0");
    printf("TempKey.SourceFlag: %s\n", state->TempKey_SourceFlag ? "1" : "0");
    printf("TempKey.KeyID:      %d\n", state->TempKey_KeyID);
    printf("TempKey.Valid:      %s\n", state->TempKey_Valid      ? "1" : "0");
    printf("Auth Key ID:        %d\n", state->AuthKeyID);
    printf("Auth Valid:         %s\n", state->AuthValid          ? "1" : "0");
    printf("SRAM RNG:           %s\n", state->SRAM_RNG           ? "1" : "0");
    printf("EEPROM RNG:         %s\n", state->EEPROM_RNG         ? "1" : "0");
}

