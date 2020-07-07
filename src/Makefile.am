AM_CFLAGS                    = -I$(top_srcdir)/include -fvisibility=hidden -std=c99 -pedantic -funroll-loops

CLEANFILES                   = tw_lic_check

noinst_LIBRARIES             = librandombytes.a libcrypto_verify.a libed25519.a libsha512.a libsha256.a

librandombytes_a_SOURCES     = ./lib_supercop/dummy/src/randombytes.c
librandombytes_a_CFLAGS      = -fPIC -I./lib_supercop/include

libcrypto_verify_a_SOURCES   = ./lib_supercop/src/crypto_verify/32/src/verify.c
libcrypto_verify_a_CFLAGS    = -fPIC -I./lib_supercop/include -I./lib_supercop/src/crypto_verify/32/include

libed25519_a_SOURCES         = ./lib_supercop/src/ed25519/src/fe_0.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_neg.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_msub.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_precomp_0.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_1.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_pow22523.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_p1p1_to_p2.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_scalarmult_base.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_add.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_sq2.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_p1p1_to_p3.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_sub.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_cmov.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_sq.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_p2_0.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_tobytes.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_copy.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_sub.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_p2_dbl.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/keypair.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_frombytes.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_tobytes.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_p3_0.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/open.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_invert.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_add.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_p3_dbl.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/sc_muladd.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_isnegative.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_double_scalarmult.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_p3_tobytes.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/sc_reduce.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_isnonzero.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_frombytes.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_p3_to_cached.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/sign.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/fe_mul.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_madd.c
libed25519_a_SOURCES        += ./lib_supercop/src/ed25519/src/ge_p3_to_p2.c

libed25519_a_CFLAGS          = -fPIC
libed25519_a_CFLAGS         += -I./lib_supercop/include
libed25519_a_CFLAGS         += -I./lib_supercop/src/ed25519/include
libed25519_a_CFLAGS         += -I./lib_nacl/include

libsha512_a_SOURCES          = ./lib_nacl/src/sha512/src/hash.c
libsha512_a_SOURCES         += ./lib_nacl/src/sha512_block/src/blocks.c
libsha512_a_CFLAGS           = -fPIC -I./lib_nacl/include -I./lib_nacl/src/sha512/include -I./lib_nacl/src/sha512_block/include

libsha256_a_SOURCES          = ./lib_nacl/src/sha256/src/hash.c
libsha256_a_SOURCES         += ./lib_nacl/src/sha256_block/src/blocks.c
libsha256_a_CFLAGS           = -fPIC -I./lib_nacl/include -I./lib_nacl/src/sha256/include -I./lib_nacl/src/sha256_block/include

lib_LTLIBRARIES		     = libtwineconsensus.la

TWINECONSENSUS_C_FILES       = twineconsensus.c
TWINECONSENSUS_C_FILES      += ./math/src/tw_uint.c
TWINECONSENSUS_C_FILES      += ./crypt/src/tw_hash.c ./crypt/src/tw_ripemd_160.c
TWINECONSENSUS_C_FILES      += ./test/src/tw_test_harness.c
TWINECONSENSUS_LIB_FILES     = librandombytes.a libcrypto_verify.a libed25519.a libsha512.a libsha256.a
libtwineconsensus_la_SOURCES = $(TWINECONSENSUS_C_FILES) $(TWINECONSENSUS_LIB_FILES)
libtwineconsensus_la_CFLAGS  = -L../src/ -led25519 -lcrypto_verify -lrandombytes -lsha512 -lsha256
libtwineconsensus_la_CFLAGS += -I./math/src/ -I./crypt/src -I./lib_nacl/include -I./lib_supercop/include
libtwineconsensus_la_LDFLAGS = -export-symbols export-functions -no-undefined -version-info @twcon_ver@:@twcon_rev@:@twcon_age@

include_HEADERS              = ../include/twineconsensus.h

all-local: tw_lic_check

install-exec-hook:
	../scripts/install_natives.sh $(host) $(prefix) @twcon_ver@ $(TW_NATIVE_PATH)

tw_lic_check: $(include_HEADERS) $(libtwineconsensus_la_SOURCES)
	../scripts/check_licence.sh $(include_HEADERS) $(TWINECONSENSUS_C_FILES)
	touch tw_lic_check
