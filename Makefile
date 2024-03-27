GCC=gcc -Icryptoauthlib/lib -Lcryptoauthlib/lib -lcryptoauth
DEPS_H=atca_config.h common.h
DEPS_C=common.c

all: random chip_info sha setup_608 aes_encrypt aes_decrypt nonce write_8 \
	serial mac gendig read_slot xor check_mac state write_enc

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

serial: $(DEPS_C) $(DEPS_H) serial.c
	$(GCC) -o serial serial.c $(DEPS_C)

mac: $(DEPS_C) $(DEPS_H) mac.c
	$(GCC) -o mac mac.c $(DEPS_C)

gendig: $(DEPS_C) $(DEPS_H) gendig.c
	$(GCC) -o gendig gendig.c $(DEPS_C)

read_slot: $(DEPS_C) $(DEPS_H) read_slot.c
	$(GCC) -o read_slot read_slot.c $(DEPS_C)

aes_decrypt: $(DEPS_C) $(DEPS_H) aes_decrypt.c
	$(GCC) -o aes_decrypt aes_decrypt.c $(DEPS_C)

xor: $(DEPS_C) $(DEPS_H) xor.c
	$(GCC) -o xor xor.c $(DEPS_C)

check_mac: $(DEPS_C) $(DEPS_H) check_mac.c
	$(GCC) -o check_mac check_mac.c $(DEPS_C)

state: $(DEPS_C) $(DEPS_H) state.c
	$(GCC) -o state state.c $(DEPS_C)

write_enc: $(DEPS_C) $(DEPS_H) write_enc.c
	$(GCC) -o write_enc write_enc.c $(DEPS_C)

