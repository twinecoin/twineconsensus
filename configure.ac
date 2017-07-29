define(TWCON_VER,    0)   # Increment for interface changes (revision back to zero)
define(TWCON_REV,    0)   # Increment for other changes
define(TWCON_AGE,    0)   # Increment if interfade changes are backward compatible (zero otherwise)

AC_INIT([TwineCoin Consensus Library],
        [TCON_MAJ_VER.TCON_MIN_VER.TCON_REV],
        [https://github.com/twinecoin/twineconsensus/issues],
        [twineconsensus],
        [https://twinecoin.org])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])

AC_CONFIG_SRCDIR([src])
AC_PREFIX_DEFAULT([${PWD}/out])

AM_INIT_AUTOMAKE
AM_PROG_AR

AC_PROG_CC
AC_C_CONST
LT_INIT

AC_SUBST(twcon_ver, TWCON_VER)
AC_SUBST(twcon_rev, TWCON_REV)
AC_SUBST(twcon_age, TWCON_AGE)

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT

