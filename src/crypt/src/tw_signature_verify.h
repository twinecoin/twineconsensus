/**          Copyright (c) 2017-2019 Twinecoin Developers
 * The file is licenced under the MIT software license, see LICENCE
 * or http://www.opensource.org/licenses/mit-license.php.
 */
#ifndef TW_SIGNATURE_VERIFY_H
#define TW_SIGNATURE_VERIFY_H

#include "tw_uint.h"

/* Checks an ed25519 signature and returns the message
 *
 * The message and message_length arguments are outputs.  The message
 * length cannot exceed the signature length.
 *
 * message:          The message is placed into this buffer
 * message_length:   The message length is placed in this field
 * signature:        The signature to be tested (includes signed message)
 * signature_length: The signature length
 * public_key:       The public key (fixed 32 bytes)
 */
int tw_checksig_ed25519(tw_u8* message, tw_u32* message_length,
                        const tw_u8* signature, const tw_u32 signature_length,
                        const tw_u8* public_key);
#endif
