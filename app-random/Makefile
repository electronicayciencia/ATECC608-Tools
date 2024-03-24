GCC=gcc -I../cryptoauthlib/lib -L../cryptoauthlib/lib -lcryptoauth
DEPS_H=atca_config.h common.h
DEPS_C=common.c

all: random chip_info sha setup_608 aes_encrypt random_nonce write_8	eyc_original

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

random_nonce: $(DEPS_C) $(DEPS_H) random_nonce.c
	$(GCC) -o random_nonce random_nonce.c $(DEPS_C)

write_8: $(DEPS_C) $(DEPS_H) write_8.c
	$(GCC) -o write_8 write_8.c $(DEPS_C)

eyc_original: $(DEPS_C) $(DEPS_H) eyc_original.c
	$(GCC) -o eyc_original eyc_original.c $(DEPS_C)

