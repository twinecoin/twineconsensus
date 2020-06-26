AM_CFLAGS =                   -I$(top_srcdir)/include -std=c99 @CHECK_CFLAGS@
LDADD =                       $(top_builddir)/src/libtwineconsensus.la @CHECK_LIBS@
AM_LDFLAGS =                  -static

CLEANFILES =                  tw_lic_check

TESTS =                       test_placeholder
check_PROGRAMS =              test_placeholder

TW_TWINE_HEADER =             $(top_builddir)/include/twineconsensus.h vectors/*.h

test_placeholder_SOURCES =    test_placeholder.c $(TW_TWINE_HEADER)

TW_ALL_SRC =                  $(test_placeholder_SOURCES)

all-local: tw_lic_check

tw_lic_check: $(TW_ALL_SRC)
	../scripts/check_licence.sh $(TW_ALL_SRC)
	touch tw_lic_check
