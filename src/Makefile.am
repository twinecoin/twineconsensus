AM_CPPFLAGS                  = -I$(top_srcdir)/include

CLEANFILES                   = tw_lic_check

lib_LTLIBRARIES		     = libtwineconsensus.la
libtwineconsensus_la_SOURCES = twineconsensus.c tw_u256.c
libtwineconsensus_la_LDFLAGS = -no-undefined -version-info @twcon_ver@:@twcon_rev@:@twcon_age@

include_HEADERS              = ../include/twineconsensus.h

all-local: tw_lic_check

tw_lic_check: $(include_HEADERS) $(libtwineconsensus_la_SOURCES)
	../scripts/check_licence.sh $(include_HEADERS) $(libtwineconsensus_la_SOURCES)
	touch tw_lic_check
