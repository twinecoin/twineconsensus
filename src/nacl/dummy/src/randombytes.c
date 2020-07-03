/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <stdio.h>
#include <stdlib.h>

#include "randombytes.h"

/* This is a dummy implementation of randombytes to avoid modifying the nacl
 * source files.  The function is only called for key pair generation.  The
 * consensus library does not generate keys.
 */
void randombytes(unsigned char* buf,unsigned long long len) {
  fprintf(stderr, "Illegal state:  randombytes() should not be callable\n");
  exit(EXIT_FAILURE);
}
