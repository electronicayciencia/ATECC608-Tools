// ATCA Configuration

ATCAIfaceCfg atecc608_i2c = {
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

