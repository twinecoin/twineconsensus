/**              Copyright (c) 2017 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include <assert.h>
#include "tw_sha256.h"

int tw_h256_equal(const tw_h256* a, const tw_h256* b) {
  for (int i = 0; i < 32; i++) {
    if (a->h[i] != b->h[i]) {
      return 0;
    }
  }
  return 1;
}

tw_sha256_state tw_create_sha256_state() {
  secp256k1_sha256_t sha256_state;
  secp256k1_sha256_initialize(&sha256_state);
  return sha256_state;
}

void tw_sha256_reset(tw_sha256_state* sha256_state) {
  secp256k1_sha256_initialize(sha256_state);
}

void tw_sha256_write(tw_sha256_state* sha256_state, const tw_u8 *data, const size_t size) {
  secp256k1_sha256_write(sha256_state, data, size);
}

void tw_sha256_done(tw_sha256_state* sha256_state, tw_h256* out) {
  secp256k1_sha256_finalize(sha256_state, out->h);
}

void tw_sha256_hash(tw_h256* out, const tw_u8 *data, const size_t size) {
  tw_sha256_state sha256_state = tw_create_sha256_state();
  tw_sha256_write(&sha256_state, data, size);
  tw_sha256_done(&sha256_state, out);
}
