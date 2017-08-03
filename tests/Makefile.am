AM_CFLAGS =                   -I$(top_srcdir)/include @CHECK_CFLAGS@
LDADD =                       $(top_builddir)/src/libtwineconsensus.la @CHECK_LIBS@

CLEANFILES =                  tw_lic_check

TESTS =                       test_version_check test_u256_check
check_PROGRAMS =              test_version_check test_u256_check

test_version_check_SOURCES =  test_version_check.c $(top_builddir)/include/twineconsensus.h
test_u256_check_SOURCES =     test_u256.c $(top_builddir)/include/twineconsensus.h

all-local: tw_lic_check

tw_lic_check: $(test_version_check_SOURCES) $(test_u256_check_SOURCES)
	../scripts/check_licence.sh $(test_version_check_SOURCES) $(test_u256_check_SOURCES)
	touch tw_lic_check
