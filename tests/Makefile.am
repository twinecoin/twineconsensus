AM_CFLAGS =                   -I$(top_srcdir)/include -std=c99 @CHECK_CFLAGS@
AM_CFLAGS +=                  -I$(top_srcdir)/src/math/src
AM_CFLAGS +=                  -I$(top_srcdir)/src/crypt/src
AM_CFLAGS +=                  -I$(top_srcdir)/src/test/src
LDADD =                       $(top_builddir)/src/libtwineconsensus.la @CHECK_LIBS@
# AM_LDFLAGS =                  -static

CLEANFILES =                  tw_lic_check

TESTS =                       test_uint test_hash test_ed25519
check_PROGRAMS =              test_uint test_hash test_ed25519

TW_TWINE_HEADER =             $(top_builddir)/include/twineconsensus.h vectors/*.h

test_uint_SOURCES =           test_uint.c $(TW_TWINE_HEADER)
test_hash_SOURCES =           test_hash.c $(TW_TWINE_HEADER)
test_ed25519_SOURCES =        test_ed25519.c $(TW_TWINE_HEADER)

TW_ALL_SRC =                  $(test_uint_SOURCES)

all-local: tw_lic_check

tw_lic_check: $(TW_ALL_SRC)
	../scripts/check_licence.sh $(TW_ALL_SRC)
	touch tw_lic_check
