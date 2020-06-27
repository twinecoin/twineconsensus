# Project Version

define(_TWCON_MAJ, 0)        # Twinecoin Consensus Major Release
define(_TWCON_MIN, 0)        # Twinecoin Consensus Minor Release
define(_TWCON_REV, dev)      # Twinecoin Consensus Revision

# Interface Versioning

define(_TWCON_IF_VER,   0)   # Increment for interface changes (set revision to zero)
define(_TWCON_IF_REV,   0)   # Increment for changes that are back/forward compatible
define(_TWCON_IF_AGE,   0)   # Increment if interface changes are backward compatible (zero otherwise)

# Initialize Automake
AC_INIT([Twinecoin Consensus Library],
        [_TWCON_MAJ, _TWCON_MIN, _TWCON_REV],
        [https://github.com/twinecoin/twineconsensus/issues],
        [twineconsensus],
        [https://twinecoin.org])

# Set directories for scripts and cache files
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])

# Defines can be accessed by c files
AC_DEFINE(TWCON_MAJ, _TWCON_MAJ, [Twinecoin Consensus Major Release])
AC_DEFINE(TWCON_MIN, _TWCON_MIN, [Twinecoin Consensus Minor Release])
AC_DEFINE(TWCON_REV, _TWCON_REV, [Twinecoin Consensus Revision])

AC_DEFINE(TWCON_IF_VER, _TWCON_IF_VER, [Twinecoin Interface Version])
AC_DEFINE(TWCON_IF_REV, _TWCON_IF_REV, [Twinecoin Interface Revision])
AC_DEFINE(TWCON_IF_AGE, _TWCON_IF_AGE, [Twinecoin Interface Age])

# Send defines to config.h
AC_CONFIG_HEADERS([src/config.h])

# Verify in project root directory
AC_CONFIG_SRCDIR([src/twineconsensus.c])

# Set location for output files
AC_PREFIX_DEFAULT([${PWD}/out])

# General automake script init
AM_INIT_AUTOMAKE([subdir-objects])

# Verify c compiler
AC_PROG_CC

# Required for tests
AM_PROG_AR 

# Initialize libtool
LT_INIT

# Verify that check available for tests
PKG_CHECK_MODULES([CHECK], [check >= 0.9.6])

AC_SUBST(twcon_ver, _TWCON_IF_VER)
AC_SUBST(twcon_rev, _TWCON_IF_REV)
AC_SUBST(twcon_age, _TWCON_IF_AGE)

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])

AC_OUTPUT
