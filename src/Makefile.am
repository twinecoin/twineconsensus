## Process this file with automake to produce Makefile.in.

lib_LTLIBRARIES		     = libtwineconsensus.la
libtwineconsensus_la_SOURCES = twineconsensus.c
libtwineconsensus_la_LDFLAGS = -no-undefined -version-info @twcon_ver@:@twcon_rev@:@twcon_age@

include_HEADERS              = twineconsensus.h

bin_PROGRAMS                 = twineconsensustest
twineconsensustest_SOURCES   = main.c
twineconsensustest_LDADD     = libtwineconsensus.la

