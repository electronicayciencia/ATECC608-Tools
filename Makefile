GCC=gcc -Icryptoauthlib/lib -Lcryptoauthlib/lib
LIB=-lcryptoauth
DEPS_H=atca_config.h common.h
DEPS_C=common.c

binaries = random chip_info sha setup_608 aes_encrypt aes_decrypt \
	nonce_rand load_tempkey write_8 serial mac gendig read_slot xor \
	check_mac state write_enc write_slot derivekey genecckey

.PHONY: clean all

all: $(binaries)

clean:
	rm -f $(binaries) *.o

$(binaries): %: %.c $(DEPS_H) $(DEPS_C)
	$(GCC) -o $@ $< $(DEPS_C) $(LIB)

