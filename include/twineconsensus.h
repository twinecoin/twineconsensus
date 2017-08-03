/**              Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#ifndef TWINECONSENSUS_H
#define TWINECONSENSUS_H

#include <stdint.h>

/**  
 * Interface version test
 *
 * Tests if the library supports an interface version
 * 
 *   version:  The library version
 *   return:  non-zero if the version is supported
 */
extern uint32_t twlib_test_version(uint32_t version);

#endif
