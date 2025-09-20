#ifndef CRYPTO_ECDH_H
#define CRYPTO_ECDH_H

#include <stddef.h>
#include <stdint.h>

#include "crypto_common.h"

/**
 * Perform ECDH key agreement.
 *
 * @param priv_key_id   Reference to private key.
 * @param peer_pub      Peer’s public key bytes.
 * @param peer_pub_len  Length of public key.
 * @param alg           Algorithm (currently P-256).
 * @param shared        Output shared secret buffer.
 * @param shared_len    Input: size of buffer. Output: actual length.
 */
crypto_status_t crypto_ecdh_derive(uint32_t priv_key_id,
                       const uint8_t *peer_pub, size_t peer_pub_len,
                       crypto_ecdh_alg_t alg,
                       uint8_t *shared, size_t *shared_len);

#endif // CRYPTO_ECDH_H
