/**              Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include "twineconsensus.h"
#include "config.h"

uint32_t twlib_test_version(uint32_t version) {
  return version <= (uint32_t) TWCON_IF_VER;
}
