AM_CFLAGS                    = -I$(top_srcdir)/include -fvisibility=hidden -std=c99 -pedantic -funroll-loops

CLEANFILES                   = tw_lic_check

noinst_LIBRARIES             = librandombytes.a libcrypto_verify.a libed25519.a libsha512.a libsha256.a

librandombytes_a_SOURCES     = nacl/dummy/src/randombytes.c
librandombytes_a_CFLAGS      = -fPIC -I./nacl/include

libcrypto_verify_a_SOURCES   = ./nacl/src/crypto_verify/32/src/verify.c
libcrypto_verify_a_CFLAGS    = -fPIC -I./nacl/include -I./nacl/src/crypto_verify/32/include

libed25519_a_SOURCES         = ./nacl/src/ed25519/src/fe25519.c
libed25519_a_SOURCES        += ./nacl/src/ed25519/src/ge25519.c
libed25519_a_SOURCES        += ./nacl/src/ed25519/src/sc25519.c
libed25519_a_SOURCES        += ./nacl/src/ed25519/src/sign.c
libed25519_a_CFLAGS          = -fPIC -I./nacl/include -I./nacl/src/ed25519/include

libsha512_a_SOURCES          = ./nacl/src/sha512/src/hash.c
libsha512_a_SOURCES         += ./nacl/src/sha512_block/src/blocks.c
libsha512_a_CFLAGS           = -fPIC -I./nacl/include -I./nacl/src/sha512/include -I./nacl/src/sha512_block/include

libsha256_a_SOURCES          = ./nacl/src/sha256/src/hash.c
libsha256_a_SOURCES         += ./nacl/src/sha256_block/src/blocks.c
libsha256_a_CFLAGS           = -fPIC -I./nacl/include -I./nacl/src/sha256/include -I./nacl/src/sha256_block/include

lib_LTLIBRARIES		     = libtwineconsensus.la

TWINECONSENSUS_C_FILES       = twineconsensus.c ./math/src/tw_uint.c ./test/src/tw_test_harness.c
TWINECONSENSUS_LIB_FILES     = librandombytes.a libcrypto_verify.a libed25519.a libsha512.a libsha256.a
libtwineconsensus_la_SOURCES = $(TWINECONSENSUS_C_FILES) $(TWINECONSENSUS_LIB_FILES)
libtwineconsensus_la_CFLAGS  = -L../src/ -led25519 -lcrypto_verify -lrandombytes -lsha512 -lsha256
libtwineconsensus_la_LDFLAGS = -export-symbols export-functions -no-undefined -version-info @twcon_ver@:@twcon_rev@:@twcon_age@
libtwineconsensus_LDADD      = $(TWINECONSENSUS_LIB_FILES)

include_HEADERS              = ../include/twineconsensus.h

all-local: tw_lic_check

install-exec-hook:
	../scripts/install_natives.sh $(host) $(prefix) @twcon_ver@ $(TW_NATIVE_PATH)

tw_lic_check: $(include_HEADERS) $(libtwineconsensus_la_SOURCES)
	../scripts/check_licence.sh $(include_HEADERS) $(TWINECONSENSUS_C_FILES)
	touch tw_lic_check
