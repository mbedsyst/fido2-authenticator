#ifndef CRYPTO_HMAC_H
#define CRYPTO_HMAC_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    CRYPTO_HMAC_SHA256,
} crypto_hmac_alg_t;

int crypto_hmac(crypto_hmac_alg_t alg,
                const uint8_t *key, size_t key_len,
                const uint8_t *input, size_t input_len,
                uint8_t *mac, size_t *mac_len);

#endif // CRYPTO_HMAC_H
