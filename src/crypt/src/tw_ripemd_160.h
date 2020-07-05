/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#ifndef TW_RIPEMD_160_H
#define TW_RIPEMD_160_H

#include "tw_uint.h"

/* Computes the RIPEMD-160 digest for a byte array as a little endian
 * tw_u512 integer
 * returns RIPEMD-160(message of length len)
 */
tw_u512 tw_ripemd_160(const tw_u8* message, const tw_u64 len);

#endif
