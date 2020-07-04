/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <string.h>

#include "tw_hash.h"

#include "crypto_hash_sha256.h"
#include "crypto_hash_sha512.h"

#define TW_U512_BYTES 64

tw_u512 tw_le_to_tw_u512(const tw_u8* bytes) {
  tw_u512 y;
  int k = 0;
  for (int i = 0; i < 8; i++) {
    tw_u64 word = 0;
    for (int j = 0; j < 8; j++) {
      tw_u8 b = bytes[k++];
      word = (word >> 8) | (((tw_u64) b) << 56);
    }
    y.d[i] = word;
  }
  return y;
}

tw_u512 tw_sha_256(const tw_u8* message, const tw_u64 len) {
  tw_u8 digest[TW_U512_BYTES];
  memset(digest + 32, 0, sizeof(*digest) * 32);
  crypto_hash_sha256_ref(digest, message, len);
  return tw_le_to_tw_u512(digest);
}

tw_u512 tw_dsha_256(const tw_u8* message, const tw_u64 len) {
  tw_u8 digest[TW_U512_BYTES];
  crypto_hash_sha256_ref(digest, message, len);
  tw_u8 digest2[TW_U512_BYTES];
  crypto_hash_sha256_ref(digest2, digest, 32);
  memset(digest2 + 32, 0, sizeof(*digest) * 32);
  return tw_le_to_tw_u512(digest2);
}

tw_u512 tw_sha_512(const tw_u8* message, const tw_u64 len) {
  tw_u8 digest[TW_U512_BYTES];
  crypto_hash_sha512_ref(digest, message, len);
  return tw_le_to_tw_u512(digest);
}

tw_u512 tw_dsha_512(const tw_u8* message, const tw_u64 len) {
  tw_u8 digest[TW_U512_BYTES];
  crypto_hash_sha512_ref(digest, message, len);
  tw_u8 digest2[TW_U512_BYTES];
  crypto_hash_sha512_ref(digest2, digest, 64);
  return tw_le_to_tw_u512(digest2);
}
