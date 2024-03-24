#ifndef COMMON_H
#define COMMON_H

#include "atca_iface.h"
extern ATCAIfaceCfg cfg_atecc608_i2c;

void printhex(const char *label, char *buffer, size_t len, const char *spacer);

#endif /* COMMON_H */
