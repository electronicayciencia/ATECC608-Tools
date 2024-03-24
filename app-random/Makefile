GCC=gcc -I../cryptoauthlib/lib -L../cryptoauthlib/lib -lcryptoauth
DEPS_H=atca_config.h common.h
DEPS_C=common.c

all: random chip_info sha setup_608 aes_encrypt nonce write_8 eyc_original serial mac

random: $(DEPS_C) $(DEPS_H) random.c
	$(GCC) -o random random.c $(DEPS_C)

chip_info: $(DEPS_C) $(DEPS_H) chip_info.c 
	$(GCC) -o chip_info chip_info.c $(DEPS_C)

sha: $(DEPS_C) $(DEPS_H) sha.c
	$(GCC) -o sha sha.c $(DEPS_C)

setup_608: $(DEPS_C) $(DEPS_H) setup_608.c
	$(GCC) -o setup_608 setup_608.c $(DEPS_C)

aes_encrypt: $(DEPS_C) $(DEPS_H) aes_encrypt.c
	$(GCC) -o aes_encrypt aes_encrypt.c $(DEPS_C)

nonce: $(DEPS_C) $(DEPS_H) nonce.c
	$(GCC) -o nonce nonce.c $(DEPS_C)

write_8: $(DEPS_C) $(DEPS_H) write_8.c
	$(GCC) -o write_8 write_8.c $(DEPS_C)

eyc_original: $(DEPS_C) $(DEPS_H) eyc_original.c
	$(GCC) -o eyc_original eyc_original.c $(DEPS_C)

serial: $(DEPS_C) $(DEPS_H) serial.c
	$(GCC) -o serial serial.c $(DEPS_C)

mac: $(DEPS_C) $(DEPS_H) mac.c
	$(GCC) -o mac mac.c $(DEPS_C)

