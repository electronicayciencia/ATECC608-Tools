GCC=gcc -I../cryptoauthlib/lib -L../cryptoauthlib/lib -lcryptoauth
DEPS_H=atca_config.h device_cfg.h
DEPS_C=device_cfg.c

all: random chip_info sha setup_608

random: $(DEPS_C) $(DEPS_H) random.c
	$(GCC) -o random random.c $(DEPS_C)

chip_info: $(DEPS_C) $(DEPS_H) chip_info.c 
	$(GCC) -o chip_info chip_info.c $(DEPS_C)

sha: $(DEPS_C) $(DEPS_H) sha.c
	$(GCC) -o sha sha.c $(DEPS_C)

setup_608: $(DEPS_C) $(DEPS_H) setup_608.c
	$(GCC) -o setup_608 setup_608.c $(DEPS_C)

