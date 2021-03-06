/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#ifndef TW_HASH_H
#define TW_HASH_H

#include "tw_uint.h"

/* Converts a little endian digest into a tw_u512 integer
 * If the byte array is more than 64 bytes, the excess bytes
 * are ignored
 * return (tw_u512) little_endian_to_int(bytes)
 */
tw_u512 tw_le_to_tw_u512(const tw_u8* bytes);

/* Computes the SHA256 digest for a byte array as a little endian
 * tw_u512 integer
 * returns SHA256(message of length len)
 */
tw_u512 tw_sha_256(const tw_u8* message, const tw_u64 len);

/* Computes the double SHA256 digest for a byte array as a little
 * endian tw_u512 integer
 * returns SHA256(message of length len)
 */
tw_u512 tw_dsha_256(const tw_u8* message, const tw_u64 len);

/* Computes the SHA512 digest for a byte array as a little endian
 * tw_u512 integer
 * returns SHA512(message of length len)
 */
tw_u512 tw_sha_512(const tw_u8* message, const tw_u64 len);

/* Computes the double SHA512 digest for a byte array as a little
 * endian tw_u512 integer
 * returns SHA512(message of length len)
 */
tw_u512 tw_dsha_512(const tw_u8* message, const tw_u64 len);

#endif
