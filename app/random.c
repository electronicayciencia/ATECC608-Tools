// gcc -I../cryptoauthlib/lib -L../cryptoauthlib/lib -lcryptoauth -o random random.c
//


#include "cryptoauthlib.h"
int main(void) {
  uint8_t random_number[32];

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
  atcab_random(random_number); // get a random number from the chip
  for(int i = 0; i < 32; i++) {
    printf("%02x ", random_number[i]);
  }
  printf("\n");
}
