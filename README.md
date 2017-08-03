Twinecoin Consensus Library
===========================

This is the Twinecoin consensus library.

This library defines the consensus rules for Twinecoin.  The implementation is
minimal and does not include any functionality other than required to define
the Twinecoin consensus rules.

Building
--------

The library is built from the root directory.

    ./autogen.sh
    ./configure
    make
    make check
    make install

The build result is placed in the ./out directory.


Cross Compiling Windows DLL
---------------------------

Windows Dlls can be cross compiled in Linux.  The configure step is different.

    ./configure --host=x86_64-w64-mingw32

