AM_CFLAGS =                   -I$(top_srcdir)/include @CHECK_CFLAGS@
LDADD =                       $(top_builddir)/src/libtwineconsensus.la @CHECK_LIBS@

TESTS =                       test_version_check test_u256
check_PROGRAMS =              test_version_check test_u256
test_version_check_SOURCES =  test_version_check.c $(top_builddir)/src/twineconsensus.h
# test_version_check_CFLAGS =   @CHECK_CFLAGS@
# test_version_check_LDADD =    $(top_builddir)/src/libtwineconsensus.la @CHECK_LIBS@
