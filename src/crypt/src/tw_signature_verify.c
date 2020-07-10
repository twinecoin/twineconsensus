/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#include "tw_signature_verify.h"
#include "crypto_sign_edwards25519sha512batch.h"

int tw_checksig_ed25519(tw_u8* message, tw_u32* message_length,
                        const tw_u8* signature, const tw_u32 signature_length,
                        const tw_u8* public_key) {
  unsigned long long message_length_ll;
  unsigned long long signature_length_ll = signature_length;

  int result = crypto_sign_edwards25519sha512batch_open(
                 message, &message_length_ll,
                 signature, signature_length_ll,
                 public_key);

  *message_length = message_length_ll;
  return result;
}
