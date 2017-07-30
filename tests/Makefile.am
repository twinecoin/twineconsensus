AM_CPPFLAGS                  = -I$(top_srcdir)/include

TESTS =                       test_version_check
check_PROGRAMS =              test_version_check
test_version_check_SOURCES =  test_version_check.c $(top_builddir)/src/twineconsensus.h
test_version_check_CFLAGS =   @CHECK_CFLAGS@
test_version_check_LDADD =    $(top_builddir)/src/libtwineconsensus.la @CHECK_LIBS@
