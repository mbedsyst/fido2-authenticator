#ifndef CRYPTO_HASH_H
#define CRYPTO_HASH_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    CRYPTO_HASH_SHA256,
    CRYPTO_HASH_SHA512,
} crypto_hash_alg_t;

int crypto_hash(crypto_hash_alg_t alg,
                const uint8_t *input, size_t input_len,
                uint8_t *digest, size_t *digest_len);

#endif // CRYPTO_HASH_H
