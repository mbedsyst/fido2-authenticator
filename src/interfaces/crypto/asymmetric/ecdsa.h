#ifndef CRYPTO_ECDSA_H
#define CRYPTO_ECDSA_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    CRYPTO_ECDSA_ALG_P256_SHA256,
    // could extend later (P384, SHA-512, etc.)
} crypto_ecdsa_alg_t;

/**
 * Sign a digest using ECDSA.
 *
 * @param key_id   Reference to private key (PSA key handle or internal ID).
 * @param alg      Algorithm (currently P-256 + SHA-256).
 * @param hash     Input message digest.
 * @param hash_len Length of digest.
 * @param sig      Output signature buffer.
 * @param sig_len  Input: size of sig buffer. Output: actual sig length.
 */
int crypto_ecdsa_sign(uint32_t key_id,
                      crypto_ecdsa_alg_t alg,
                      const uint8_t *hash, size_t hash_len,
                      uint8_t *sig, size_t *sig_len);

/**
 * Verify a digest using ECDSA.
 *
 * @param key_id   Reference to public key (PSA key handle or internal ID).
 * @param alg      Algorithm (currently P-256 + SHA-256).
 * @param hash     Input message digest.
 * @param hash_len Length of digest.
 * @param sig      Input signature buffer.
 * @param sig_len  Length of signature.
 */
int crypto_ecdsa_verify(uint32_t key_id,
                        crypto_ecdsa_alg_t alg,
                        const uint8_t *hash, size_t hash_len,
                        const uint8_t *sig, size_t sig_len);

#endif // CRYPTO_ECDSA_H
